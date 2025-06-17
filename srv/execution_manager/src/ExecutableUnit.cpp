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
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#include "ExecutableUnit.h"

// extern char **environ;

ExecutableUnit::ExecutableUnit(const std::string &name, const std::string &path, const std::vector<std::string> &args)
    : name_(name), path_(path), args_(args), state_(State::READY), pid_(-1), exit_code_(-1) {}

void ExecutableUnit::span()
{
    // Convert args_ to char* const* for posix_spawn
    std::vector<char *> c_args;
    c_args.push_back(const_cast<char *>(path_.c_str())); // argv[0] should be the program name
    for (const auto &arg : args_)
    {
        c_args.push_back(const_cast<char *>(arg.c_str()));
    }
    c_args.push_back(nullptr); // Null-terminate the argument list

    // posix_spawn_file_actions_t and posix_spawnattr_t can be used for advanced control.
    // For this basic example, we pass nullptr for default behavior.
    int status = posix_spawn(&pid_, path_.c_str(), nullptr, nullptr, c_args.data(), environ);

    if (status == 0)
    {
        state_ = State::RUNNING;
        start_time_ = std::chrono::high_resolution_clock::now();
        std::cout << "ExecutableUnit '" << name_ << "' spawned with PID: " << pid_ << std::endl;
    }
    else
    {
        state_ = State::FAILED_TO_SPAWN;
        std::cerr << "Failed to spawn ExecutableUnit '" << name_ << "': " << strerror(status) << std::endl;
        pid_ = -1;
    }
}

void ExecutableUnit::update_status()
{
    if (state_ == State::RUNNING)
    {
        int status;
        pid_t result = waitpid(pid_, &status, WNOHANG); // Check status without blocking

        if (result == pid_)
        { // Child process status is available
            if (WIFEXITED(status))
            {
                exit_code_ = WEXITSTATUS(status);
                state_ = State::EXITED;
                std::cout << "ExecutableUnit '" << name_ << "' (PID: " << pid_ << ") exited with code: " << exit_code_ << std::endl;
            }
            else if (WIFSIGNALED(status))
            {
                exit_code_ = WTERMSIG(status);
                state_ = State::EXITED; // ExecutableUnit terminated by a signal
                std::cout << "ExecutableUnit '" << name_ << "' (PID: " << pid_ << ") terminated by signal: " << exit_code_ << std::endl;
            }
        }
        else if (result == -1)
        { // Error occurred
            std::cerr << "Error checking status for ExecutableUnit '" << name_ << "' (PID: " << pid_ << "): " << strerror(errno) << std::endl;
            state_ = State::FAILED_TO_SPAWN; // Mark as failed due to check error
        }
        // If result is 0, the child is still running, no state change.
    }
}

std::string ExecutableUnit::get_name() const { return name_; }
ExecutableUnit::State ExecutableUnit::get_state() const { return state_; }
pid_t ExecutableUnit::get_pid() const { return pid_; }
int ExecutableUnit::get_exit_code() const { return exit_code_; }