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

#ifndef CAN_UTILS_H
#define CAN_UTILS_H

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include "osap_common.h"

/**
 * @brief Initializes a CAN socket and binds it to a specified interface.
 *
 * This function creates a raw CAN socket, associates it with the given
 * CAN interface name, and binds the socket to that interface. It performs
 * error checking at each step and prints error messages to stderr if any
 * operation fails.
 *
 * @param ifname The name of the CAN interface (e.g., "vcan0", "can0").
 * This string should be null-terminated.
 * @return On success, returns the file descriptor of the initialized CAN socket.
 * On failure, returns -1 and sets errno appropriately.
 */
int initialize_can_socket(const char *ifname);

/**
 * @brief Continuously receives and prints CAN frames from a given socket.
 *
 * This function enters an infinite loop, reading CAN frames from the
 * specified socket. For each successfully received frame, it prints
 * the CAN ID, data length code (DLC), and the data bytes to standard output.
 * It handles partial reads and interrupted system calls, continuing
 * to listen for frames. If a read error occurs that is not EINTR, the
 * function will exit, closing the socket.
 *
 * @param sock_ The file descriptor of the initialized CAN socket from which
 * to receive frames.
 * @return Returns 0 on successful exit (e.g., if the socket is closed by peer,
 * though the loop is intended to be infinite in typical use).
 * Returns 1 if a critical read error occurs.
 */
int receive_can_frames(int sock_, struct can_frame *frame);

#endif // CAN_UTILS_H