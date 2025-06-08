/*
 * Copyright 2024 Kamlesh Singh
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "vehicle_signal.h"
#include "can_receiver.h"
#include "extract_signal.h"

extern struct SignalDefinition signals[];
extern const int NUM_SIGNALS;

void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

/**
 * @brief Main function for the CAN frame listener application.
 *
 * This program initializes a CAN socket on a specified or default interface
 * and then continuously listens for and prints incoming CAN frames.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * - argv[0]: The name of the executable.
 * - argv[1] (optional): The name of the CAN interface to use (e.g., "vcan0", "can0").
 * If not provided, "vcan0" is used as default.
 * @return 0 on successful execution and termination.
 * 1 on error (e.g., incorrect usage, socket initialization failure, read error).
 */
int main(int argc, char **argv)
{
    int sock_ = -1;
    const char *ifname = "vcan0"; // Default interface name
    struct can_frame frame;

    // Parse command-line arguments
    if (argc > 2)
    {
        fprintf(stderr, "Usage: %s [interface]\n", argv[0]);
        return 1;
    }
    else if (argc == 2)
    {
        ifname = argv[1];
    }

    // Initialize the CAN socket
    sock_ = initialize_can_socket(ifname);
    if (sock_ < 0)
    {
        fprintf(stderr, "Failed to initialize CAN socket on interface '%s'. Exiting.\n", ifname);
        return 1;
    }


    // Start receiving CAN frames
    while (1)
    {
        int receive_status = receive_can_frames(sock_, &frame);
        uint64_t sig_val = 0;

        if (E_OK == receive_status)
        {
            for (int i = 0; i < NUM_SIGNALS; ++i)
            {
                if (frame.can_id == signals[i].can_id)
                {
                    sig_val = extractSignal((const uint8_t *)&frame.data, signals[i].start_bit, signals[i].length, signals[i].is_big_endian);
                }
            }
        }
        sleep_ms(100);
    }
    // Clean up: Close the socket
    if (close(sock_) < 0)
    {
        perror("Error closing CAN socket");
        return 1; // Indicate error during close
    }
    return 0;
}