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

#include "ApplicationRegistry.h"

void ApplicationRegistry::add_application(std::shared_ptr<ExecutableUnit> app)
{
    std::lock_guard<std::mutex> lock(applications_mutex_);
    applications_.push_back(app);
}

std::vector<std::shared_ptr<ExecutableUnit>> ApplicationRegistry::get_all_applications()
{
    std::lock_guard<std::mutex> lock(applications_mutex_);
    return applications_;
}