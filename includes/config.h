#pragma once

namespace config
{

	// Toggle training mode (when on, can helps focus for capture)
	constexpr const bool		training = true;

	// Toggle capture mode (when off, helps debugging training)
	constexpr const bool		capture = true;

	/* ******************************************* */
	/*                                             */
	/*                CAPTURE MODE                 */
	/*                                             */
	/* ******************************************* */

	// Path of the output file
	constexpr const char*		file = "rosieTest2s.csv";

	// Sampling rate of the amplifiers
	constexpr const uint32_t	sample = 250;

	// How much data is fetch per data call
	constexpr const uint32_t	frames = 1;
	
	// Duration of the recording in seconds
	constexpr const double		duration = 60.0;

	// If signal checks is requierd
	constexpr const bool		signal = true;

	// Allows attempting to connect to non paired devices
	// WARNING: Can takes a very long time to search!
	constexpr const bool		allow_nopair = false;

	/* ******************************************* */
	/*                                             */
	/*                TRAINING MODE                */
	/*                                             */
	/* ******************************************* */

	// Window's title
	constexpr const char*		title = "Neural Training";

	// Window's height
	constexpr const uint32_t	height = 900;

	// Window's width
	constexpr const uint32_t	width = 1500;

	// Automatly switches directions every
	// NOTICE: Set to <0 for disable
	constexpr const uint32_t	timer = 2;

};