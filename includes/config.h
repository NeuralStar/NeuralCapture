#pragma once
#include "unicorn.h"

namespace config {

	// Path of the output file
	constexpr const char*	file = "data.csv";

	// Sampling rate of the amplifiers
	constexpr uint32_t		sample = UNICORN_SAMPLING_RATE;

	// How much data is fetch per data call
	constexpr uint32_t		frames = 1;

	// Duration of the recording in seconds
	constexpr double		duration = 10.0;

	// If signal checks is requierd
	constexpr bool			signal = true;

};