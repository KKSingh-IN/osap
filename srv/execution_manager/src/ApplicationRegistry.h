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

#ifndef APPLICATION_REGISTRY_H
#define APPLICATION_REGISTRY_H

#include <vector>
#include <memory>
#include <mutex>

#include "ExecutableUnit.h"

class ApplicationRegistry
{
public:
    /**
     * @brief Adds an executable to the registry.
     * @param app A shared pointer to the executable object.
     */
    void add_application(std::shared_ptr<ExecutableUnit> app);

    /**
     * @brief Retrieves a copy of all executable currently in the resgistry.
     * This method returns a copy to ensure thread safety when iterating.
     * @return A vector of shared pointers to Application objects.
     */
    std::vector<std::shared_ptr<ExecutableUnit>> get_all_applications();

private:
    std::vector<std::shared_ptr<ExecutableUnit>> applications_;
    mutable std::mutex applications_mutex_;
};

#endif // APPLICATION_REGISTRY_H