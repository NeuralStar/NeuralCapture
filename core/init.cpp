#include "core.h"

/**
 * Total count of avaible devices
 * */
uint32_t g_count = 0;

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
	if (UNICORN_GetAvailableDevices(nullptr, &g_count, true))
		return clear(devices, nullptr, "An error occured while searching for devices!");
	if (!g_count)
		return clear(devices, nullptr, "No devices could be found!\nPlease pair a device first then retry..");
	std::cout << "Found Devices: " << g_count << std::endl;

	// Allocate the memory to store the devices
	devices = new t_device[g_count];
	if (!devices)
		return clear(devices, nullptr, "Failed to allocate memory for: Array of devices!");

	// Store the devices within the allocated memory
	if (UNICORN_GetAvailableDevices(devices, &g_count, true))
		return clear(devices, nullptr, "Failed to fetch the devices!");
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
	if (g_count > 2)
	{
		std::string			input;

		// Print out avaible devices
		std::cout << "Choose a device:" << std::endl;
		for (uint32_t i = 0; i < g_count; i++)
			std::cout << " - " << i + 1 << " : " << devices[i] << std::endl;

		// Request Input
		std::cout << "Enter Index: ";
		std::cin >> input;

		// Exit if requested
		if (input == "EXIT")
			return clear(devices, handle, "Exited..");
		
		// Convert input into number
		select = atoi(input.c_str());

		// If invalid, print out error message
		if (!select || select > g_count)
		{
			std::cout << "Please choose a correct value (between 1 and " << g_count << ")" << std::endl
				<< "Or exit with EXIT" << std::endl;
			return clear(devices, handle);
		}
		select--;
	}

	// Allocate memory for the handle
	handle = new t_handle;
	if (!handle) return clear(devices, handle, "Failed to allocate memory for handle!");

	// Store the handle data within the allocated memory
	if (UNICORN_OpenDevice(devices[select], handle))
		return clear(devices, handle, "An error occured while trying to connect to the target device!");
	return handle;
}