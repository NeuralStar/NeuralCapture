#include "core.h"

/**
 * Load in the configuration file
 * 
 * @param	p: Target file to load configs from
 * 
 * @return	True or False if preparation was successful
 * */
bool loadConfig(const std::string& p)
{
	try
	{
		// Load JSON data
		std::ifstream	f(p);
		nlohmann::json	j, t;
		if (!f.is_open())
			throw std::exception("No such file or directory");
		f >> j;
		f.close();

		// Insert Saving data
		t = j.at("saving");
		try
		{
			Config::method = t.at("method");
			if (Config::method == 0)
				Config::file = t.at("file");
			else
			{
				Config::ip = t.at("ip");
				Config::port = t.at("port");
			}
		}
		catch (...) { throw; }

		// Insert Training data
		t = j.at("training");
		Config::training = t.at("enable");
		try
		{
			if (Config::training)
			{
				Config::title = t.at("title");
				Config::height = t.at("height");
				Config::width = t.at("width");
				Config::timer = t.at("timer");
			}
		}
		catch (...) { throw; }

		// Insert Capture data
		t = j.at("capture");
		Config::capture = t.at("enable");
		try
		{
			if (Config::capture)
			{
				Config::sample = t.at("sample");
				Config::frames = t.at("frames");
				Config::duration = t.at("duration");
				Config::signal = t.at("signal");
				Config::allow_nopair = t.at("allow_nopair");
			}
		}
		catch (...) { throw; }
		return true;
	}
	catch (const std::exception	&e)
	{
		// Handles error
		std::cout << "Error while loading " << p << ":" << std::endl
			<< e.what() << std::endl;
		return false;
	}
}

/**
 * Handles and manages for singular runtimes
 * @return	ErrorCode (0 for None)
 * */
int launchSingle(int (*entrypoint)(data_type))
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