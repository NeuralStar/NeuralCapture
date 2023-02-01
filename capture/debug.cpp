#include "capture.h"

/**
 * Clear up used memory by passing the used
 * allocated variables and print out an message
 * if any is given
 * 
 * @param	devices: Array of avaible devices
 * @param	handle: Handle of the choosen device
 * @param	msg: Message to print
 * 
 * @return	Null pointer
 * */
nullptr_t clear(t_device *devices, t_handle *handle, const std::string &msg)
{
	if (devices)
		delete[] devices;
	if (handle)
		delete handle;

	if (!msg.empty())
		std::cout << msg << std::endl;
	return nullptr;
}

/**
 * Simply destroys the given buffer
 * and closes the output file
 *
 * @param	out: Target output file
 * @param	buffer: Recording's buffer array
 * @param	msg: Additional message to print
 *
 * @return	False
 * */
bool recordError(std::ofstream &out, float* buffer, const std::string& msg)
{
	if (out.is_open())
		out.close();
	if (buffer)
		delete[] buffer;

	if (!msg.empty())
		std::cout << msg << std::endl;
	return false;
}

/**
 * Simply prints out the version of the used APIs
 * */
void	printAPIVersion()
{
	std::cout << "Starting up system.." << std::endl
		<< "Unicorn API Version: " << UNICORN_GetApiVersion() << std::endl
		<< std::endl;
}

/**
 * Quickly checks the config content
 * 
 * @return	True or False if config is okay
 * */
bool	checkConfig()
{
	if (Config::duration == 0
		|| Config::frames == 0
		|| Config::sample == 0)
	{
		std::cout << "Values defined in the configuration file cannot be set to 0!" << std::endl;
		return false;
	}
	return true;
}
