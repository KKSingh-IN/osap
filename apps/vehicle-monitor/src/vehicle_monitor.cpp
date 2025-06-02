/*Copyright 2024 Kamlesh Singh
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
*/

#include "vehicle_monitor.h"
#include <iostream>
#include <chrono>

namespace vehicle_monitor{

std::thread VehicleMonitor::thread1_;
volatile sig_atomic_t VehicleMonitor::g_signal_status;
sigset_t VehicleMonitor::signals{};

VehicleMonitor::VehicleMonitor() {

}

VehicleMonitor::~VehicleMonitor() {

}

void VehicleMonitor::SignalHandler() {
    sigfillset(&signals);
    pthread_sigmask(SIG_BLOCK, &signals, nullptr);
    thread1_ = std::thread([&] () {
        int sig;
        if(0 == sigwait(&signals, &sig))
        {
            g_signal_status = 1;
        }
    });
}
// Dummy implementations of protocol clients
class CANClientImpl : public CANClientBase {
public:
    void readData() override {
        std::cout << "Reading CAN data" << std::endl;
    }
};

class MQTTClientImpl : public MQTTClientBase {
public:
    void readData() override {
        std::cout << "Reading MQTT data" << std::endl;
    }
};

class SomeIPClientImpl : public SomeIPClientBase {
public:
    void readData() override {
        std::cout << "Reading SOMEIP data" << std::endl;
    }
};

// Factory function implementations
std::unique_ptr<CANClientBase> createCANClient() {
    return std::make_unique<CANClientImpl>();
}

std::unique_ptr<MQTTClientBase> createMQTTClient() {
    return std::make_unique<MQTTClientImpl>();
}

std::unique_ptr<SomeIPClientBase> createSomeIPClient() {
    return std::make_unique<SomeIPClientImpl>();
}

// CANReader Implementation
class CANReader::Impl {
    std::unique_ptr<CANClientBase> can_client_;
    std::atomic<bool> running_{false};
    std::thread worker_thread_;

public:
    Impl() : can_client_(createCANClient()) {}

    bool init() {
        // Initialize CAN client
        return true;
    }

    void start() {
        if (running_) {
            std::cout << "CAN Reader is already running." << std::endl;
            return;
        }

        running_ = true;
        worker_thread_ = std::thread([this] {
            while (running_) {
                try {
                    can_client_->readData();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                } catch (const std::exception& e) {
                    std::cerr << "Error in CAN Reader: " << e.what() << std::endl;
                }
            }
        });
    }

    void stop() {
        running_ = false;
        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }
    }
};

CANReader::CANReader() : pImpl(std::make_unique<Impl>()) {}
CANReader::~CANReader() = default;
bool CANReader::init() { return pImpl->init(); }
void CANReader::start() { pImpl->start(); }
void CANReader::stop() { pImpl->stop(); }
std::string CANReader::getName() const { return "CAN"; }

// MQTTReader Implementation
class MQTTReader::Impl {
    std::unique_ptr<MQTTClientBase> mqtt_client_;
    std::atomic<bool> running_{false};
    std::thread worker_thread_;

public:
    Impl() : mqtt_client_(createMQTTClient()) {}

    bool init() {
        // Initialize MQTT client
        return true;
    }

    void start() {
        if (running_) {
            std::cout << "MQTT Reader is already running." << std::endl;
            return;
        }

        running_ = true;
        worker_thread_ = std::thread([this] {
            while (running_) {
                try {
                    mqtt_client_->readData();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                } catch (const std::exception& e) {
                    std::cerr << "Error in MQTT Reader: " << e.what() << std::endl;
                }
            }
        });
    }

    void stop() {
        running_ = false;
        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }
    }
};

MQTTReader::MQTTReader() : pImpl(std::make_unique<Impl>()) {}
MQTTReader::~MQTTReader() = default;
bool MQTTReader::init() { return pImpl->init(); }
void MQTTReader::start() { pImpl->start(); }
void MQTTReader::stop() { pImpl->stop(); }
std::string MQTTReader::getName() const { return "MQTT"; }

// SomeIPReader Implementation
class SomeIPReader::Impl {
    std::unique_ptr<SomeIPClientBase> someip_client_;
    std::atomic<bool> running_{false};
    std::thread worker_thread_;

public:
    Impl() : someip_client_(createSomeIPClient()) {}

    bool init() {
        // Initialize SomeIP client
        return true;
    }

    void start() {
        if (running_) {
            std::cout << "SomeIP Reader is already running." << std::endl;
            return;
        }

        running_ = true;
        worker_thread_ = std::thread([this] {
            while (running_) {
                try {
                    someip_client_->readData();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                } catch (const std::exception& e) {
                    std::cerr << "Error in SomeIP Reader: " << e.what() << std::endl;
                }
            }
        });
    }

    void stop() {
        running_ = false;
        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }
    }
};

SomeIPReader::SomeIPReader() : pImpl(std::make_unique<Impl>()) {}
SomeIPReader::~SomeIPReader() = default;
bool SomeIPReader::init() { return pImpl->init(); }
void SomeIPReader::start() { pImpl->start(); }
void SomeIPReader::stop() { pImpl->stop(); }
std::string SomeIPReader::getName() const { return "SOMEIP"; }

// MultiProtocolReader Implementation
MultiProtocolReader::MultiProtocolReader() {
    readers_.emplace_back(std::make_unique<CANReader>());
    readers_.emplace_back(std::make_unique<MQTTReader>());
    readers_.emplace_back(std::make_unique<SomeIPReader>());
}

MultiProtocolReader::~MultiProtocolReader() {
    stop();
}

bool MultiProtocolReader::init() {
    for (auto& reader : readers_) {
        if (!reader->init()) {
            std::cerr << "Failed to initialize " << reader->getName() << " reader." << std::endl;
            return false;
        }
    }
    return true;
}

void MultiProtocolReader::start() {
    running_ = true;
    for (auto& reader : readers_) {
        reader->start();
    }

    processing_thread_ = std::thread([this] {
        while (running_) {
            processData();
        }
    });
}

void MultiProtocolReader::stop() {
    running_ = false;
    for (auto& reader : readers_) {
        reader->stop();
    }
    if (processing_thread_.joinable()) {
        processing_thread_.join();
    }
}

void MultiProtocolReader::processData() {
    std::string data;
    if (data_queue_.try_pop(data)) {
        // Process the data
        std::cout << "Processing data: " << data << std::endl;
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} // namespace vehicle_monitor