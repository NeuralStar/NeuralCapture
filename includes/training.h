#pragma once

///  Includes  ///

#include <iostream>
#include <cstdint>
#include <random>

#include "SFML/Graphics.hpp"

#include "Data.h"
#include "config.h"

///  Functions  ///

sf::VertexArray* const	prepareArrows();
void					display(sf::RenderWindow* const win, sf::VertexArray** v, Data* const data);
void					windowLoop(sf::RenderWindow* const win, sf::VertexArray** v, Data* const data);