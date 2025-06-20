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

#include <stdint.h>
#include <stdbool.h>

#include "osap_common.h"

/**
 * @brief Extracts a bitfield signal from a byte array.
 *
 * This function extracts a 'length' bit signal starting at 'startbit'
 * from a byte 'frame', handling both big-endian and little-endian systems.
 *
 * @param frame A pointer to the array of bytes of CAN frame
 * @param startbit The 0-indexed starting bit position of the signal within the frame.
 * @param length The length of the signal in bits (max 64 for uint64_t).
 * @param is_big_endian True if the frame data is big-endian, false for little-endian.
 * @return The extracted signal as a uint64_t. Returns 0 for invalid lengths.
 */
uint64_t extractSignal(const uint8_t *frame, uint16_t startbit, uint8_t length, bool is_big_endian);