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

#ifndef TASK_DISPATCHER_H
#define TASK_DISPATCHER_H

#include "ApplicationRegistry.h"
#include "ExecutableUnit.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory> // For std::shared_ptr

class TaskDispatcher
{
public:
    /**
     * @brief Constructor for the TaskDispatcher.
     * @param pool A shared pointer to the ApplicationRegistry.
     * @param num_threads The number of worker threads to create for loading applications.
     */
    TaskDispatcher(std::shared_ptr<ApplicationRegistry> pool, size_t num_threads);

    /**
     * @brief Destructor. Ensures all worker threads are joined.
     */
    ~TaskDispatcher();

    /**
     * @brief Queues an application to be loaded by a worker thread.
     * @param app A shared pointer to the Application object to load.
     */
    void load_application(std::shared_ptr<ExecutableUnit> app);

private:
    std::shared_ptr<ApplicationRegistry> pool_;
    std::vector<std::thread> worker_threads_;
    std::queue<std::shared_ptr<ExecutableUnit>> task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_; // Flag to signal threads to stop

    /**
     * @brief The main function executed by each worker thread.
     * Picks up applications from the queue and launches them.
     */
    void worker_function();
};

#endif // TASK_DISPATCHER_H