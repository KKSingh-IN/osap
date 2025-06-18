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

#include "VehicleControlApp.hpp"
#include <iostream>

VehicleControlApp::VehicleControlApp() : Application("VehicleControlApp")
{
    std::cout << appName_ << ": Specific application constructor called." << std::endl;

    // Subscribe to relevant signals
    subscribeToSignal("SpeedSignal", this, &VehicleControlApp::handleSpeedSignal);
    subscribeToSignal("BrakeRequestSignal", this, &VehicleControlApp::handleBrakeRequestSignal);
}

void VehicleControlApp::onInitialize()
{
    Application::onInitialize(); // Call base implementation
    std::cout << appName_ << ": Performing specific initialization tasks (e.g., loading config, checking sensors)." << std::endl;
    // If it fails: triggerApplicationEvent(AppEvent::INIT_FAIL);
    // For now, assume success.
}

void VehicleControlApp::onExecute()
{
    // This method is called repeatedly when the application is in the RUNNING state.
    // Example: Publish current brake pressure periodically
    static int counter = 0;
    if (++counter % 200 == 0)
    { // Publish every 200 ticks (approx 2 seconds with 10ms sleep)
        BrakePressureSignal currentPressure;
        currentPressure.current_pressure_psi = 100.0; // Dummy value
        publishSignal("BrakePressureSignal", currentPressure);
        std::cout << appName_ << ": Published BrakePressureSignal." << std::endl;
    }
}

void VehicleControlApp::onTerminate()
{
    Application::onTerminate(); // Call base implementation
    std::cout << appName_ << ": Performing specific cleanup tasks (e.g., saving state, closing connections)." << std::endl;
}

// Signal Handler Implementations
void VehicleControlApp::handleSpeedSignal(const SpeedSignal &signal)
{
    std::cout << appName_ << ": Received Speed: " << signal.speed_kmph
              << " kmph. Current App State: " << static_cast<int>(getCurrentApplicationState()) << std::endl;
}

void VehicleControlApp::handleBrakeRequestSignal(const BrakeRequestSignal &signal)
{
    std::cout << appName_ << ": Received Brake Request: Pedal Position=" << signal.brake_pedal_position << std::endl;
}