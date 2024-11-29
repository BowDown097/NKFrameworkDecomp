#pragma once
#include "NewFramework/Platform/Shared/LoggingComponent.h"
#include <boost/function/function1.hpp>
#include <vector>

namespace BehaviourTree
{
    enum ABool : bool { ABool_Failure, ABool_Success };
    enum class AState { Idle, Running, Success, Failure };
    enum class LoopCondition { None, LoopOnSuccess, LoopOnFailure, Looping };

    struct IBlackboard
    {
        virtual ~IBlackboard() = default;
    };

    struct Action
    {
        AState state{}; // 0x08
        AState lastState{}; // 0xC

        virtual void Start(IBlackboard* blackboard);
        virtual void Stop(IBlackboard* blackboard);
        virtual void Update(IBlackboard* blackboard, const float& elapsed) {}
        virtual ~Action() = default;
        virtual Action* clone();
        virtual std::string DebugString() { return ""; }
        virtual void CheckChildState(IBlackboard* blackboard);
        void PrintTreeUsingThisAsRoot(ILoggingComponent* component);
        void GraphTreeUsingThisAsRoot(ILoggingComponent* component, bool useLastState);
    };

    struct Leaf : Action {};

    template<typename T>
    struct Conditional : Leaf
    {
        T condition; // 0x10

        void Start(IBlackboard* blackboard) override
        {
            state = condition ? AState::Success : AState::Failure;
        }

        Action* clone() override
        {
            Conditional* out = new Conditional;
            out->condition = condition;
            return out;
        }
    };

    struct Decider : Leaf
    {
        float pValue; // 0x10

        std::string DebugString() override;
        void Start(IBlackboard* blackboard) override;
        Action* clone() override;
    };

    class Wait : public Leaf
    {
    public:
        float min; // 0x10
        float max; // 0x14

        std::string DebugString() override;
        void Start(IBlackboard* blackboard) override;
        void Update(IBlackboard* blackboard, const float& elapsed) override;
        Action* clone() override;
    private:
        float elapsed; // 0x18
        float timeout; // 0x1C
    };

    class Composite : public Action
    {
    public:
        std::vector<Action*>* actions = new std::vector<Action*>; // 0x10

        Composite(const LoopCondition& loopCondition) : loopCondition(loopCondition) {}
        Composite(const Composite& other);
        ~Composite() override;

        std::string DebugString() override;
        void Start(IBlackboard* blackboard) override;
        void Stop(IBlackboard* blackboard) override;
        void Update(IBlackboard* blackboard, const float& elapsed) override;
        void CheckLoop(IBlackboard* blackboard);

        // speculative: avoids repeating code
        inline void AddAction(Action* action)
        {
            if (actions)
                actions->push_back(action);
        }
    protected:
        int childIndex; // 0x18
        LoopCondition loopCondition; // 0x1C
    };

    class Sequence : public Composite
    {
    public:
        static Sequence* Create(const LoopCondition& loopCondition);
        void CheckChildState(IBlackboard* blackboard) override;
        Action* clone() override;
    private:
        Sequence(const LoopCondition& loopCondition) : Composite(loopCondition) {}
        Sequence(const Sequence& other) : Composite(other) {}
    };

    class Selector : public Composite
    {
    public:
        static Selector* Create(const LoopCondition& loopCondition);
        void CheckChildState(IBlackboard* blackboard) override;
        Action* clone() override;
    private:
        Selector(const LoopCondition& loopCondition) : Composite(loopCondition) {}
        Selector(const Selector& other) : Composite(other) {}
    };

    class Parallel : public Composite
    {
    public:
        static Parallel* Create(const LoopCondition& loopCondition, bool succeedOnOne, bool failOnOne);
        std::string DebugString() override;
        void Start(IBlackboard* blackboard) override;
        void Stop(IBlackboard* blackboard) override;
        void Update(IBlackboard* blackboard, const float& elapsed) override;
        void CheckChildState(IBlackboard* blackboard) override;
        Action* clone() override;
    private:
        bool succeedOnOne; // 0x20
        bool failOnOne; // 0x21

        Parallel(const LoopCondition& loopCondition, bool succeedOnOne, bool failOnOne)
            : Composite(loopCondition), succeedOnOne(succeedOnOne), failOnOne(failOnOne) {}
        Parallel(const Parallel& other)
            : Composite(other), succeedOnOne(other.succeedOnOne), failOnOne(other.failOnOne) {}
    };

    struct Decorator : Action
    {
        Action* child{}; // 0x10

        void Start(IBlackboard* blackboard) override;
        void Stop(IBlackboard* blackboard) override;
        void Update(IBlackboard* blackboard, const float& elapsed) override;
        ~Decorator() override;
        Action* clone() override;
        void CheckChildState(IBlackboard* blackboard) override;
    };

    struct NotDecorator : Decorator
    {
        void CheckChildState(IBlackboard* blackboard) override;
        Action* clone() override;
    };

    struct ForceState : Decorator
    {
        AState forceState; // 0x18

        void CheckChildState(IBlackboard* blackboard) override;
        std::string DebugString() override;
        Action* clone() override;
    };

    struct RandomDecorator : Decorator
    {
        float pValue; // 0x18

        std::string DebugString() override;
        void Start(IBlackboard* blackboard) override;
        Action* clone() override;
    };

    struct Loop : Decorator
    {
        LoopCondition loopCondition; // 0x18
        int count = -1; // 0x1C
        int index{}; // 0x20

        std::string DebugString() override;
        void CheckChildState(IBlackboard* blackboard) override;
        Action* clone() override;
    };

    class LoopOnCondition : public Decorator
    {
    public:
        static LoopOnCondition* Create(boost::function1<bool, IBlackboard*> func, Action* childAction);
        void CheckChildState(IBlackboard* blackboard) override;
        Action* clone() override;
    private:
        boost::function1<bool, IBlackboard*> func; // 0x18

        LoopOnCondition(boost::function1<bool, IBlackboard*> func, Action* childAction);
        LoopOnCondition(const LoopOnCondition& other);
    };

    namespace BHTreeDebugger
    {
        void PrintTreeState(Action* action, ILoggingComponent* component, int tabs);
        void GraphTreeState(Action* action, ILoggingComponent* component, bool useLastState);
        std::string ClassName(Action* action);
        void RecursiveGraph(Action* action, ILoggingComponent* component, bool useLastState, int index = 0);
    }
}
