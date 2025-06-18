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
#include "CommonApplicationStates.hpp"
#include "StateMachine.hpp"

// State implementations use the concrete StateMachine to trigger transitions

void UninitializedState::enter(IStateMachine<AppState, AppEvent> &sm)
{
    std::cout << "App State: Entering UninitializedState" << std::endl;
}

void UninitializedState::exit(IStateMachine<AppState, AppEvent> &sm) {}

void UninitializedState::handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event)
{
    if (event == AppEvent::INIT_REQUEST)
    {
        sm.transitionTo(AppState::INITIALIZING);
    }
}

AppState UninitializedState::getType() const { return AppState::UNINITIALIZED; }

void InitializingState::enter(IStateMachine<AppState, AppEvent> &sm)
{
    std::cout << "App State: Entering InitializingState" << std::endl;
    /* Intialization tasks to be added */
    sm.handleEvent(AppEvent::INIT_SUCCESS);
}

void InitializingState::exit(IStateMachine<AppState, AppEvent> &sm) {}

void InitializingState::handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event)
{
    if (event == AppEvent::INIT_SUCCESS)
    {
        sm.transitionTo(AppState::READY);
    }
    else if (event == AppEvent::INIT_FAIL)
    {
        sm.transitionTo(AppState::ERROR);
    }
}

AppState InitializingState::getType() const { return AppState::INITIALIZING; }

void ReadyState::enter(IStateMachine<AppState, AppEvent> &sm)
{
    std::cout << "App State: Entering ReadyState" << std::endl;
}

void ReadyState::exit(IStateMachine<AppState, AppEvent> &sm) {}

void ReadyState::handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event)
{
    if (event == AppEvent::START_REQUEST)
    {
        sm.transitionTo(AppState::RUNNING);
    }
    else if (event == AppEvent::SHUTDOWN_REQUEST)
    {
        sm.transitionTo(AppState::TERMINATING);
    }
}

AppState ReadyState::getType() const { return AppState::READY; }

void RunningState::enter(IStateMachine<AppState, AppEvent> &sm)
{
    std::cout << "App State: Entering RunningState" << std::endl;
}

void RunningState::exit(IStateMachine<AppState, AppEvent> &sm) {}

void RunningState::handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event)
{
    if (event == AppEvent::PAUSE_REQUEST)
    {
        sm.transitionTo(AppState::PAUSED);
    }
    else if (event == AppEvent::STOP_REQUEST || event == AppEvent::SHUTDOWN_REQUEST)
    {
        sm.transitionTo(AppState::TERMINATING);
    }
    else if (event == AppEvent::ERROR_OCCURRED)
    {
        sm.transitionTo(AppState::ERROR);
    }
    else if (event == AppEvent::TICK)
    {
        /* Periodic task to be added */
    }
}

AppState RunningState::getType() const { return AppState::RUNNING; }

void PausedState::enter(IStateMachine<AppState, AppEvent> &sm)
{
    std::cout << "App State: Entering PausedState" << std::endl;
}

void PausedState::exit(IStateMachine<AppState, AppEvent> &sm) {}

void PausedState::handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event)
{
    if (event == AppEvent::RESUME_REQUEST)
    {
        sm.transitionTo(AppState::RUNNING);
    }
    else if (event == AppEvent::SHUTDOWN_REQUEST)
    {
        sm.transitionTo(AppState::TERMINATING);
    }
}
AppState PausedState::getType() const { return AppState::PAUSED; }

void ErrorState::enter(IStateMachine<AppState, AppEvent> &sm)
{
    std::cerr << "App State: Entering ErrorState! Application may be unstable." << std::endl;
}

void ErrorState::exit(IStateMachine<AppState, AppEvent> &sm) {}

void ErrorState::handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event)
{
    if (event == AppEvent::SHUTDOWN_REQUEST)
    {
        sm.transitionTo(AppState::TERMINATING);
    }
    // Could also allow a RECOVERY_REQUEST to transition back to READY or INITIALIZING
}

AppState ErrorState::getType() const { return AppState::ERROR; }

void TerminatingState::enter(IStateMachine<AppState, AppEvent> &sm)
{
    std::cout << "App State: Entering TerminatingState" << std::endl;
    /* Perform final cleanup steps */
    sm.transitionTo(AppState::TERMINATED);
}

void TerminatingState::exit(IStateMachine<AppState, AppEvent> &sm) {}

void TerminatingState::handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event)
{
    // No transitions out of terminating state based on events, only progress to TERMINATED
}

AppState TerminatingState::getType() const { return AppState::TERMINATING; }

void TerminatedState::enter(IStateMachine<AppState, AppEvent> &sm)
{
    std::cout << "App State: Entering TerminatedState. Application is now fully shut down." << std::endl;
}

void TerminatedState::exit(IStateMachine<AppState, AppEvent> &sm) {}

void TerminatedState::handleEvent(IStateMachine<AppState, AppEvent> &sm, AppEvent event)
{
    // No further events should be handled or transitions from TERMINATED
}

AppState TerminatedState::getType() const { return AppState::TERMINATED; }