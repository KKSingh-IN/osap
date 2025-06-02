/*Copyright 2024 Kamlesh Singh
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
#include <unistd.h>

#include "vehicle_monitor.h"


int main(int argc, char *argv[]) {

    /*Set main thread name for debugging purpose*/
    pthread_setname_np(pthread_self(), "vehicle_monitor_main");

    vehicle_monitor::VehicleMonitor::SignalHandler();

    vehicle_monitor::MultiProtocolReader app;
    app.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    app.stop();

    vehicle_monitor::VehicleMonitor::thread1_.join();

    return 0;
}
