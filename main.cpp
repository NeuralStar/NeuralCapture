#include "core.h"

/**
 * Entrypoint of the software
 * @return	ErrorCode (0 for None)
 * */
int main()
{
	if (!config::training && config::capture)
		return launchSingle(launchCapture);
	else if (config::training && !config::capture)
		return launchSingle(launchTraining);
	else if (config::training && config::capture)
		return launchThreads();
	std::cout << "Warning: Both Training or Capture are inactive in config.h!" << std::endl;
	return EXIT_SUCCESS;
}