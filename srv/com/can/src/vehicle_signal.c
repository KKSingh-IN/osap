#include "vehicle_signal.h"
#include <string.h>


struct SignalDefinition signals[] = {
    {
        .name = "EngineRPM",
        .can_id = 0x1A0,
        .start_bit = 0,
        .length = 16,
        .scale = 0.25,
        .offset = 0.0,
        .is_signed = 0,
        .is_big_endian = 0,
        .unit = "rpm"
    },
    {
        .name = "VehicleSpeed",
        .can_id = 0x2B0,
        .start_bit = 8,
        .length = 12,
        .scale = 0.01,
        .offset = 0.0,
        .is_signed = 0,
        .is_big_endian = 1,
        .unit = "km/h"
    },
    {
        .name = "CoolantTemp",
        .can_id = 0x3C0,
        .start_bit = 0,
        .length = 8,
        .scale = 1.0,
        .offset = -40.0,
        .is_signed = 1,
        .is_big_endian = 0,
        .unit = "degC"
    },
    {
        .name = "FuelLevel",
        .can_id = 0x4D0,
        .start_bit = 4,
        .length = 8,
        .scale = 0.5,
        .offset = 0.0,
        .is_signed = 0,
        .is_big_endian = 0,
        .unit = "%"
    },
    {
        .name = "BrakePressure",
        .can_id = 0x5E0,
        .start_bit = 0,
        .length = 10,
        .scale = 0.1,
        .offset = 0.0,
        .is_signed = 0,
        .is_big_endian = 1,
        .unit = "kPa"
    }
};

const int NUM_SIGNALS = sizeof(signals) / sizeof(signals[0]);