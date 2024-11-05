#include "BehaviourTree.h"
#include <sstream>

#if defined(__has_include) && __has_include(<cxxabi.h>) && !defined(__GABIXX_CXXABI_H__)
#include <cxxabi.h>
#define NEED_DEMANGLE
#endif

namespace BehaviourTree
{
    void Action::Stop(IBlackboard*)
    {
        if (state != AState::Idle)
            lastState = state;
        state = AState::Idle;
    }

    void Action::PrintTreeUsingThisAsRoot(ILoggingComponent* component)
    {
        BHTreeDebugger::PrintTreeState(this, component, 2);
    }

    void Action::GraphTreeUsingThisAsRoot(ILoggingComponent* component, bool useLastState)
    {
        BHTreeDebugger::GraphTreeState(this, component, useLastState);
    }

    std::string Decider::DebugString()
    {
        std::ostringstream oss;
        oss << "P = " << pValue;
        return oss.str();
    }

    void Decider::Start(IBlackboard*)
    {
        state = (pValue >= ((float)rand() / (float)RAND_MAX)) ? AState::Success : AState::Failure;
    }

    Action* Decider::clone()
    {
        Decider* out = new Decider;
        out->pValue = pValue;
        return out;
    }

    std::string Wait::DebugString()
    {
        std::ostringstream oss;
        oss << "min: " << min << ", max: " << max;
        return oss.str();
    }

    void Wait::Start(IBlackboard*)
    {
        state = AState::Running;
        timeout = min;
        if (max != 0.0)
            timeout = min + (((float)rand() / (float)RAND_MAX) * (max - min));
        elapsed = 0;
    }

    void Wait::Update(IBlackboard*, const float& elapsed)
    {
        this->elapsed += elapsed;
        state = this->elapsed < timeout ? AState::Running : AState::Idle;
    }

    Action* Wait::clone()
    {
        Wait* out = new Wait;
        out->min = min;
        out->max = max;
        out->elapsed = elapsed;
        out->timeout = timeout;
        return out;
    }

    Composite::Composite(const Composite& other) : loopCondition(other.loopCondition)
    {
        actions->assign(other.actions->begin(), other.actions->end());
    }

    Composite::~Composite()
    {
        if (!actions)
            return;

        for (Action* action : *actions)
            if (action)
                delete action;
        delete actions;
    }

    std::string Composite::DebugString()
    {
        switch (loopCondition)
        {
        case LoopCondition::LoopOnSuccess:
            return "Loop on success";
        case LoopCondition::LoopOnFailure:
            return "Loop on failure";
        case LoopCondition::Looping:
            return "Looping";
        default:
            return "";
        }
    }

    void Composite::Start(IBlackboard* blackboard)
    {
        childIndex = 0;

        if (!actions || actions->empty())
        {
            state = AState::Failure;
            return;
        }

        bool loopOnFailure = loopCondition == LoopCondition::LoopOnFailure || loopCondition == LoopCondition::Looping;
        bool loopOnSuccess = loopCondition == LoopCondition::LoopOnSuccess || loopCondition == LoopCondition::Looping;

        if (!actions->at(childIndex))
        {
            if (loopOnFailure)
                state = AState::Running;
            else
                state = AState::Failure;
            return;
        }

        actions->at(childIndex)->Start(blackboard);
        CheckChildState(blackboard);

        if ((state == AState::Failure && loopOnFailure) || (state == AState::Success && loopOnSuccess))
            state = AState::Running;
    }

    void Composite::Stop(IBlackboard* blackboard)
    {
        if (actions && actions->size() > childIndex && actions->at(childIndex))
            actions->at(childIndex)->Stop(blackboard);

        childIndex = 0;
        Action::Stop(blackboard);
    }

    void Composite::Update(IBlackboard* blackboard, const float& elapsed)
    {
        if (state != AState::Running)
            return;

        if (actions->at(childIndex)->state == AState::Running)
            actions->at(childIndex)->Update(blackboard, elapsed);

        CheckChildState(blackboard);
        CheckLoop(blackboard);
    }

    void Composite::CheckLoop(IBlackboard* blackboard)
    {
        bool loopOnFailure = loopCondition == LoopCondition::LoopOnFailure || loopCondition == LoopCondition::Looping;
        bool loopOnSuccess = loopCondition == LoopCondition::LoopOnSuccess || loopCondition == LoopCondition::Looping;
        if ((state == AState::Failure && loopOnFailure) || (state == AState::Success && loopOnSuccess))
        {
            Stop(blackboard);
            Start(blackboard);
        }
    }

    Sequence* Sequence::Create(const LoopCondition& loopCondition)
    {
        return new Sequence(loopCondition);
    }

    void Sequence::CheckChildState(IBlackboard* blackboard)
    {
        Action* childAction = actions->at(childIndex);
        AState childState = childAction->state;

        if (childState == AState::Idle)
        {
            state = AState::Failure;
        }
        else if (childState != AState::Success)
        {
            state = childState;
        }
        else if (actions->size() - 1 == childIndex)
        {
            state = AState::Success;
        }
        else
        {
            childAction->Stop(blackboard);
            actions->at(++childIndex)->Start(blackboard);
            CheckChildState(blackboard);
        }
    }

    Action* Sequence::clone()
    {
        return new Sequence(*this);
    }

    Selector* Selector::Create(const LoopCondition& loopCondition)
    {
        return new Selector(loopCondition);
    }

    void Selector::CheckChildState(IBlackboard* blackboard)
    {
        Action* childAction = actions->at(childIndex);
        AState childState = childAction->state;

        if (childState == AState::Failure)
        {
            if (actions->size() - 1 != childIndex)
            {
                childAction->Stop(blackboard);
                Action* nextAction = actions->at(++childIndex);
                if (nextAction)
                {
                    nextAction->Start(blackboard);
                    CheckChildState(blackboard);
                }
            }
        }
        else if (childState != AState::Idle)
        {
            state = childState;
            return;
        }

        state = AState::Failure;
    }

    Action* Selector::clone()
    {
        return new Selector(*this);
    }

    Parallel* Parallel::Create(const LoopCondition& loopCondition, bool succeedOnOne, bool failOnOne)
    {
        return new Parallel(loopCondition, succeedOnOne, failOnOne);
    }

    std::string Parallel::DebugString()
    {
        std::string out = Composite::DebugString();
        out += !out.empty() ? "\\n" : "";

        std::string part = succeedOnOne ? "one" : "all";
        part.insert(0, "Succeed on ");
        out += part;

        out += "\\n";

        part = failOnOne ? "one" : "all";
        part.insert(0, "Fail on ");
        out += part;

        return out;
    }

    void Parallel::Start(IBlackboard* blackboard)
    {
        state = AState::Running;
        if (!actions || actions->empty())
        {
            state = AState::Failure;
            return;
        }

        for (Action* action : *actions)
            if (action)
                action->Start(blackboard);
    }

    void Parallel::Stop(IBlackboard* blackboard)
    {
        for (Action* action : *actions)
            if (action)
                action->Stop(blackboard);
        Action::Stop(blackboard); // clazy:exclude=skipped-base-method
    }

    void Parallel::Update(IBlackboard* blackboard, const float& elapsed)
    {
        if (state != AState::Running)
            return;

        for (Action* action : *actions)
            if (action && action->state == AState::Running)
                action->Update(blackboard, elapsed);
        CheckChildState(blackboard);
    }

    void Parallel::CheckChildState(IBlackboard* blackboard)
    {
        auto checkState = [this](AState state, bool passOnOne) {
            auto stateEqual = [state](const Action* action) -> bool { return action->state == state; };
            return passOnOne
                ? std::any_of(actions->begin(), actions->end(), stateEqual)
                : std::all_of(actions->begin(), actions->end(), stateEqual);
        };

        if (checkState(AState::Failure, failOnOne))
            state = AState::Failure;
        else if (checkState(AState::Success, succeedOnOne))
            state = AState::Success;
    }

    Action* Parallel::clone()
    {
        return new Parallel(*this);
    }

    void Decorator::Start(IBlackboard* blackboard)
    {
        if (!child)
        {
            state = AState::Failure;
            return;
        }

        child->Start(blackboard);
        CheckChildState(blackboard);
    }

    void Decorator::Stop(IBlackboard* blackboard)
    {
        if (child)
            child->Stop(blackboard);
        Action::Stop(blackboard);
    }

    void Decorator::Update(IBlackboard* blackboard, const float& elapsed)
    {
        if (child->state == AState::Running)
            child->Update(blackboard, elapsed);
        CheckChildState(blackboard);
    }

    Decorator::~Decorator()
    {
        if (child)
            delete child;
    }

    Action* Decorator::clone()
    {
        Decorator* out = new Decorator;
        if (child)
            out->child = child->clone();
        return out;
    }

    void Decorator::CheckChildState(IBlackboard*)
    {
        state = child->state;
    }

    void NotDecorator::CheckChildState(IBlackboard*)
    {
        if (child->state == AState::Failure)
            state = AState::Success;
        else if (child->state != AState::Idle && child->state != AState::Success)
            state = child->state;
        else
            state = AState::Failure;
    }

    Action* NotDecorator::clone()
    {
        NotDecorator* out = new NotDecorator;
        if (child)
            out->child = child->clone();
        return out;
    }

    void ForceState::CheckChildState(IBlackboard* blackboard)
    {
        if (child->state == AState::Running)
            state = child->state;
        else
            state = forceState;
    }

    std::string ForceState::DebugString()
    {
        switch (forceState)
        {
        case AState::Running:
            return "Running";
        case AState::Success:
            return "Success";
        case AState::Failure:
            return "Failure";
        default:
            return "";
        }
    }

    Action* ForceState::clone()
    {
        ForceState* out = new ForceState;
        if (child)
            out->child = child->clone();
        out->forceState = forceState;
        return out;
    }

    std::string RandomDecorator::DebugString()
    {
        std::ostringstream oss;
        oss << "P = " << pValue;
        return oss.str();
    }

    void RandomDecorator::Start(IBlackboard* blackboard)
    {
        if (pValue >= ((float)rand() / (float)RAND_MAX) && child)
        {
            child->Start(blackboard);
            CheckChildState(blackboard);
        }
        else
        {
            state = AState::Failure;
        }
    }

    Action* RandomDecorator::clone()
    {
        RandomDecorator* out = new RandomDecorator;
        if (child)
            out->child = child->clone();
        out->pValue = pValue;
        return out;
    }

    std::string Loop::DebugString()
    {
        std::string out;

        switch (loopCondition)
        {
        case LoopCondition::LoopOnSuccess:
            out += "Loop on success";
            break;
        case LoopCondition::LoopOnFailure:
            out += "Loop on failure";
            break;
        case LoopCondition::Looping:
            out += "Looping";
            break;
        default: break;
        }

        if (count > 0)
        {
            std::ostringstream oss;
            oss << "\\n" << count << " times";
            out += oss.str();
        }

        return out;
    }

    void Loop::CheckChildState(IBlackboard* blackboard)
    {
        if (child->state == AState::Idle)
        {
            state = AState::Failure;
            return;
        }

        bool loopOnFailure = loopCondition == LoopCondition::LoopOnFailure || loopCondition == LoopCondition::Looping;
        bool loopOnSuccess = loopCondition == LoopCondition::LoopOnSuccess || loopCondition == LoopCondition::Looping;

        state = child->state;
        if ((child->state == AState::Failure && !loopOnFailure) ||
            (child->state != AState::Success || !loopOnSuccess))
        {
            return;
        }

        if (count <= 0 || index++ < count)
        {
            child->Stop(blackboard);
            child->Start(blackboard);
            state = AState::Running;
        }
        else
        {
            state = AState::Success;
        }
    }

    Action* Loop::clone()
    {
        Loop* out = new Loop;
        if (child)
            out->child = child->clone();
        out->loopCondition = loopCondition;
        out->count = count;
        return out;
    }

    LoopOnCondition* LoopOnCondition::Create(boost::function1<bool, IBlackboard*> func, Action* childAction)
    {
        return new LoopOnCondition(func, childAction);
    }

    LoopOnCondition::LoopOnCondition(boost::function1<bool, IBlackboard*> func, Action* childAction) : func(func)
    {
        child = childAction;
    }

    void LoopOnCondition::CheckChildState(IBlackboard* blackboard)
    {
        if (child->state == AState::Idle)
        {
            state = AState::Failure;
        }
        else if ((child->state != AState::Success && state != AState::Failure) || !func(blackboard))
        {
            state = child->state;
        }
        else
        {
            child->Stop(blackboard);
            child->Start(blackboard);
            state = AState::Running;
        }
    }

    Action* LoopOnCondition::clone()
    {
        return new LoopOnCondition(*this);
    }

    LoopOnCondition::LoopOnCondition(const LoopOnCondition& other) : func(other.func)
    {
        if (other.child)
            child = other.child->clone();
    }

    void BHTreeDebugger::PrintTreeState(Action* action, ILoggingComponent* component, int tabs)
    {
        std::string pad;
        for (int i = 0; i < tabs; i++)
            pad += "    ";

        std::string classNameWithPad = pad + ClassName(action);
        std::string stateStr;
        if (action->state == AState::Running)
            stateStr = "Running";
        if (action->state == AState::Failure)
            stateStr = "Failure";
        if (action->state == AState::Success)
            stateStr = "Success";

        classNameWithPad = classNameWithPad.replace(0, stateStr.size(), stateStr.data(), stateStr.size());
        component->Log(classNameWithPad.data());

        if (Composite* composite = dynamic_cast<Composite*>(action))
        {
            component->Log((pad + "{").c_str());
            for (Action* childAction : *composite->actions)
                PrintTreeState(childAction, component, tabs + 1);
            component->Log((pad + "}").c_str());
        }
        else if (Decorator* decorator = dynamic_cast<Decorator*>(action))
        {
            component->Log((pad + "{").c_str());
            PrintTreeState(decorator->child, component, tabs + 1);
            component->Log((pad + "}").c_str());
        }
    }

    void BHTreeDebugger::GraphTreeState(Action* action, ILoggingComponent* component, bool useLastState)
    {
        component->Log("\nGenerating graph using dot notation. Plug me into http://webgraphviz.com/");
        component->Log("=======================================\n");
        component->Log("digraph g{");
        RecursiveGraph(action, component, useLastState);
        component->Log("}");
        component->Log("\n=======================================\n");
    }

    std::string BHTreeDebugger::ClassName(Action* action)
    {
        std::string out;

    #ifdef NEED_DEMANGLE
        int status = -4;
        char* demangled = abi::__cxa_demangle(typeid(action).name(), nullptr, nullptr, &status);
        out = demangled;
    #else
        out = typeid(action).name();
    #endif

        const std::string prefix = "BehaviourTree::";
        size_t prefixIndex = out.find(prefix);
        if (prefixIndex != std::string::npos)
            out.erase(prefixIndex, prefix.size());

    #ifdef NEED_DEMANGLE
        free(demangled);
    #endif

        return out;
    }

    void BHTreeDebugger::RecursiveGraph(Action* action, ILoggingComponent* component, bool useLastState, int index)
    {
        AState state = useLastState ? action->lastState : action->state;

        Composite* composite = dynamic_cast<Composite*>(action);
        Decorator* decorator = dynamic_cast<Decorator*>(action);
        Sequence* sequence = dynamic_cast<Sequence*>(action);
        Selector* selector = dynamic_cast<Selector*>(action);
        Parallel* parallel = dynamic_cast<Parallel*>(action);

        std::string label = ClassName(action);
        std::string debugString = action->DebugString();
        if (!debugString.empty())
            label += "\\n" + debugString;

        std::string shape = "box";
        if (sequence)
            shape = "triangle";
        else if (selector)
            shape = "invtriangle";
        else if (parallel)
            shape = "diamond";
        else if (decorator)
            shape = "invtrapezium";

        std::string fillColor = "aliceblue";
        if (sequence)
            fillColor = "aquamarine";
        else if (selector)
            fillColor = "deepskyblue";
        else if (parallel)
            fillColor = "cadetblue1";
        else if (decorator)
            fillColor = "bisque";

        std::string color = "black";
        if (state == AState::Running)
            color = "goldenrod1";
        else if (state == AState::Failure)
            color = "red";
        else if (state == AState::Success)
            color = "green";

        std::string style = "bold,solid";
        if (state == AState::Idle)
            style = "dotted";

        std::ostringstream entryStream;
        entryStream << "\t" << index
            << "["
            << "label=\"" << label << "\","
            << "shape=\"" << shape << ","
            << "fillcolor=" << fillColor << ","
            << "style=\"" << "filled,rounded," << style << "\""
            << "];";

        std::string entry = entryStream.str();
        component->Log(entry.c_str());

        if (index > 0)
        {
            std::ostringstream entryDataStream;
            entryDataStream << "\t" << index << " -> "
                << "["
                << "color=" << color << ","
                << "style=\"" << style << "\""
                << "];";

            std::string entryData = entryDataStream.str();
            component->Log(entryData.c_str());
        }

        if (composite)
        {
            for (Action* compositeAction : *composite->actions)
                RecursiveGraph(compositeAction, component, useLastState, ++index);
        }
        else if (decorator)
        {
            RecursiveGraph(decorator->child, component, useLastState, ++index);
        }
    }
}
