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
#include "TaskDispatcher.h"

TaskDispatcher::TaskDispatcher(std::shared_ptr<ApplicationRegistry> pool, size_t num_threads)
    : pool_(pool), stop_(false)
{
    for (size_t i = 0; i < num_threads; ++i)
    {
        worker_threads_.emplace_back(&TaskDispatcher::worker_function, this);
    }
}

TaskDispatcher::~TaskDispatcher()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true; // Set stop flag
    }
    condition_.notify_all(); // Wake up all waiting threads
    for (std::thread &thread : worker_threads_)
    {
        if (thread.joinable())
        {
            thread.join(); // Wait for each thread to finish
        }
    }
}

void TaskDispatcher::load_application(std::shared_ptr<ExecutableUnit> app)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        task_queue_.push(app); // Add application to the queue
    }
    condition_.notify_one(); // Notify one worker thread that a new task is available
}

void TaskDispatcher::worker_function() {
    while (true) {
        std::shared_ptr<ExecutableUnit> app_to_load;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            condition_.wait(lock, [this] {
                return stop_ || !task_queue_.empty(); // Wait until there's a task or stop is true
            });

            if (stop_ && task_queue_.empty()) {
                return; // Exit thread if stop is true and queue is empty
            }
            app_to_load = task_queue_.front();
            task_queue_.pop();
        }

        // Launch the ExecutableUnit outside the lock to avoid holding it too long
        if (app_to_load) {
            app_to_load->span();
            // Add the ExecutableUnit to the pool. This is done here assuming 'span'
            // is where the ExecutableUnit transitions to a state where it's tracked.
            pool_->add_application(app_to_load);
        }
    }
}