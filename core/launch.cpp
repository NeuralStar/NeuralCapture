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
int		launchCapture(Data *const data)
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
		clear(devices, target, "An error occured while trying to record the device!");
		return exitThread(data, EXIT_FAILURE);
	}
	clear(devices, target, "System exiting successfully!");
	return exitThread(data, EXIT_SUCCESS);
}

/**
 * Entrypoint for training mode
 * @return	ErrorCode (0 for None)
 * */
int		launchTraining(Data *const data)
{
	// STEP 1: Pre-render graphics
	assets_type a = new Assets();
	if (!a || !a->arrows) return exitThread(data, EXIT_FAILURE);

	// STEP 2: Prepare window
	window_type win = new sf::RenderWindow
		(sf::VideoMode(config::width, config::height), config::title);
	if (!win) return exitThread(data, EXIT_FAILURE);

	// STEP 3: Run the window's cycle
	windowLoop(win, data, a);

	// STEP 4: Clear everything
	delete a;
	delete win;
	return exitThread(data, EXIT_SUCCESS);
}

/**
 * Handles and manages for singular runtimes
 * @return	ErrorCode (0 for None)
 * */
int launchSingle(int (*entrypoint)(Data* const))
{
	// Initialize data structure
	data_type data = new Data();
	if (!data) return EXIT_FAILURE;

	// Run the actual target
	int error = entrypoint(data);

	// Clear up memory
	delete data;
	return error;
}

/**
 * Handles and manages all threads ressources
 * @return	ErrorCode (0 for None)
 * */
int		launchThreads()
{
	// Initialize data structure
	data_type data = new Data();
	if (!data) return EXIT_FAILURE;

	// Prepares the runtime
	int error = 0;
	sf::Thread t(launchCapture, data);

	// Launch threads
	t.launch();
	error = launchTraining(data);
	t.wait();

	// Clear up memory
	delete data;
	return error;
}