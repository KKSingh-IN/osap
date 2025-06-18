/*Copyright 2025 Kamlesh Singh
 *
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License.
 *You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing, software
 *distributed under the License is distributed on an "AS IS" BASIS,
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *See the License for the specific language governing permissions and
 *limitations under the License.
 */

#include <iostream>
#include "StateMachine.hpp"

template <typename StateEnum, typename EventEnum>
StateMachine<StateEnum, EventEnum>::StateMachine(StateEnum initialState)
    : currentStateEnum_(initialState) {}

template <typename StateEnum, typename EventEnum>
void StateMachine<StateEnum, EventEnum>::addState(StateEnum type, std::unique_ptr<IState<StateEnum, EventEnum>> state)
{
    states_[type] = std::move(state);
    // If this is the initial state being added, set current state pointer
    if (type == currentStateEnum_ && !currentState_)
    {
        currentState_ = states_[type].get();
    }
}

template <typename StateEnum, typename EventEnum>
void StateMachine<StateEnum, EventEnum>::transitionTo(StateEnum newStateEnum)
{
    if (currentState_)
    {
        // std::cout << "State Machine: Exiting " << static_cast<int>(currentStateEnum_) << std::endl;
        currentState_->exit(*this);
    }

    auto it = states_.find(newStateEnum);
    if (it != states_.end())
    {
        currentState_ = it->second.get();
        currentStateEnum_ = newStateEnum;
        // std::cout << "State Machine: Entering " << static_cast<int>(newStateEnum) << std::endl;
        currentState_->enter(*this);
    }
    else
    {
        std::cerr << "State Machine Error: Attempted to transition to unknown state: "
                  << static_cast<int>(newStateEnum) << std::endl;
    }
}

template <typename StateEnum, typename EventEnum>
void StateMachine<StateEnum, EventEnum>::handleEvent(EventEnum event)
{
    if (currentState_)
    {
        currentState_->handleEvent(*this, event);
    }
    else
    {
        // std::cerr << "State Machine Warning: Event " << static_cast<int>(event)
        //           << " received, but no current state is set." << std::endl;
    }
}

template <typename StateEnum, typename EventEnum>
StateEnum StateMachine<StateEnum, EventEnum>::getCurrentState() const
{
    return currentStateEnum_;
}

// Explicit instantiations for common usage
template class StateMachine<AppState, AppEvent>;