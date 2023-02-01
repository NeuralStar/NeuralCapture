#include "training.h"

/**
 * Takes the input and alters data to performs
 * the tasks related to the given inputs
 * 
 * @param	win: Targetted window
 * @param	key: Key input received
 * */
static void keyHandler
	(window_type win, const sf::Event::KeyEvent &key, data_type data)
{
	switch (key.code)
	{
		case sf::Keyboard::Escape:
			data->isActive = false;
			win->close();
			break;

		case sf::Keyboard::Space:
		case sf::Keyboard::Return:
			data->display = Data::None;
			break;

		default:
			break;
	}
}

/**
 * Handles the given event accordingly
 * 
 * @param	win: Targetted window
 * @param	event: Event to handle
 * */
static void eventHandler
	(window_type win, const sf::Event &event, data_type data)
{
	switch (event.type)
	{
		case sf::Event::Closed:
			win->close();
			break;

		case sf::Event::KeyPressed:
			keyHandler(win, event.key, data);
			break;

		default:
			break;
	}
}

/**
 * Handles the logic system, which is the inbetween
 * task before events and drawing
 * 
 * @param	data: Internal data about the software
 * */
static void logicHandler(data_type data)
{
	static Timer<std::chrono::seconds>	rng;
	static Timer<std::chrono::seconds>	min;
	uint8_t								all = 2;

	// Apply Timers
	min.finish();
	rng.finish();
	if (min.duration() > 3) all++;

	// Resets every X time (from config)
	// or reset timer if manually done
	if (data->display == Data::None)
		rng.start();
	if (Config::timer > 0 && rng.duration() > Config::timer)
	{
		data->display = Data::None;
		rng.start();
	}

	// Changes to a new set of display and direction
	srand(static_cast<uint32_t>(time(nullptr)));
	if (data->display == Data::None)
	{
		// Set back display to Arrow
		// (will be random aswell once more options are avaible)
		data->display = static_cast<Data::Display>(rand() % all);

		// Resets timer if Void appears to prevent
		// from appearing too often
		if (data->display == Data::Void)
		{
			min.start();
			return;
		}

		// Pick random direction and make sure it never same as current
		Data::Direction newDir = static_cast<Data::Direction>(rand() % 0x4);
		if (data->direction == newDir && newDir + 1 < 0x4)
			data->direction = static_cast<Data::Direction>(newDir + 1);
		else if (data->direction == newDir)
			data->direction = Data::Up;
		else
			data->direction = newDir;
	}
}

/**
 * System's main loop, from where everything
 * will be managed and run
 * 
 * @param	win: Targetted window
 * @param	data: Internal data about the software
 * */
void windowLoop(window_type win, data_type data, assets_type a)
{
	// Wait until everything is ready
	data->trainingRdy = true;
	if (!data->captureRdy)
		std::cout << "Waiting on capture module.." << std::endl;

	// Displaying loop
	sf::Event	e;
	while (win->isOpen())
	{
		// Checks if application is still active
		if (!data->isActive)
		{
			win->close();
			return;
		}

		// Handles events first
		while (win->pollEvent(e))
			eventHandler(win, e, data);
		if (!win->isOpen())
			break;

		// Does a bit of logic in between
		if (data->captureRdy)
			logicHandler(data);

		// Handles drawing second
		win->clear();
		if (data->captureRdy)
			display(win, data, a);
		win->display();
	}
}