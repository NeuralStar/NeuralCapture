#include "training.h"

/**
 * Draws out arrows using the preloaded ones
 * 
 * @param	win: Targetted window
 * @param	v: Array of pre-rendered vertices
 * @param	dir: Direction of the display
 * */
static void displayArrow
	(sf::RenderWindow* const win, sf::VertexArray* v, const Data::Direction &dir)
{
	switch (dir)
	{
		case Data::Up:
			win->draw(v[0]);
			win->draw(v[4]);
			break;
		case Data::Down:
			win->draw(v[1]);
			win->draw(v[5]);
			break;
		case Data::Left:
			win->draw(v[2]);
			win->draw(v[6]);
			break;
		case Data::Right:
		default:
			win->draw(v[3]);
			win->draw(v[7]);
			break;
	}
}

/**
 * Function which draws everything on the window
 * (Third Cycle Step)
 *
 * @param	win: Targetted window
 * @param	v: Array of pre-rendered vertexes
 * @param	data: Internal data about the software
 * */
void	display
	(sf::RenderWindow* const win, sf::VertexArray** v, Data* const data)
{
	if (!v) return;
	switch (data->display)
	{
		case Data::Arrow:
			if (!v[Data::Arrow]) return;
			displayArrow(win, v[Data::Arrow], data->direction);
			break;
		default:
			break;
	}
}