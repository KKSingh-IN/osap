## License
Copyright 2025 Kamlesh Singh

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.



# Automotive Application Requirements

This document outlines the system-level and software requirements for the sample vehicle monitor application, focusing on automotive communication.

## 1. General System Requirements

### 1.1. Application Platform

* **REQ-SYS-001: Target Operating System Support:** The application shall be deployable on a Linux-based operating system (e.g., Ubuntu, Yocto Linux for embedded).
* **REQ-SYS-002: Hardware Compatibility:** The application shall be compatible with standard automotive-grade hardware or simulation environments capable of supporting the specified communication protocols (e.g., Raspberry Pi with CAN HAT, automotive-grade embedded board, or a PC with a virtual CAN interface).
* **REQ-SYS-003: Modularity:** The application architecture shall be modular to allow for easy integration of new communication protocols or features.
* **REQ-SYS-004: Configuration Management:** The application shall provide a clear and organized way to configure communication parameters (e.g., IP addresses, port numbers, CAN IDs, MQTT topics).

### 1.2. Build Environment

* **REQ-BUILD-001: Cross-Compilation Support:** The build system shall support cross-compilation for the target embedded platform.
* **REQ-BUILD-002: Dependency Management:** The build system shall clearly define and manage all external dependencies.
* **REQ-BUILD-003: Automated Testing Integration:** The build process shall integrate with a testing framework to run automated unit and integration tests.
* **REQ-BUILD-004: Version Control Integration:** The project shall be fully integrated with Git for version control and hosted on GitHub.

### 1.3. Runtime Environment

* **REQ-RUNTIME-001: Logging and Diagnostics:** The application shall provide comprehensive logging capabilities for debugging and monitoring communication activities and application behavior.
* **REQ-RUNTIME-002: Error Handling:** The application shall gracefully handle communication errors, network disconnections, and protocol-specific error conditions.
* **REQ-RUNTIME-003: Resource Management:** The application shall efficiently manage system resources (CPU, memory) to ensure stable operation in a resource-constrained environment.

## 2. Communication Protocol Specific Requirements

### 2.1. CAN (Controller Area Network)

* **REQ-CAN-001: CAN Interface Support:** The application shall support communication over a virtual or physical CAN interface (e.g., `socketcan` on Linux, a USB-to-CAN adapter).
* **REQ-CAN-002: CAN Frame Sending/Receiving:** The application shall be able to send and receive standard CAN frames (11-bit ID) and extended CAN frames (29-bit ID).
* **REQ-CAN-003: CAN Data Parsing/Serialization:** The application shall be able to parse incoming CAN messages into meaningful data and serialize data into CAN frames for transmission, potentially using a DBC file for signal definitions.
* **REQ-CAN-004: CAN FD (Flexible Data-Rate) Support (Optional but Recommended):** The application should support CAN FD for higher data throughput.
* **REQ-CAN-005: Message Prioritization:** The CAN communication module shall handle message prioritization inherent to the CAN protocol.
* **REQ-CAN-006: Error Detection and Handling:** The CAN communication module shall handle CAN error frames and bus-off conditions.

### 2.2. SOME/IP (Scalable service-Oriented MiddlewarE over IP)

* **REQ-SOMEIP-001: SOME/IP Service Discovery (SOME/IP-SD):** The application shall implement SOME/IP Service Discovery to dynamically discover and offer services on the network.
* **REQ-SOMEIP-002: Service/Client Role Implementation:** The application shall be able to act as both a SOME/IP service provider and a SOME/IP client.
* **REQ-SOMEIP-003: Method Call and Event Subscription:** The application shall support SOME/IP method calls (RPC) and event subscription/notification mechanisms.
* **REQ-SOMEIP-004: Data Serialization/Deserialization:** The application shall correctly serialize and deserialize SOME/IP messages according to the SOME/IP standard, handling various data types (structs, arrays, enums, bitfields).
* **REQ-SOMEIP-005: Multicast Communication:** The SOME/IP module shall support multicast for service discovery and event propagation.
* **REQ-SOMEIP-006: UDP/TCP Transport:** The SOME/IP implementation should support both UDP (for events and notifications) and TCP (for reliable method calls).

### 2.3. MQTT (Message Queuing Telemetry Transport)

* **REQ-MQTT-001: MQTT Client Implementation:** The application shall implement an MQTT client to connect to an MQTT broker.
* **REQ-MQTT-002: Publish/Subscribe Functionality:** The application shall be able to publish messages to specific MQTT topics and subscribe to topics to receive messages.
* **REQ-MQTT-003: Quality of Service (QoS) Support:** The application shall support different MQTT QoS levels (0, 1, 2) for message delivery guarantees.
* **REQ-MQTT-004: Retained Messages and Last Will and Testament:** The application should demonstrate the use of retained messages and Last Will and Testament features for maintaining state and handling unexpected disconnections.
* **REQ-MQTT-005: Secure Connection (TLS/SSL):** The application shall support secure MQTT connections using TLS/SSL.
* **REQ-MQTT-006: Reconnection Logic:** The MQTT client shall implement robust automatic reconnection logic in case of network interruptions.

## UDS (Unified Diagnostic Services) over CAN/DoIP (Diagnostics over IP):
* **REQ-UDS-001: The application shall demonstrate basic UDS services**

## 3. Testing:

* **REQ-TEST-001: Unit Tests:** Each major component and communication module shall have dedicated unit tests.
* **REQ-TEST-002: Integration Tests:** Integration tests shall verify end-to-end communication flows across different protocols.
* **REQ-TEST-003: Simulation Environment:** The project shall include a basic simulation environment to test communication protocols without physical hardware.

## 4. Documentation:

* **REQ-DOC-001: Architecture Documentation:** The project shall include documentation outlining the overall system architecture and design choices.
* **REQ-DOC-002: API Documentation:** All exposed APIs for communication modules shall be clearly documented.
* **REQ-DOC-003: Setup and Usage Guide:** Comprehensive instructions for setting up the build and runtime environment and using the application shall be provided.
* **REQ-DOC-004: Protocol Specific Details:** Documentation shall include details on how each protocol is implemented and configured within the application.