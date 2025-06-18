/* Copyright 2025 Kamlesh Singh
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

#include "Application.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>

// Global atomic flag definition for shutdown
std::atomic<bool> g_shutdown_requested(false);

// OS signal handler function implementation
extern "C" void osSignalHandler(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        std::cout << "\nOS Signal Handler: Caught signal " << signal << ". Requesting application shutdown." << std::endl;
        g_shutdown_requested.store(true);
    }
}

template <typename ConcreteApp>
Application<ConcreteApp>::Application(const std::string &appName)
    : stateMachine_(AppState::UNINITIALIZED),
      signalBus_(SignalBus::getInstance()), // Ensure SignalBus singleton is initialized
      appName_(appName)
{
    std::cout << appName_ << ": Application constructor." << std::endl;
    addCommonStates(); // Add the predefined common states
}

template <typename ConcreteApp>
Application<ConcreteApp>::~Application()
{
    std::cout << appName_ << ": Application destructor." << std::endl;
}

template <typename ConcreteApp>
bool Application<ConcreteApp>::initialize()
{
    std::cout << appName_ << ": Initializing..." << std::endl;
    // Register OS signal handler for graceful shutdown
    std::signal(SIGINT, osSignalHandler);
    std::signal(SIGTERM, osSignalHandler);
    // Ignore SIGPIPE to prevent crashes on broken pipes (common in IPC)
    std::signal(SIGPIPE, SIG_IGN);

    stateMachine_.handleEvent(AppEvent::INIT_REQUEST); // Trigger state transition
    onInitialize();                                    // Hook for derived class-specific initialization

    // After onInitialize, derived class should trigger INIT_SUCCESS/FAIL
    // If not, we might consider it successful by default if no error occurred
    if (stateMachine_.getCurrentState() == AppState::INITIALIZING)
    {
        stateMachine_.handleEvent(AppEvent::INIT_SUCCESS); // Auto-succeed if not explicitly failed
    }

    if (stateMachine_.getCurrentState() == AppState::READY)
    {
        std::cout << appName_ << ": Initialization successful." << std::endl;
        stateMachine_.handleEvent(AppEvent::START_REQUEST); // Automatically request to start running
        return true;
    }
    else
    {
        std::cerr << appName_ << ": Initialization failed. Current state: "
                  << static_cast<int>(stateMachine_.getCurrentState()) << std::endl;
        return false;
    }
}

template <typename ConcreteApp>
void Application<ConcreteApp>::run()
{
    std::cout << appName_ << ": Entering main run loop." << std::endl;
    while (!g_shutdown_requested.load() && stateMachine_.getCurrentState() != AppState::TERMINATED)
    {
        if (stateMachine_.getCurrentState() == AppState::RUNNING)
        {
            onExecute(); // Hook for derived class's main logic
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Adjust tick rate as needed
        // stateMachine_.handleEvent(AppEvent::TICK);
    }
    std::cout << appName_ << ": Exiting main run loop." << std::endl;
}

template <typename ConcreteApp>
void Application<ConcreteApp>::terminate()
{
    std::cout << appName_ << ": Terminating..." << std::endl;
    stateMachine_.handleEvent(AppEvent::SHUTDOWN_REQUEST); // Trigger state transition
    onTerminate();                                         // Hook for derived class-specific cleanup

    // Ensure the state machine reaches TERMINATED
    int attempts = 0;
    while (stateMachine_.getCurrentState() != AppState::TERMINATED && attempts < 20)
    { // Increased attempts
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        attempts++;
        stateMachine_.handleEvent(AppEvent::TICK); // Give states a chance to process termination
    }

    std::cout << appName_ << ": Termination complete. Final state: "
              << static_cast<int>(stateMachine_.getCurrentState()) << std::endl;
}

template <typename ConcreteApp>
AppState Application<ConcreteApp>::getCurrentApplicationState() const
{
    return stateMachine_.getCurrentState();
}

template <typename ConcreteApp>
void Application<ConcreteApp>::triggerApplicationEvent(AppEvent event)
{
    stateMachine_.handleEvent(event);
}

template <typename ConcreteApp>
void Application<ConcreteApp>::onInitialize()
{
    std::cout << appName_ << ": Common initialization hook." << std::endl;
}

template <typename ConcreteApp>
void Application<ConcreteApp>::onTerminate()
{
    std::cout << appName_ << ": Common termination hook." << std::endl;
}

template <typename ConcreteApp>
void Application<ConcreteApp>::addCommonStates()
{
    // Add the standard set of application lifecycle states
    stateMachine_.addState(AppState::UNINITIALIZED, std::make_unique<UninitializedState>());
    stateMachine_.addState(AppState::INITIALIZING, std::make_unique<InitializingState>());
    stateMachine_.addState(AppState::READY, std::make_unique<ReadyState>());
    stateMachine_.addState(AppState::RUNNING, std::make_unique<RunningState>());
    stateMachine_.addState(AppState::PAUSED, std::make_unique<PausedState>());
    stateMachine_.addState(AppState::ERROR, std::make_unique<ErrorState>());
    stateMachine_.addState(AppState::TERMINATING, std::make_unique<TerminatingState>());
    stateMachine_.addState(AppState::TERMINATED, std::make_unique<TerminatedState>());
}

// Explicit instantiation of the Application template for common usage
#include "../vehicle-control/VehicleControlApp.hpp" // Include ConcreteApp header for instantiation
template class Application<MyVehicleControlApp>;