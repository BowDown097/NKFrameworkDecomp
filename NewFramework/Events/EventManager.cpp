#include "EventManager.h"

#include "NewFramework/Platform/Shared/Assert.h"
#include "Observer.h"

void CEventManager::SendEvent(IEvent* pEvent) {
    if (pEvent) {
        _SendEvent(pEvent, false);
        if (_uProcessingCount == 0) {
            RemovePendingObservers();
            AddPendingObservers();
        }
    }
}

void CEventManager::QueueEvent(IEvent* pEvent) {
    boost::unique_lock lock(_queueMutex);
    if (field_F0) {
        field_C0.push_back(pEvent);
    } else {
        field_90.push_back(pEvent);
    }
}

void CEventManager::Unsubscribe(IObserver* pObserver, const std::type_info& typeInfo) {
    if (_uProcessingCount > 0) {
        AddPendingRemovingObserver(pObserver, typeInfo);
        return;
    }

    auto it = _subscribedObserverMap.find(&typeInfo);
    if (it != _subscribedObserverMap.end()) {
        for (auto it2 = it->second.begin(); it2 != it->second.end();) {
            if (it2->second == pObserver) {
                it->second.erase(it2);
            } else {
                ++it2;
            }
        }
    }
}

void CEventManager::Subscribe(IObserver* pObserver, const std::type_info& typeInfo) {
    NKAssert(!_bDummy, "Something has tried to subscribe to a dummy event manager"); ENFORCE_LINE(44);

    if (_uProcessingCount > 0) {
        AddPendingSubscribingObserver(pObserver, typeInfo);
        return;
    }

    auto it = _subscribedObserverMap.emplace(&typeInfo, std::vector<std::pair<bool, IObserver*>>{});
    it.first->second.emplace_back(true, pObserver);
}

void CEventManager::Clear() {
    _subscribedObserverMap.clear();
}

void CEventManager::AddPendingSubscribingObserver(IObserver* pObserver, const std::type_info& typeInfo) {
    auto it = _subscribedObserverMap.find(&typeInfo);

    if (!_pendingRemovingObservers.empty()) {
        bool bFoundMatch{};
        for (auto it2 = _pendingRemovingObservers.begin(); it2 != _pendingRemovingObservers.end();) {
            if (*it2 == pObserver) {
                if (it != _subscribedObserverMap.end()) {
                    for (auto it3 = it->second.begin(); it3 != it->second.end(); ++it3) {
                        if (it3->second == pObserver) {
                            bFoundMatch = true;
                            break;
                        }
                    }
                }
                _pendingRemovingObservers.erase(it2);
            } else {
                ++it2;
            }
        }
        if (bFoundMatch) {
            return;
        }
    }

    _pendingObservers.emplace_back(pObserver, &typeInfo);
}

void CEventManager::AddPendingRemovingObserver(IObserver* pObserver, const std::type_info& typeInfo) {
    auto it = _subscribedObserverMap.find(&typeInfo);
    bool bFoundMatch{};

    if (it != _subscribedObserverMap.end()) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            if (it2->second == pObserver) {
                it2->first = false;
                bFoundMatch = true;
                break;
            }
        }
    }

    for (auto it2 = _pendingObservers.begin(); it2 != _pendingObservers.end();) {
        if (it2->first == pObserver) {
            _pendingObservers.erase(it2);
            bFoundMatch = false;
        } else {
            ++it2;
        }
    }

    if (bFoundMatch) {
        _pendingRemovingObservers.push_back(pObserver);
    }
}

void CEventManager::AddPendingObservers() {
    for (auto it = _pendingObservers.begin(); it != _pendingObservers.end(); ++it) {
        Subscribe(it->first, *it->second);
    }
    _pendingObservers.clear();
}

void CEventManager::RemovePendingObservers() {
    for (IObserver* pObserver : _pendingRemovingObservers) {
        for (auto it = _subscribedObserverMap.begin(); it != _subscribedObserverMap.end(); ++it) {
            for (auto it2 = it->second.begin(); it2 != it->second.end();) {
                if (it2->second == pObserver) {
                    it->second.erase(it2);
                } else {
                    ++it2;
                }
            }
        }
    }
    _pendingRemovingObservers.clear();
}

bool CEventManager::_SendEvent(IEvent* pEvent, bool bDontDeleteEvent) {
    ++_uProcessingCount;

    auto it = _subscribedObserverMap.find(&typeid(pEvent));
    bool bFoundMatch{};

    if (it != _subscribedObserverMap.end()) {
        bool bHandled{};
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it) {
            if (it2->first) {
                it2->second->HandleEvent_Internal(pEvent, bHandled);
                bFoundMatch = true;
            } else {
                bHandled = false;
            }
            if (bHandled) {
                break;
            }
        }
    }

    if (!bDontDeleteEvent || !pEvent->PersistsIfQueuedAndNotSeen() || bFoundMatch) {
        delete pEvent;
        pEvent = nullptr;
    }

    --_uProcessingCount;
    return pEvent == nullptr;
}

void CEventManager::ProcessQueue() {
    boost::unique_lock lock(_queueMutex);
    std::vector<IEvent*> processed;

    field_F0 = !field_F0;

    std::deque<IEvent*>& queue = field_F0 ? field_C0 : field_90;
    for (auto it = queue.begin(); it != queue.end();) {
        if (*it && !_SendEvent(*it, true)) {
            processed.push_back(*it);
        }
        queue.pop_front();
    }

    for (IEvent* pEvent : processed) {
        queue.push_back(pEvent);
    }

    if (_uProcessingCount == 0) {
        RemovePendingObservers();
        AddPendingObservers();
    }
}
