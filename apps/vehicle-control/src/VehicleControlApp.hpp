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


#ifndef VEHICLE_CONTROL_APP_HPP
#define VEHICLE_CONTROL_APP_HPP

#include "../../common-framework/Application.hpp" // Include the common framework base
#include "CustomSignals.hpp"                      // application-specific signals

class VehicleControlApp : public Application<VehicleControlApp>
{
public:
    VehicleControlApp();

protected:
    // Override lifecycle hooks from Application base
    void onInitialize() override;
    void onExecute() override; // Pure virtual in base, must implement
    void onTerminate() override;

    // Signal Handler Implementations
    void handleSpeedSignal(const SpeedSignal &signal);
    void handleBrakeRequestSignal(const BrakeRequestSignal &signal);
};

#endif // VEHICLE_CONTROL_APP_HPP