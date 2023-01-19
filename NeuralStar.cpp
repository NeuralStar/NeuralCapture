#include "core.h"

/**
 * Entrypoint of the software
 * 
 * @return	ErrorCode (0 for None)
 * */
int main()
{
	// STEP 1: Verify system
	printAPIVersion();
	if (!checkConfig()) return EXIT_FAILURE;

	// STEP 2: Get Devices
	t_device* devices = fetch_devices();
	if (!devices) return EXIT_FAILURE;
	t_handle* target = select_device(devices);
	if (!target) return EXIT_FAILURE;

	// STEP 3: Recording
	if (!recordDevice(target))
	{
		clear(devices, target, "An error occured while trying to record the device!");
		return EXIT_FAILURE;
	}
	else
	{
		clear(devices, target, "System exiting successfully!");
		return EXIT_SUCCESS;
	}
}