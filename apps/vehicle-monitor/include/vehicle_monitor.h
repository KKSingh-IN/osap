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

#ifndef VEHICLE_MONITOR_H
#define VEHICLE_MONITOR_H

#include <memory>
#include <vector>
#include <string>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

// Abstract base classes for protocol clients
class CANClientBase {
public:
    virtual ~CANClientBase() = default;
    virtual void readData() = 0;
};

class MQTTClientBase {
public:
    virtual ~MQTTClientBase() = default;
    virtual void readData() = 0;
};

class SomeIPClientBase {
public:
    virtual ~SomeIPClientBase() = default;
    virtual void readData() = 0;
};

// Factory functions to create protocol clients
std::unique_ptr<CANClientBase> createCANClient();
std::unique_ptr<MQTTClientBase> createMQTTClient();
std::unique_ptr<SomeIPClientBase> createSomeIPClient();

class ProtocolReader {
public:
    virtual ~ProtocolReader() = default;
    virtual bool init() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual std::string getName() const = 0;
};

class CANReader : public ProtocolReader {
    class Impl;
    std::unique_ptr<Impl> pImpl;

public:
    CANReader();
    ~CANReader() override;

    bool init() override;
    void start() override;
    void stop() override;
    std::string getName() const override;
};

class MQTTReader : public ProtocolReader {
    class Impl;
    std::unique_ptr<Impl> pImpl;

public:
    MQTTReader();
    ~MQTTReader() override;

    bool init() override;
    void start() override;
    void stop() override;
    std::string getName() const override;
};

class SomeIPReader : public ProtocolReader {
    class Impl;
    std::unique_ptr<Impl> pImpl;

public:
    SomeIPReader();
    ~SomeIPReader() override;

    bool init() override;
    void start() override;
    void stop() override;
    std::string getName() const override;
};

template<typename T>
class ThreadSafeQueue {
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;

public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
        cond_.notify_one();
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }
};

class MultiProtocolReader {
    std::vector<std::unique_ptr<ProtocolReader>> readers_;
    ThreadSafeQueue<std::string> data_queue_;
    std::atomic<bool> running_{false};
    std::thread processing_thread_;

public:
    MultiProtocolReader();
    ~MultiProtocolReader();

    bool init();
    void start();
    void stop();

private:
    void processData();
};


#endif