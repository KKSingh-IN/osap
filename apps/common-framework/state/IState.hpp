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


#ifndef COMMON_FRAMEWORK_ISTATE_HPP
#define COMMON_FRAMEWORK_ISTATE_HPP

#include <memory>

// Forward declarations
template<typename StateEnum, typename EventEnum>
class IStateMachine;

// Base class for all state implementations
template<typename StateEnum, typename EventEnum>
class IState {
public:
    virtual ~IState() = default;
    virtual void enter(IStateMachine<StateEnum, EventEnum>& sm) = 0;
    virtual void exit(IStateMachine<StateEnum, EventEnum>& sm) = 0;
    virtual void handleEvent(IStateMachine<StateEnum, EventEnum>& sm, EventEnum event) = 0;
    virtual StateEnum getType() const = 0;
};

#endif // COMMON_FRAMEWORK_ISTATE_HPP