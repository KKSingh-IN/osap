/*Copyright 2025 Kamlesh Singh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef COMMON_FRAMEWORK_COMMON_APPLICATION_STATES_HPP
#define COMMON_FRAMEWORK_COMMON_APPLICATION_STATES_HPP

#include "IState.hpp"
#include "ApplicationState.hpp"

// Forward declare the concrete StateMachine for enter/exit/handleEvent methods
template <typename StateEnum, typename EventEnum>
class StateMachine;

// Base State definitions (declarations)
class UninitializedState : public IState<AppState, AppEvent>
{
public:
    void enter(IStateMachine<AppState, AppEvent> &sm) override;
    void exit(IStateMachine<AppState, AppEvent> &sm) override;
    void handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event) override;
    AppState getType() const override;
};

class InitializingState : public IState<AppState, AppEvent>
{
public:
    void enter(IStateMachine<AppState, AppEvent> &sm) override;
    void exit(IStateMachine<AppState, AppEvent> &sm) override;
    void handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event) override;
    AppState getType() const override;
};

class ReadyState : public IState<AppState, AppEvent>
{
public:
    void enter(IStateMachine<AppState, AppEvent> &sm) override;
    void exit(IStateMachine<AppState, AppEvent> &sm) override;
    void handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event) override;
    AppState getType() const override;
};

class RunningState : public IState<AppState, AppEvent>
{
public:
    void enter(IStateMachine<AppState, AppEvent> &sm) override;
    void exit(IStateMachine<AppState, AppEvent> &sm) override;
    void handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event) override;
    AppState getType() const override;
};

class PausedState : public IState<AppState, AppEvent>
{
public:
    void enter(IStateMachine<AppState, AppEvent> &sm) override;
    void exit(IStateMachine<AppState, AppEvent> &sm) override;
    void handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event) override;
    AppState getType() const override;
};

class ErrorState : public IState<AppState, AppEvent>
{
public:
    void enter(IStateMachine<AppState, AppEvent> &sm) override;
    void exit(IStateMachine<AppState, AppEvent> &sm) override;
    void handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event) override;
    AppState getType() const override;
};

class TerminatingState : public IState<AppState, AppEvent>
{
public:
    void enter(IStateMachine<AppState, AppEvent> &sm) override;
    void exit(IStateMachine<AppState, AppEvent> &sm) override;
    void handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event) override;
    AppState getType() const override;
};

class TerminatedState : public IState<AppState, AppEvent>
{
public:
    void enter(IStateMachine<AppState, AppEvent> &sm) override;
    void exit(IStateMachine<AppState, AppEvent> &sm) override;
    void handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event) override;
    AppState getType() const override;
};

#endif // COMMON_FRAMEWORK_COMMON_APPLICATION_STATES_HPP