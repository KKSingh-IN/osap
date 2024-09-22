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


option(BUILD_TESTS "Build the tests" ON)
option(USE_VSOMEIP "Use SOME/IP" ON)
option(USE_MQTT "Use MQTT" ON)
option(USE_CAN "Use CAN" ON)
option(USE_HTTPS "Use HTTPS" ON)
option(USE_MAC "Use MAC" ON)


include(FetchContent)


function(add_module NAME)
    add_library(${NAME} STATIC
        ../../src/${NAME}/${NAME}.cpp
        ../../include/${NAME}/${NAME}.h
    )
    target_include_directories(${NAME} PUBLIC ../../include/${NAME})
endfunction()



if(USE_VSOMEIP)
    FetchContent_Declare(
        vsomeip
        GIT_REPOSITORY https://github.com/GENIVI/vsomeip.git
        GIT_TAG 3.1.20.3
    )
    FetchContent_MakeAvailable(vsomeip)
endif()

if(USE_MQTT)
    find_package(PahoMqttCpp REQUIRED)
endif()

if(USE_CAN)
    find_package(SocketCAN REQUIRED)
endif()

if(USE_HTTPS)
    find_package(OpenSSL REQUIRED)
endif()

if(USE_MAC)

endif()

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/vehicle_monitor.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE

)

if(USE_VSOMEIP)
    target_link_libraries(${PROJECT_NAME} PRIVATE vsomeip)
endif()

if(USE_MQTT)
    target_link_libraries(${PROJECT_NAME} PRIVATE PahoMqttCpp::paho-mqttpp3)
endif()

if(USE_CAN)
    target_link_libraries(${PROJECT_NAME} PRIVATE SocketCAN::SocketCAN)
endif()

if(USE_HTTPS)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()

if(USE_MAC)

endif()
