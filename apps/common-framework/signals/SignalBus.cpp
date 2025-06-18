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

#include "SignalBus.hpp"
#include <iostream>

// --- CONCEPTUAL IPC LAYER IMPLEMENTATION ---
IpcBridge &IpcBridge::getInstance()
{
    static IpcBridge instance;
    return instance;
}

IpcBridge::IpcBridge() = default;
IpcBridge::~IpcBridge() = default;

void IpcBridge::registerIpcReceiver(IpcSignalReceiver receiver)
{
    ipcReceiver_ = receiver;
    std::cout << "[IPC Bridge] Receiver registered." << std::endl;
}

void IpcBridge::sendSignal(const std::string &signalName, const ISignal &signalData)
{
    // In a real system, this would involve serialization and actual IPC send.
    // For demo, we just print.
    std::cout << "[IPC Bridge] Sending signal: " << signalName << std::endl;
}

void IpcBridge::receiveAndDispatch(const std::string &signalName, const ISignal &signalData)
{
    if (ipcReceiver_)
    {
        std::cout << "[IPC Bridge] Received and dispatching signal: " << signalName << std::endl;
        ipcReceiver_(signalName, signalData);
    }
}
// --- END CONCEPTUAL IPC LAYER IMPLEMENTATION ---

// --- SIGNAL BUS IMPLEMENTATION ---
SignalBus &SignalBus::getInstance()
{
    static SignalBus instance;
    return instance;
}

SignalBus::SignalBus()
{
    IpcBridge::getInstance().registerIpcReceiver(
        [this](const std::string &signalName, const ISignal &signalData)
        {
            this->dispatchInternal(signalName, signalData);
        });
}

SignalBus::~SignalBus() = default;

void SignalBus::dispatchInternal(const std::string &signalName, const ISignal &signalData)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (subscribers_.count(signalName))
    {
        // std::cout << "SignalBus: Dispatching internal signal '" << signalName << "'" << std::endl;
        for (const auto &handler : subscribers_[signalName])
        {
            handler(signalData);
        }
    }
}