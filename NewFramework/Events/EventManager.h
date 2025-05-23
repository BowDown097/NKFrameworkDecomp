#pragma once

#include "Uncategorized/types.h"

#include <deque>
#include <map>

#include <boost/thread/recursive_mutex.hpp>

using CEventType = const std::type_info*;

struct IObserver;

struct IEvent {
    virtual ~IEvent() = default;
    virtual bool PersistsIfQueuedAndNotSeen() { return false; }
};

class CEventManager {
public:
    void Clear();
    void Subscribe(IObserver* pObserver, const std::type_info& typeInfo);
    void AddPendingSubscribingObserver(IObserver* pObserver, const std::type_info& typeInfo);
    void Unsubscribe(IObserver* pObserver, const std::type_info& typeInfo);
    void AddPendingRemovingObserver(IObserver* pObserver, const std::type_info& typeInfo);
    void AddPendingObservers();
    void RemovePendingObservers();
    void SendEvent(IEvent* pEvent);
    void QueueEvent(IEvent* pEvent);
    void ProcessQueue();
private:
    std::map<CEventType, std::vector<std::pair<bool, IObserver*>>> m_subscribedObserverMap; // 0x00
    std::vector<std::pair<IObserver*, CEventType>> m_pendingObservers; // 0x18
    std::vector<IObserver*> m_pendingRemovingObservers; // 0x30
    uint m_uProcessingCount{}; // 0x48
    boost::recursive_mutex m_queueMutex; // 0x50
    std::deque<IEvent*> field_90; // 0x90
    std::deque<IEvent*> field_C0; // 0xC0
    ushort field_F0{}; // 0xF0 // yes, this is a short value, despite being used as a bool. very weird.
    bool m_bDummy{}; // 0xF2

    bool _SendEvent(IEvent* pEvent, bool bDontDeleteEvent);
};
