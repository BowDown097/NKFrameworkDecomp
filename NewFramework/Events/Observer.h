#pragma once

#include "EventManager.h"
#include "NewFramework/Platform/Shared/Assert.h"

















struct IObserver {
    virtual ~IObserver() = default;
    virtual void HandleEvent_Internal(IEvent* pBaseEvent, bool& bResult);
    virtual void ManagerDestroyed();
};

template<typename T>
class CObserver : public IObserver {
public:
    ~CObserver() override {
        if (m_pEventManager) {
            Unsubscribe(m_pEventManager);
        }
    }

    void HandleEvent_Internal(IEvent* pBaseEvent, bool& bResult) override {
        T* pEvent = dynamic_cast<T*>(pBaseEvent);
        if (pEvent) {
            HandleEvent(pEvent, bResult);
        }
    }

    void ManagerDestroyed() override {
        m_pEventManager = nullptr;
    }

    virtual void HandleEvent(T* pEvent) {}

    virtual void HandleEvent(T* pEvent, bool& bResult) {
        HandleEvent(pEvent);
    }

    void Subscribe(CEventManager* pEventManager) {
        if (m_pEventManager) {
            Unsubscribe(pEventManager);
        }
        if (pEventManager) {
            pEventManager->Subscribe(this, typeid(T));
            m_pEventManager = pEventManager;
        }
    }

    void Unsubscribe(CEventManager* pEventManager) {
        if (pEventManager) {
            if (m_pEventManager) {
                NKAssert(m_pEventManager == pEventManager, "Trying to unsubscribe from a different manager to our subscribed one"); ENFORCE_LINE(67);
            }
            pEventManager->Unsubscribe(this, typeid(T));
            m_pEventManager = nullptr;
        }
    }
private:
    CEventManager* m_pEventManager{}; // 0x08
};
