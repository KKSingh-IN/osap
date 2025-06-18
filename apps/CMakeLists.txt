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

project(ApplicationSoftware CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable warnings for better code quality
add_compile_options(-Wall -Wextra -Wpedantic)

# Ensure position-independent code for shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add subdirectories for the Common Framework and the Application
add_subdirectory(common-framework)
add_subdirectory(vehicle-control)

# --- Runtime Configuration for Shared Libraries
# set_target_properties(vehicle_app PROPERTIES
#     BUILD_RPATH "$ORIGIN/../common-framework"
# )