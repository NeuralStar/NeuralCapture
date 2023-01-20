#include "training.h"
#include <random>

/**
 * Takes the input and alters data to performs
 * the tasks related to the given inputs
 * 
 * @param	win: Targetted window
 * @param	key: Key input received
 * */
static void keyHandler
	(sf::RenderWindow* const win, const sf::Event::KeyEvent &key, Data* const data)
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
	(sf::RenderWindow* const win, const sf::Event &event, Data* const data)
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
static void logicHandler(Data* const data)
{
	srand(static_cast<uint32_t>(time(nullptr)));
	if (data->display == Data::None)
	{
		// Set back display to Arrow
		// (will be random aswell once more options are avaible)
		data->display = Data::Arrow;

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
void windowLoop
	(sf::RenderWindow* const win, sf::VertexArray** v, Data* const data)
{
	sf::Event	e;
	while (win->isOpen())
	{
		// Checks if application is still active
		if (!data->isActive)
			win->close();

		// Handles events first
		while (win->pollEvent(e))
			eventHandler(win, e, data);
		if (!win->isOpen())
			break;

		// Does a bit of logic in between
		logicHandler(data);

		// Handles drawing second
		win->clear();
		display(win, v, data);
		win->display();
	}
}