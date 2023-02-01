#pragma once

#include <cstdint>
#include <string>

class Config
{

	public:
		// Toggle training mode (when on, can helps focus for capture)
		static bool			training;

		// Toggle capture mode (when off, helps debugging training)
		static bool			capture;

		/* ******************************************* */
		/*                                             */
		/*                CAPTURE MODE                 */
		/*                                             */
		/* ******************************************* */

		// Path of the output file
		static std::string	file;

		// Sampling rate of the amplifiers
		static uint32_t		sample;

		// How much data is fetch per data call
		static uint32_t		frames;

		// Duration of the recording in seconds
		static double		duration;

		// If signal checks is requierd
		static bool			signal;

		// Allows attempting to connect to non paired devices
		// WARNING: Can takes a very long time to search!
		static bool			allow_nopair;

		/* ******************************************* */
		/*                                             */
		/*                TRAINING MODE                */
		/*                                             */
		/* ******************************************* */

		// Window's title
		static std::string	title;

		// Window's height
		static uint32_t		height;

		// Window's width
		static uint32_t		width;

		// Automatly switches directions every
		// NOTICE: Set to <0 for disable
		static uint32_t		timer;

};