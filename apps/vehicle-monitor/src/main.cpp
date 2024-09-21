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

#include <iostream>
#include <csignal>
#include <unistd.h>

volatile sig_atomic_t g_signal_status = 0;

void signal_handler(int signal) {
    g_signal_status = signal;
}

int main(int argc, char *argv[]) {

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);


    while (g_signal_status == 0) {
        std::cout << "Alive..." << std::endl;
        sleep(1);
    }

    std::cout << std::endl;
    if (g_signal_status == SIGINT) {
        std::cout << "SIGINT received" << std::endl;
    } else if (g_signal_status == SIGTERM) {
        std::cout << "SIGTERM received" << std::endl;
    }

    return 0;
}