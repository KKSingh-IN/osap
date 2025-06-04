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
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "can_receiver.h"

int initialize_can_socket(const char *ifname)
{
    int sock_ = -1;
    struct sockaddr_can addr;
    struct ifreq ifr;

    // Create a CAN socket
    if ((sock_ = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket creation failed");
        return -1;
    }

    // Get the interface index
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0'; // Ensure null-termination

    if (ioctl(sock_, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("ioctl SIOCGIFINDEX failed");
        close(sock_); // Clean up the socket
        return -1;
    }

    // Bind the socket to the CAN interface
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock_, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Socket bind failed");
        close(sock_); // Clean up the socket
        return -1;
    }

    printf("Successfully initialized CAN socket on interface: %s\n", ifname);
    return sock_;
}

int receive_can_frames(int sock_)
{
    struct can_frame frame;
    int nbytes;

    if (sock_ < 0)
    {
        fprintf(stderr, "Error: Invalid socket descriptor passed to receive_can_frames.\n");
        return 1;
    }

    printf("Listening for CAN frames...\n");

    while (1)
    { // Loop indefinitely to receive frames
        nbytes = read(sock_, &frame, sizeof(struct can_frame));

        if (nbytes < 0)
        {
            if (errno == EINTR)
            { // Interrupted system call, try again
                continue;
            }
            perror("Read error on CAN socket");
            return 1; // Critical read error
        }
        else if (nbytes == 0)
        {
            // End of file (shouldn't happen with sockets unless closed by peer)
            fprintf(stderr, "Read returned 0 bytes (socket possibly closed by peer).\n");
            break; // Exit loop
        }
        else if (nbytes < sizeof(struct can_frame))
        {
            fprintf(stderr, "Partial CAN frame received: %d bytes. Expected %lu.\n", nbytes, sizeof(struct can_frame));
            continue; // Continue to next read
        }

        // Print the received CAN frame
        printf("0x%03X [%d] ", frame.can_id, frame.can_dlc);
        for (int i = 0; i < frame.can_dlc; i++)
        {
            printf("%02X ", frame.data[i]);
        }
        printf("\n"); // Use newline instead of \r\n for better cross-platform console behavior
    }

    return 0; // Normal exit from loop (e.g., if socket was closed by peer)
}