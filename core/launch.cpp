#include "core.h"

/**
 * Entrypoint for the brainwave captures
 * @return	ErrorCode (0 for None)
 * */
int		launchCapture(Data *const)
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

/**
 * Entrypoint for training mode
 * @return	ErrorCode (0 for None)
 * */
int		launchTraining(Data *const data)
{
	// STEP 1: Pre-render graphics and prepare the window
	sf::VertexArray** v = new sf::VertexArray*[2];
	if (!v) return EXIT_FAILURE;
	v[Data::Arrow] = prepareArrows();
	if (!v[Data::Arrow]) return EXIT_FAILURE;
	sf::RenderWindow* const win = new sf::RenderWindow
		(sf::VideoMode(config::width, config::height), config::title);
	if (!win) return EXIT_FAILURE;

	// STEP 2: Run the window's cycle
	windowLoop(win, v, data);

	// STEP 3: Clear everything
	delete[] v[Data::Arrow];
	delete[] v;
	delete win;

	return EXIT_SUCCESS;
}

/**
 * Handles and manages for singular runtimes
 * @return	ErrorCode (0 for None)
 * */
int launchSingle(int (*entrypoint)(Data* const))
{
	// Initialize data structure
	Data* const data = new Data();
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
	Data* const data = new Data();
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