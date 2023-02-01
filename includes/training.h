#pragma once

///  Includes  ///

#include <iostream>
#include <cstdint>
#include <random>

#include "Data.h"
#include "Assets.h"
#include "Timer.h"
#include "Config.h"

///  Definitions  ///

typedef sf::RenderWindow* const		window_type;
typedef Data* const					data_type;
typedef Assets* const				assets_type;

///  Functions  ///

void		display(window_type win, data_type data, assets_type a);
void		windowLoop(window_type win, data_type data, assets_type a);