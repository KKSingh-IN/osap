#ifndef VEHICLE_SIGNAL_H
#define VEHICLE_SIGNAL_H

#include <stdint.h>

#define MAX_SIGNAL_NAME_LENGTH 64
#define MAX_UNIT_NAME_LENGTH 64

struct SignalDefinition
{
    char name[MAX_SIGNAL_NAME_LENGTH];
    uint32_t can_id;
    uint8_t start_bit;
    uint8_t length;
    double scale;
    double offset;
    uint8_t is_signed;
    uint8_t is_big_endian;
    char unit[MAX_UNIT_NAME_LENGTH];
};

#endif VEHICLE_SIGNAL_H