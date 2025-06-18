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

#ifndef COMMON_FRAMEWORK_SIGNAL_BUS_HPP
#define COMMON_FRAMEWORK_SIGNAL_BUS_HPP

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <iostream>

#include "ISignal.hpp"

// Forward declare IpcBridge

/*---CONCEPTUAL IPC LAYER (Will replace by real IPC)---*/
class IpcBridge
{
public:
    // A simplified callback type for when a signal is received from IPC
    using IpcSignalReceiver = std::function<void(const std::string &signalName, const ISignal &signalData)>;

    static IpcBridge &getInstance(); // Singleton access

    void registerIpcReceiver(IpcSignalReceiver receiver);
    void sendSignal(const std::string &signalName, const ISignal &signalData);

    // This simulates receiving a signal from IPC and dispatching it to the registered receiver.
    // This method would be called by IPC's event loop when a message arrives.
    void receiveAndDispatch(const std::string &signalName, const ISignal &signalData);

private:
    IpcBridge(); // Private constructor for Singleton
    ~IpcBridge();
    IpcBridge(const IpcBridge &) = delete;
    IpcBridge &operator=(const IpcBridge &) = delete;

    IpcSignalReceiver ipcReceiver_;
};
/*--- END CONCEPTUAL IPC LAYER ---*/

/*--- The application's in-process SignalBus, which uses the IPC Bridge ---*/
class SignalBus
{
public:
    static SignalBus &getInstance(); // Singleton access

    // Subscribe to a signal type within this process
    template <typename T, typename Class>
    void subscribe(const std::string &signalName, Class *instance, void (Class::*method)(const T &))
    {
        std::lock_guard<std::mutex> lock(mutex_);
        subscribers_[signalName].push_back(
            [instance, method, signalName](const ISignal &signal)
            {
                // Dynamic cast to the concrete signal type (safe due to ISignal inheritance)
                if (const T *concreteSignal = dynamic_cast<const T *>(&signal))
                {
                    (instance->*method)(*concreteSignal);
                }
                else
                {
                    std::cerr << "SignalBus Error: Type mismatch for signal " << signalName << std::endl;
                }
            });
        std::cout << "SignalBus: Subscribed to '" << signalName << "'" << std::endl;
    }

    // Publish a signal (internally and via IPC)
    template <typename T>
    void publish(const std::string &signalName, const T &signalData)
    {
        // Send via IPC for other processes
        IpcBridge::getInstance().sendSignal(signalName, signalData);

        // Also dispatch internally for subscribers within this process
        dispatchInternal(signalName, signalData);
    }

private:
    SignalBus(); // Private constructor for Singleton
    ~SignalBus();
    SignalBus(const SignalBus &) = delete;
    SignalBus &operator=(const SignalBus &) = delete;

    // Internal dispatching logic
    void dispatchInternal(const std::string &signalName, const ISignal &signalData);

    std::map<std::string, std::vector<std::function<void(const ISignal &)>>> subscribers_;
    std::mutex mutex_; // For thread-safe access to subscribers_
};

#endif // COMMON_FRAMEWORK_SIGNAL_BUS_HPP