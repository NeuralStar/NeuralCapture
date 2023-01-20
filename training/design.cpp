#include "SFML/Graphics/VertexArray.hpp"
#include "config.h"

/**
 * Initializes each triangles vertices
 * of the arrows patterns
 *
 * @param	v: Array of Vertices array to modify
 * */
static void defineTrianglesVertices(sf::VertexArray* const v)
{
	// Define up arrow top
	v[0][0].position = sf::Vector2f(1 / 2.f * config::width, 1 / 8.f * config::height);
	v[0][1].position = sf::Vector2f(1 / 3.f * config::width, 3 / 8.f * config::height);
	v[0][2].position = sf::Vector2f(2 / 3.f * config::width, 3 / 8.f * config::height);

	// Define down arrow top
	v[1][0].position = sf::Vector2f(1 / 2.f * config::width, 7 / 8.f * config::height);
	v[1][1].position = sf::Vector2f(1 / 3.f * config::width, 5 / 8.f * config::height);
	v[1][2].position = sf::Vector2f(2 / 3.f * config::width, 5 / 8.f * config::height);

	// Define left arrow top
	v[2][0].position = sf::Vector2f(1 / 8.f * config::width, 1 / 2.f * config::height);
	v[2][1].position = sf::Vector2f(3 / 8.f * config::width, 1 / 3.f * config::height);
	v[2][2].position = sf::Vector2f(3 / 8.f * config::width, 2 / 3.f * config::height);

	// Define right arrow top
	v[3][0].position = sf::Vector2f(7 / 8.f * config::width, 1 / 2.f * config::height);
	v[3][1].position = sf::Vector2f(5 / 8.f * config::width, 1 / 3.f * config::height);
	v[3][2].position = sf::Vector2f(5 / 8.f * config::width, 2 / 3.f * config::height);
}

/**
 * Initializes the rectangular vertices
 * of the arrows patterns
 *
 * @param	v: Array of Vertices array to modify
 * */
static void defineQuadsVertices(sf::VertexArray* const v)
{
	// Define up arrow body
	v[0][0].position = sf::Vector2f(3 / 7.f * config::width, 3 / 8.f * config::height);
	v[0][1].position = sf::Vector2f(3 / 7.f * config::width, 7 / 8.f * config::height);
	v[0][2].position = sf::Vector2f(4 / 7.f * config::width, 7 / 8.f * config::height);
	v[0][3].position = sf::Vector2f(4 / 7.f * config::width, 3 / 8.f * config::height);

	// Define down arrow body
	v[1][0].position = sf::Vector2f(3 / 7.f * config::width, 5 / 8.f * config::height);
	v[1][1].position = sf::Vector2f(3 / 7.f * config::width, 1 / 8.f * config::height);
	v[1][2].position = sf::Vector2f(4 / 7.f * config::width, 1 / 8.f * config::height);
	v[1][3].position = sf::Vector2f(4 / 7.f * config::width, 5 / 8.f * config::height);

	// Define left arrow body
	v[2][0].position = sf::Vector2f(3 / 8.f * config::width, 3 / 7.f * config::height);
	v[2][1].position = sf::Vector2f(7 / 8.f * config::width, 3 / 7.f * config::height);
	v[2][2].position = sf::Vector2f(7 / 8.f * config::width, 4 / 7.f * config::height);
	v[2][3].position = sf::Vector2f(3 / 8.f * config::width, 4 / 7.f * config::height);

	// Define right arrow body
	v[3][0].position = sf::Vector2f(5 / 8.f * config::width, 3 / 7.f * config::height);
	v[3][1].position = sf::Vector2f(1 / 8.f * config::width, 3 / 7.f * config::height);
	v[3][2].position = sf::Vector2f(1 / 8.f * config::width, 4 / 7.f * config::height);
	v[3][3].position = sf::Vector2f(5 / 8.f * config::width, 4 / 7.f * config::height);
}

/**
 * Pre-render a set of arrows pointing in all four
 * directions: North, South, East, West
 *
 * @return	Array of pre-rendered vertices
 * */
sf::VertexArray* const prepareArrows()
{
	// Prepare vertices
	sf::VertexArray* const	v = new sf::VertexArray[2 * 4];
	if (!v) return nullptr;
	uint8_t			i = 0;

	// Create triangles vertices
	for (; i < 4; i++)
	{
		v[i].setPrimitiveType(sf::Triangles);
		v[i].resize(3);
		for (uint8_t y = 0; y < 3; y++)
			v[i][y].color = sf::Color::White;
	}
	defineTrianglesVertices(v);

	// Create quads vertices
	for (; i < 8; i++)
	{
		v[i].setPrimitiveType(sf::Quads);
		v[i].resize(4);
		for (uint8_t y = 0; y < 4; y++)
			v[i][y].color = sf::Color::White;
	}
	defineQuadsVertices(v + 4);
	return v;
}