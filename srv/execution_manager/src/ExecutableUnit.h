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

#ifndef EXECUTABLE_UNIT_H
#define EXECUTABLE_UNIT_H

#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include <spawn.h>

class ExecutableUnit
{
public:
    // Enum to represent the state of an ExecutableUnit
    enum class State
    {
        READY,
        RUNNING,
        EXITED,
        FAILED_TO_SPAWN
    };

    /**
     * @brief Constructor for the ExecutableUnit class.
     * @param name The name of the ExecutableUnit.
     * @param path The full path to the executable.
     * @param args A vector of string arguments for the executable.
     */
    ExecutableUnit(const std::string &name, const std::string &path, const std::vector<std::string> &args);

    /**
     * @brief Spawns the ExecutableUnit.
     */
    void span();

    /**
     * @brief Updates the current status of the running ExecutableUnit.
     * Checks if the process has exited and updates its state and exit code.
     */
    void update_status();

    // Getters for ExecutableUnit runtime information
    std::string get_name() const;
    State get_state() const;
    pid_t get_pid() const;
    int get_exit_code() const;

private:
    std::string name_;
    std::string path_;
    std::vector<std::string> args_;
    State state_;
    pid_t pid_;
    int exit_code_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};

#endif // EXECUTABLE_UNIT_H