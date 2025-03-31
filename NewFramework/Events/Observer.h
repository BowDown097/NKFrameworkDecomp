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
        if (_pEventManager) {
            Unsubscribe(_pEventManager);
        }
    }

    void HandleEvent_Internal(IEvent* pBaseEvent, bool& bResult) override {
        T* pEvent = dynamic_cast<T*>(pBaseEvent);
        if (pEvent) {
            HandleEvent(pEvent, bResult);
        }
    }

    void ManagerDestroyed() override {
        _pEventManager = nullptr;
    }

    virtual void HandleEvent(T* pEvent) {}

    virtual void HandleEvent(T* pEvent, bool& bResult) {
        HandleEvent(pEvent);
    }

    void Subscribe(CEventManager* pEventManager) {
        if (_pEventManager) {
            Unsubscribe(pEventManager);
        }
        if (pEventManager) {
            pEventManager->Subscribe(this, typeid(T));
            _pEventManager = pEventManager;
        }
    }

    void Unsubscribe(CEventManager* pEventManager) {
        if (pEventManager) {
            if (_pEventManager) {
                NKAssert(_pEventManager == pEventManager, "Trying to unsubscribe from a different manager to our subscribed one"); ENFORCE_LINE(67);
            }
            pEventManager->Unsubscribe(this, typeid(T));
            _pEventManager = nullptr;
        }
    }
private:
    CEventManager* _pEventManager{}; // 0x08
};
