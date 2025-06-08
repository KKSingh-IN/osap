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

#include "extract_signal.h"

// Using 'ULL' for unsigned long long literal to ensure 64-bit operation.
#define MASK64(nbits) ((nbits) == 0 ? 0ULL : (0xFFFFFFFFFFFFFFFFULL >> (64 - (nbits))))

uint64_t extractSignal(const uint8_t *frame, uint16_t startbit, uint8_t length, bool is_big_endian)
{
    // Input Validation
    if (length == 0 || length > 64)
    {
        return 0ULL; // Return 0 or handle error appropriately for invalid lengths
    }

    // Determine the byte range involved
    uint16_t startByte = startbit / 8;
    uint8_t bitOffsetInStartByte = startbit % 8;

    // The end bit position (inclusive)
    uint16_t endBit = startbit + length - 1;
    uint16_t endByte = endBit / 8;

    // Number of bytes involved in the extraction, covering all bits from startbit to endBit
    // Add 1 because it's an inclusive range [startByte, endByte]
    uint8_t bytesToFetch = endByte - startByte + 1;

    uint64_t rawValue = 0ULL;

    // Big-endian
    if (is_big_endian)
    {
        // For big-endian, the highest-address byte contains the most significant bits
        // Read bytes from startByte forwards.
        for (uint8_t i = 0; i < bytesToFetch; ++i)
        {
            rawValue = (rawValue << 8) | frame[startByte + i];
        }

        // Remove the bits that are after desired signal
        // within the `rawValue` block.
        // The number of trailing bits is:
        // (total bits read into rawValue) - (total signal bits including initial offset)
        // (bytesToFetch * 8) - (bitOffsetInStartByte + length)
        uint8_t bitsToShiftRight = (bytesToFetch * 8) - (bitOffsetInStartByte + length);
        rawValue >>= bitsToShiftRight;
    }
    else
    { // Little-endian
        // For little-endian, the lowest-address byte contains the least significant bits
        // Read bytes from startByte forwards.
        for (uint8_t i = 0; i < bytesToFetch; ++i)
        {
            // Cast frame[startByte + i] to uint64_t before shifting to avoid overflow if i*8 > 7
            rawValue |= ((uint64_t)frame[startByte + i]) << (i * 8);
        }

        // After assembling, the bits are aligned such that the last byte read is the MSB.
        // Right-shift to remove the leading bits before the signal's LSB.
        rawValue >>= bitOffsetInStartByte;
    }

    // Mask to isolate the desired length bits
    return rawValue & MASK64(length);
}