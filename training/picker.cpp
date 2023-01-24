#include "training.h"

/**
 * Writes down on screen plain text
 * 
 * @param	win: Targetted window
 * @param	dir: Direction of the displayable
 * */
static void displayText(window_type win, Assets::text_type t, const Data::Direction& dir)
{
	switch (dir)
	{
		case Data::Up:
			break;
		case Data::Down:
			break;
		case Data::Left:
			break;
		case Data::Right:
		default:
			break;
	}
}

/**
 * Function which draws everything on the window
 * (Third Cycle Step)
 *
 * @param	win: Targetted window
 * @param	data: Internal data about the software
 * @param	assets: Storage structure containing assets to draw
 * */
void display(window_type win, data_type d, assets_type a)
{
	if (!a) return;
	switch (d->display)
	{
		case Data::Arrow:
			if (!a->arrows) return;
			win->draw(a->arrows[d->direction]);						// Draw Tip
			win->draw(a->arrows[d->direction + a->arrow_count]);	// Draw Stem
			break;
		case Data::Text:
			if (!a->texts) return;
			win->draw(a->texts[d->direction]);						// Draw text
			break;
		default:
			break;
	}
}