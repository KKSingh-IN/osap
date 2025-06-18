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

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include "VehicleControlApp.hpp"
#include "../../common-framework/signals/SignalBus.hpp" // For IpcBridge access through SignalBus
#include "CustomSignals.hpp"                         // For testing publishing signals from main

int main(int argc, char *argv[])
{
    std::cout << "--- VehicleControlApp Executable Started ---" << std::endl;

    VehicleControlApp app;

    // simulating publish in the same process for demonstration.
    std::thread simulated_publisher_thread([]()
                                           {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for app to initialize
        for (int i = 0; i < 5; ++i) {
            SpeedSignal speed;
            speed.speed_kmph = 50 + i * 10;
            std::cout << "[SIMULATED IPC]: Sending SpeedSignal: " << speed.speed_kmph << " kmph" << std::endl;
            // Directly use IpcBridge to simulate an external process sending a signal
            IpcBridge::getInstance().receiveAndDispatch("SpeedSignal", speed);
            std::this_thread::sleep_for(std::chrono::seconds(3));

            BrakeRequestSignal brakeReq;
            brakeReq.brake_pedal_position = (double)i / 5.0;
            std::cout << "[SIMULATED IPC]: Sending BrakeRequestSignal: " << brakeReq.brake_pedal_position << std::endl;
            IpcBridge::getInstance().receiveAndDispatch("BrakeRequestSignal", brakeReq);
            std::this_thread::sleep_for(std::chrono::seconds(3));
        } });

    // 1. Initialize the application
    if (!app.initialize())
    {
        std::cerr << "Application initialization failed. Exiting." << std::endl;
        if (simulated_publisher_thread.joinable())
        {
            simulated_publisher_thread.join();
        }
        return 1;
    }

    // 2. Run the application's main loop
    app.run();

    // 3. Terminate the application
    app.terminate();

    if (simulated_publisher_thread.joinable())
    {
        simulated_publisher_thread.join();
    }

    std::cout << "--- VehicleControlApp Executable Finished ---" << std::endl;
    return 0;
}