#include "EventManager.h"

#include "NewFramework/Platform/Shared/Assert.h"
#include "Observer.h"

void CEventManager::SendEvent(IEvent* pEvent) {
    if (pEvent) {
        _SendEvent(pEvent, false);
        if (m_uProcessingCount == 0) {
            RemovePendingObservers();
            AddPendingObservers();
        }
    }
}

void CEventManager::QueueEvent(IEvent* pEvent) {
    boost::unique_lock lock(m_queueMutex);
    if (field_F0) {
        field_C0.push_back(pEvent);
    } else {
        field_90.push_back(pEvent);
    }
}

void CEventManager::Unsubscribe(IObserver* pObserver, const std::type_info& typeInfo) {
    if (m_uProcessingCount > 0) {
        AddPendingRemovingObserver(pObserver, typeInfo);
        return;
    }

    auto it = m_subscribedObserverMap.find(&typeInfo);
    if (it != m_subscribedObserverMap.end()) {
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
    NKAssert(!m_bDummy, "Something has tried to subscribe to a dummy event manager"); ENFORCE_LINE(44);

    if (m_uProcessingCount > 0) {
        AddPendingSubscribingObserver(pObserver, typeInfo);
        return;
    }

    auto it = m_subscribedObserverMap.emplace(&typeInfo, std::vector<std::pair<bool, IObserver*>>{});
    it.first->second.emplace_back(true, pObserver);
}

void CEventManager::Clear() {
    m_subscribedObserverMap.clear();
}

void CEventManager::AddPendingSubscribingObserver(IObserver* pObserver, const std::type_info& typeInfo) {
    auto it = m_subscribedObserverMap.find(&typeInfo);

    if (!m_pendingRemovingObservers.empty()) {
        bool bFoundMatch{};
        for (auto it2 = m_pendingRemovingObservers.begin(); it2 != m_pendingRemovingObservers.end();) {
            if (*it2 == pObserver) {
                if (it != m_subscribedObserverMap.end()) {
                    for (auto it3 = it->second.begin(); it3 != it->second.end(); ++it3) {
                        if (it3->second == pObserver) {
                            bFoundMatch = true;
                            break;
                        }
                    }
                }
                m_pendingRemovingObservers.erase(it2);
            } else {
                ++it2;
            }
        }
        if (bFoundMatch) {
            return;
        }
    }

    m_pendingObservers.emplace_back(pObserver, &typeInfo);
}

void CEventManager::AddPendingRemovingObserver(IObserver* pObserver, const std::type_info& typeInfo) {
    auto it = m_subscribedObserverMap.find(&typeInfo);
    bool bFoundMatch{};

    if (it != m_subscribedObserverMap.end()) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            if (it2->second == pObserver) {
                it2->first = false;
                bFoundMatch = true;
                break;
            }
        }
    }

    for (auto it2 = m_pendingObservers.begin(); it2 != m_pendingObservers.end();) {
        if (it2->first == pObserver) {
            m_pendingObservers.erase(it2);
            bFoundMatch = false;
        } else {
            ++it2;
        }
    }

    if (bFoundMatch) {
        m_pendingRemovingObservers.push_back(pObserver);
    }
}

void CEventManager::AddPendingObservers() {
    for (auto it = m_pendingObservers.begin(); it != m_pendingObservers.end(); ++it) {
        Subscribe(it->first, *it->second);
    }
    m_pendingObservers.clear();
}

void CEventManager::RemovePendingObservers() {
    for (IObserver* pObserver : m_pendingRemovingObservers) {
        for (auto it = m_subscribedObserverMap.begin(); it != m_subscribedObserverMap.end(); ++it) {
            for (auto it2 = it->second.begin(); it2 != it->second.end();) {
                if (it2->second == pObserver) {
                    it->second.erase(it2);
                } else {
                    ++it2;
                }
            }
        }
    }
    m_pendingRemovingObservers.clear();
}

bool CEventManager::_SendEvent(IEvent* pEvent, bool bDontDeleteEvent) {
    ++m_uProcessingCount;

    auto it = m_subscribedObserverMap.find(&typeid(pEvent));
    bool bFoundMatch{};

    if (it != m_subscribedObserverMap.end()) {
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

    --m_uProcessingCount;
    return pEvent == nullptr;
}

void CEventManager::ProcessQueue() {
    boost::unique_lock lock(m_queueMutex);
    std::vector<IEvent*> processed;

    ushort field_F0old = field_F0;
    field_F0 = !field_F0old;
    std::deque<IEvent*>& queue = field_F0old ? field_C0 : field_90;

    for (auto it = queue.begin(); it != queue.end(); queue.pop_front()) {
        if (*it && !_SendEvent(*it, true)) {
            processed.push_back(*it);
        }
    }

    for (IEvent* pEvent : processed) {
        queue.push_back(pEvent);
    }

    if (m_uProcessingCount == 0) {
        RemovePendingObservers();
        AddPendingObservers();
    }
}
