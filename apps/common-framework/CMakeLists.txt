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

add_library(common-framework SHARED
    Application.cpp
    signals/SignalBus.cpp
    state/CommonApplicationStates.cpp
    state/StateMachine.cpp
)


target_include_directories(common-framework PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # common-framework/
    ${CMAKE_CURRENT_SOURCE_DIR}/signals # common-framework/signals/
    ${CMAKE_CURRENT_SOURCE_DIR}/state   # common-framework/state/
)

target_link_libraries(common-framework PUBLIC pthread)

# Set properties for the shared library
set_target_properties(common-framework PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)

# install(TARGETS common-framework DESTINATION lib)
# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#         DESTINATION include/common-framework
#         FILES_MATCHING PATTERN "*.hpp"
#         PATTERN "build" EXCLUDE
#         PATTERN "CMakeLists.txt" EXCLUDE
# )