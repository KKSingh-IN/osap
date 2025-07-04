# Copyright 2024 Kamlesh Singh
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.15)
project(vehicle_app VERSION 1.0)

add_executable(vehicle_app
    src/VehicleControlApp.cpp
    src/main.cpp
)

target_include_directories(vehicle_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} # Application/
    ${CMAKE_SOURCE_DIR}/common-framework          # common-framework/
    ${CMAKE_SOURCE_DIR}/common-framework/signals  # common-framework/signals/
    ${CMAKE_SOURCE_DIR}/common-framework/state    # common-framework/state/
)

target_link_libraries(vehicle_app common-framework)

set_target_properties(vehicle_app PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)

# install(TARGETS vehicle_app DESTINATION bin)