#include "capture.h"

/**
 * Total count of paired and not paired
 * avaible devices
 * */

uint32_t g_paired = 0;
uint32_t g_unpair = 0;

/**
 * Prints out avaible devices
 *
 * @param	devices: Array of available devices
 * @param	index_offset: Offset of indexes for display purposes
 * */
static void printDevices(t_device* devices, const uint32_t index_offset)
{
	if (Config::allow_nopair || g_unpair)
		std::cout << " - Paired - " << std::endl;
	for (uint32_t i = 0; i < g_paired; i++)
		std::cout << " - " << i + index_offset << " : " << devices[i] << std::endl;
	if (Config::allow_nopair || g_unpair)
	{
		std::cout << " - Non Paired - " << std::endl;
		for (uint32_t i = g_paired; i < g_unpair; i++)
			std::cout << " - " << i + index_offset << " : " << devices[i] << std::endl;
	}
}

/**
 * Searches and register nearby devices
 * using the Unicorn API
 * 
 * @return	Array of availabe devices
 * */
t_device* fetch_devices(void)
{
	t_device*	devices = nullptr;

	// Count the avaible devices
	if (UNICORN_GetAvailableDevices(nullptr, &g_paired, true))
		return clear(devices, nullptr, "An error occured while searching for devices!");
	if (Config::allow_nopair && UNICORN_GetAvailableDevices(nullptr, &g_unpair, false))
		return clear(devices, nullptr, "An error occured while searching for unpaired devices!");
	if (!(g_paired + g_unpair))
		return clear(devices, nullptr, "No devices could be found!\nPlease pair a device first then retry..");

	// Display counts
	std::cout << "Found Devices: " << g_paired + g_unpair << std::endl;
	if (Config::allow_nopair)
		std::cout << "Paired: " << g_paired << " | Not Paired: " << g_unpair << std::endl;

	// Allocate the memory to store the devices
	devices = new t_device[g_paired + g_unpair];
	if (!devices)
		return clear(devices, nullptr, "Failed to allocate memory for: Array of devices!");

	// Store the devices within the allocated memory
	if (UNICORN_GetAvailableDevices(devices, &g_paired, true))
		return clear(devices, nullptr, "Failed to fetch the devices!");
	if (Config::allow_nopair && UNICORN_GetAvailableDevices(&devices[g_paired], &g_unpair, false))
		return clear(devices, nullptr, "Failed to fetch the unpaired devices!");
	return devices;
}

/**
 * Prompt and select any located devices
 * within Unicorn API
 * 
 * @param	devices: Array of available devices
 * 
 * @return	Handle of the choosen device
 * */
t_handle* select_device(t_device* devices)
{
	t_handle*	handle = nullptr;
	uint32_t	select = 0;

	// if: multiple devices are available, prompt a selector
	// otherwise: auto selects the only one available
	if (g_paired > 2)
	{
		std::string			input;

		// Prompt the user with available devices
		std::cout << "Choose a device:" << std::endl;
		printDevices(devices, 1);
		std::cout << "Enter Index: ";
		std::cin >> input;

		// Convert input into number or Exit if requested
		if (input == "EXIT")
			return clear(devices, handle, "Exited..");
		select = atoi(input.c_str());

		// If invalid, print out error message
		if (!select || select > g_paired)
		{
			std::cout << "Please choose a correct value (between 1 and " << g_paired + g_unpair << ")" << std::endl
				<< "Or exit with EXIT" << std::endl;
			return clear(devices, handle);
		}
		select--;
	}

	// Allocate memory for the handle
	handle = new t_handle;
	if (!handle) return clear(devices, handle, "Failed to allocate memory for handle!");

	// Store the handle data within the allocated memory
	std::cout << "Trying to connect to: " << devices[select] << "..." << std::endl;
	if (UNICORN_OpenDevice(devices[select], handle))
		return clear(devices, handle, "An error occured while trying to connect to the target device!");
	std::cout << "Connection successfully etablished!" << std::endl;
	return handle;
}