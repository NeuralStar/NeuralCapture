#include "capture.h"

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
(t_handle* handle, std::ofstream& out, float*& buffer, uint32_t& buffer_size)
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
	std::cout << "Sampling Rate: " << Config::sample << "Hz" << std::endl;
	std::cout << "Frame Length: " << Config::frames << std::endl;
	std::cout << "Total Channels: " << channels << std::endl;
	std::cout << "Recording Duration: " << Config::duration << "s" << std::endl;
	std::cout << std::endl;

	// Prepare buffers
	buffer_size = channels * Config::frames;
	buffer = new float[buffer_size];
	if (!buffer) return false;

	// Prepare output file
	out.open(Config::file, std::ios_base::binary);
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
	(t_handle* handle, std::ofstream &out, float* &buffer, uint32_t &buffer_size, Data *const data)
{
	// Do last preparations and define collums
	if (UNICORN_StartAcquisition(*handle, Config::signal))
		return recordError(out, buffer, "The recording couldnt be started!");
	uint32_t calls = static_cast<int>(Config::duration * (static_cast<float>(Config::sample) / Config::frames));
	if (!defineCollums(handle, out))
		return recordError(out, buffer, "An error occured while defining collums!");

	// Wait until everything is ready
	data->captureRdy = true;
	if (!data->trainingRdy)
		std::cout << "Waiting on training module.." << std::endl;
	while (!data->trainingRdy);

	// Recording loop
	Timer<std::chrono::microseconds>	t;
	std::cout << "Recording has started!" << std::endl;
	for (uint32_t i = 0; i < calls && data->isActive; i++)
	{
		// Fetch the data from the headset
		if (UNICORN_GetData(*handle, Config::frames, buffer, buffer_size))
			recordError(out, buffer, "An error occured while recording!");

		// Output the returned data
		writeValues(out, buffer, buffer_size);
		writeDirectives(out, data);
		t.finish();
		out << "," << t.duration();
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
bool	recordDevice(t_handle* handle, Data *const data)
{
	// Variables
	std::ofstream out;
	float* buffer = nullptr;
	uint32_t buffer_size = 0;

	// Prepare and run
	if (!recordPrepare(handle, out, buffer, buffer_size))
		return false;
	if (!recordRun(handle, out, buffer, buffer_size, data))
		return false;

	// Clear memory and returns
	delete[] buffer;
	out.close();
	return true;
}