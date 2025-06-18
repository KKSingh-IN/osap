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

#ifndef COMMON_FRAMEWORK_APPLICATION_HPP
#define COMMON_FRAMEWORK_APPLICATION_HPP

#include "state/ApplicationState.hpp"
#include "state/StateMachine.hpp"
#include "state/CommonApplicationStates.hpp"
#include "signals/SignalBus.hpp"
#include "signals/ISignal.hpp"

#include <string>
#include <atomic>

// Global atomic flag for shutdown, accessed by the signal handler
extern std::atomic<bool> g_shutdown_requested;

// OS signal handler function
extern "C" void osSignalHandler(int signal);

template <typename ConcreteApp> // Using CRTP (Curiously Recurring Template Pattern) for static_cast safety
class Application
{
protected:
    StateMachine<AppState, AppEvent> stateMachine_;
    SignalBus &signalBus_;
    std::string appName_;

public:
    explicit Application(const std::string &appName);
    virtual ~Application();

    // Public Lifecycle Methods (Called by main)
    bool initialize();
    void run();
    void terminate();

    // State Machine Access for Derived Classes
    AppState getCurrentApplicationState() const;
    void triggerApplicationEvent(AppEvent event);

    // Signal Bus Access for Derived Classes
    template <typename T, typename Class>
    void subscribeToSignal(const std::string &signalName, Class *instance, void (Class::*method)(const T &))
    {
        // static_cast ensures that 'this' (the ConcreteApp instance) is correctly passed.
        signalBus_.subscribe(signalName, static_cast<ConcreteApp *>(instance), method);
    }

    template <typename T>
    void publishSignal(const std::string &signalName, const T &signalData)
    {
        signalBus_.publish(signalName, signalData);
    }

protected:
    // Virtual Hooks for Derived Application Logic
    virtual void onInitialize();
    virtual void onExecute() = 0; // Pure virtual: MUST be implemented by derived applications
    virtual void onTerminate();

    // Allows derived classes to add or override state definitions
    virtual void addCommonStates();
};

#endif // COMMON_FRAMEWORK_APPLICATION_HPP