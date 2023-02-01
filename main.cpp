#include "core.h"

/**
 * Entrypoint of the software
 * 
 * @param	c: Amount of given arguments
 * @param	args: Array of given arguments
 * 
 * @return	ErrorCode (0 for None)
 * */
int main(int c, char** args)
{
	// Load Config
	const char* file = "config.json";
	if (c > 2)
		file = args[1];
	if (!loadConfig(file))
		return EXIT_FAILURE;

	// Start modules
	if (!Config::training && Config::capture)
		return launchSingle(launchCapture);
	else if (Config::training && !Config::capture)
		return launchSingle(launchTraining);
	else if (Config::training && Config::capture)
		return launchThreads();

	// If none of modules are enabled
	std::cout << "Warning: Both Training or Capture are inactive in configuration file!" << std::endl;
	return EXIT_SUCCESS;
}