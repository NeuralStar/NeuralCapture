#include "core.h"

/**
 * Quick terminate function
 * */
static const int exitThread(data_type d, const int v)
{
	d->isActive = false;
	return v;
}

/**
 * Entrypoint for the brainwave captures
 * @return	ErrorCode (0 for None)
 * */
int		launchCapture(data_type data)
{
	// STEP 1: Verify system
	printAPIVersion();
	if (!checkConfig()) return exitThread(data, EXIT_FAILURE);

	// STEP 2: Get Devices
	t_device* devices = fetch_devices();
	if (!devices) return exitThread(data, EXIT_FAILURE);
	t_handle* target = select_device(devices);
	if (!target) return exitThread(data, EXIT_FAILURE);

	// STEP 3: Recording
	if (!recordDevice(target, data))
	{

	// STEP 4: Clear everything
		stop_connection();
		clear(devices, target, "An error occured while trying to record the device!");
		return exitThread(data, EXIT_FAILURE);
	}
	stop_connection();
	clear(devices, target, "System exiting successfully!");
	return exitThread(data, EXIT_SUCCESS);
}

/**
 * Entrypoint for training mode
 * @return	ErrorCode (0 for None)
 * */
int		launchTraining(data_type data)
{
	// STEP 1: Pre-render graphics
	assets_type a = new Assets();
	if (!a || !a->arrows) return exitThread(data, EXIT_FAILURE);

	// STEP 2: Prepare window
	window_type win = new sf::RenderWindow
		(sf::VideoMode(Config::width, Config::height), Config::title);
	if (!win) return exitThread(data, EXIT_FAILURE);

	// STEP 3: Run the window's cycle
	windowLoop(win, data, a);

	// STEP 4: Clear everything
	delete a;
	delete win;
	return exitThread(data, EXIT_SUCCESS);
}