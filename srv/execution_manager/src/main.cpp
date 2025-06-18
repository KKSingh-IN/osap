/*
 * Copyright 2025 Kamlesh Singh
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
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <thread>

#include "ExecutableUnit.h"
#include "ApplicationRegistry.h"
#include "TaskDispatcher.h"

// extern char **environ;

int main()
{
    // Create an ApplicationRegistry instance
    auto app_pool = std::make_shared<ApplicationRegistry>();

    // Create an TaskDispatcher with a shared pool and desired number of threads
    TaskDispatcher loader(app_pool, 2); // Using 2 worker threads

    // Create Application objects and queue them for loading
    auto app1 = std::make_shared<ExecutableUnit>("main1", "main1", std::vector<std::string>{"App1"});
    auto app2 = std::make_shared<ExecutableUnit>("main2", "main2", std::vector<std::string>{"App2"});
    auto app3 = std::make_shared<ExecutableUnit>("main3", "main3", std::vector<std::string>{"App3"});
    auto app4 = std::make_shared<ExecutableUnit>("main4", "main4", std::vector<std::string>{"App4"});

    loader.load_application(app1);
    loader.load_application(app2);
    loader.load_application(app3);
    loader.load_application(app4);

    // Main loop to monitor applications
    std::cout << "\nStarting application monitoring loop..." << std::endl;
    uint64_t i = 0;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "\n--- Monitoring Applications (Tick: " << i + 1 << ") ---" << std::endl;
        for (auto &app : app_pool->get_all_applications())
        {
            app->update_status(); // Update the state of each application
            std::cout << "  App: " << app->get_name()
                      << ", PID: " << app->get_pid()
                      << ", State: ";
            switch (app->get_state())
            {
            case ExecutableUnit::State::READY:
                std::cout << "READY";
                break;
            case ExecutableUnit::State::RUNNING:
                std::cout << "RUNNING";
                break;
            case ExecutableUnit::State::EXITED:
                std::cout << "EXITED (" << app->get_exit_code() << ")";
                break;
            case ExecutableUnit::State::FAILED_TO_SPAWN:
                std::cout << "FAILED_TO_SPAWN";
                break;
            }
            std::cout << std::endl;
        }
        std::cout << "-----------------------------------" << std::endl;
        i += 1;
    }

    std::cout << "\nApplication manager finished monitoring." << std::endl;

    return 0;
}