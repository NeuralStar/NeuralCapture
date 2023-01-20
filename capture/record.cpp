#include "capture.h"
#include <map>

/**
 * Requests and print out the channel's indexes,
 * to put down in the output file for each collumns
 * 
 * @param	handle: Target device's handle
 * @param	out: Output stream to write on
 * 
 * @return	True or False if was successful
 * */
static bool defineCollums(t_handle* handle, std::ofstream &out)
{
	// List of all possible entries
	const char* all[] =
	{
		"EEG 1", "EEG 2", "EEG 3", "EEG 4", "EEG 5", "EEG 6", "EEG 7", "EEG 8",
		"Accelerometer X", "Accelerometer Y", "Accelerometer Z",
		"Gyroscope X", "Gyroscope Y", "Gyroscope Z",
		"Battery Level", "Validation Indicator", "Counter",
	};

	std::map<uint32_t, const char*>		map;
	std::pair<const char*, uint32_t>	pair;

	// Get each index of each entries
	for (uint32_t i = 0; i < 17; i++)
	{
		pair = std::make_pair(all[i], 0);
		if (UNICORN_GetChannelIndex(*handle, pair.first, &pair.second))
		{
			std::cout << "Failed fetch channels index : " << pair.first << std::endl;
			return false;
		}
		map.insert(std::make_pair(pair.second, pair.first));
	}

	// Finally put each entries in index order
	for (auto it = map.begin(); it != map.end(); it++)
	{
		if (it != map.begin())
			out << ",";
		out << it->second;
	}
	return true;
}

/**
 * Prepares the recording by opening the output and
 * allocating the neccesary memory aswell as storing
 * a few extra requierd data
 * 
 * @param	handle: Target device's handle
 * @param	out: Output stream to open file on
 * @param	buffer: Pointer to memory to allocate
 * @param	buffer_size: Variable to store size of buffer
 * 
 * @return	True or False if preparation was successful
 * */
static bool	recordPrepare
	(t_handle* handle, std::ofstream &out, float* &buffer, uint32_t &buffer_size)
{
	// Get some data from the handle
	uint32_t channels = 0;
	if (UNICORN_GetNumberOfAcquiredChannels(*handle, &channels))
		return false;
	t_config configs;
	if (UNICORN_GetConfiguration(*handle, &configs))
		return false;

	// Print out recording configs
	std::cout << "Recording Configs:" << std::endl;
	std::cout << "Sampling Rate: " << config::sample << "Hz" << std::endl;
	std::cout << "Frame Length: " << config::frames << std::endl;
	std::cout << "Total Channels: " << channels << std::endl;
	std::cout << "Recording Duration: " << config::duration << std::endl;
	std::cout << std::endl;

	// Prepare buffers
	buffer_size = channels * config::frames;
	buffer = new float[buffer_size];
	if (!buffer) return false;

	// Prepare output file
	out.open(config::file, std::ios_base::binary);
	if (!out.is_open())
		return recordError(out, buffer, "Target output file couldn't be openned!");
	return true;
}

/**
 * Prepares the recording by opening the output and
 * allocating the neccesary memory aswell as storing
 * a few extra requierd data
 *
 * @param	handle: Target device's handle
 * @param	out: Output stream to write on
 * @param	buffer: Memory to use as buffer
 * @param	buffer_size: Size of the buffer
 *
 * @return	True or False if recording was successful
 * */
static bool recordRun
	(t_handle* handle, std::ofstream &out, float* &buffer, uint32_t &buffer_size)
{
	// Starts recording
	if (UNICORN_StartAcquisition(*handle, config::signal))
		return recordError(out, buffer, "The recording couldnt be started!");
	std::cout << "Recording has started!" << std::endl;

	// Recording loop
	uint32_t calls = static_cast<int>(config::duration * (static_cast<float>(config::sample) / config::frames));
	if (!defineCollums(handle, out))
		return recordError(out, buffer);
	for (uint32_t i = 0; i < calls; i++)
	{
		if (UNICORN_GetData(*handle, config::frames, buffer, buffer_size))
			recordError(out, buffer, "An error occured while recording!");
		for (uint32_t y = 0; y < buffer_size; y++)
		{
			if (y) out << ",";
			else out << "\n";
			out << buffer[y];
		}
	}

	// Stop recording
	if (UNICORN_StopAcquisition(*handle))
		return recordError(out, buffer, "The recording couldn't be stopped properly!");
	std::cout << "Recording has been stopped!" << std::endl;
	return true;
}

/**
 * Starts recording data from the target devices
 * 
 * @param	handle: Target device's handle
 * 
 * @return	True of False if successful
 * */
bool	recordDevice(t_handle* handle)
{
	// Variables
	std::ofstream out;
	float* buffer = nullptr;
	uint32_t buffer_size = 0;

	// Prepare and run
	if (!recordPrepare(handle, out, buffer, buffer_size))
		return false;
	if (!recordRun(handle, out, buffer, buffer_size))
		return false;

	// Clear memory and returns
	delete[] buffer;
	out.close();
	return true;
}