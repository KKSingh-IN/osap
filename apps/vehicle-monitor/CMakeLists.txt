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
project(VehicleMonitor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cppcheck Integration
find_program(CPPCHECK_EXECUTABLE cppcheck)

# Cppcheck Integration

# Add an option to enable/disable Cppcheck
option(ENABLE_CPPCHECK "Enable Cppcheck static analysis during compilation" ON)

if(ENABLE_CPPCHECK)
    message(STATUS "Cppcheck static analysis is ENABLED.")

    # Find the Cppcheck executable
    # 'REQUIRED' enables CMake to throw error out if cppcheck is not found.
    find_program(CPPCHECK_EXECUTABLE NAMES cppcheck REQUIRED
        DOC "Path to the Cppcheck executable."
    )

    if(CPPCHECK_EXECUTABLE)
        message(STATUS "Cppcheck executable found at: ${CPPCHECK_EXECUTABLE}")

        # Define common Cppcheck options
        # These options will be applied to both C and C++ source files.
        # --enable=all: Enables all checks
        # --inconclusive: Report inconclusive checks
        # --inline-suppr: Allows in-line suppressions
        # --quiet: Suppress unnecessary verbose output from Cppcheck itself.
        # --suppressions-list: Path to a file containing rules for suppressing warnings.
        set(CPPCHECK_COMMON_OPTS
            "--enable=all"
            "--inconclusive"
            "--inline-suppr"
            "--quiet"
            # Ensure the suppressions file path is correct relative to the project source directory
            "--suppressions-list=${CMAKE_SOURCE_DIR}/cppcheck_suppressions.txt"
        )

        # Set Cppcheck for C source files
        # --std=c11: Specify the C standard
        set(CMAKE_C_CPPCHECK "${CPPCHECK_EXECUTABLE}" "--std=c11" ${CPPCHECK_COMMON_OPTS})
        # Set Cppcheck for C++ source files
        # --std=c++20: Specify the C++ standard.
        # Add include directories using `CMAKE_CXX_STANDARD_INCLUDE_DIRS` or manual `-I` flags
        set(CMAKE_CXX_CPPCHECK "${CPPCHECK_EXECUTABLE}" "--std=c++20" ${CPPCHECK_COMMON_OPTS})

        # set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} -I"${CMAKE_SOURCE_DIR}/include" -I"${CMAKE_SOURCE_DIR}/src")
        # set(CMAKE_C_CPPCHECK ${CMAKE_C_CPPCHECK} -I"${CMAKE_SOURCE_DIR}/include" -I"${CMAKE_SOURCE_DIR}/src")

    else()
        message(WARNING "Cppcheck executable not found. Cppcheck analysis will not be performed.")
    endif()
else()
    message(STATUS "Cppcheck static analysis is DISABLED. To enable, set -DENABLE_CPPCHECK=ON.")
endif()

# End Cppcheck Integration

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/vehicle_monitor.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
)

