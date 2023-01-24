#include "Assets.h"

/* ********************************** */
/*                                    */
/*            CONSTRUCTORS            */
/*                                    */
/* ********************************** */

Assets::Assets():
	arrows(prepareArrows(arrow_count)),
	fonts(prepareFonts(font_count)),
	texts(prepareTexts(text_count))
{}

Assets::~Assets()
{
	if (arrows) delete[] arrows;
	if (texts) delete[] texts;
	if (fonts) delete[] fonts;
}

/* ********************************** */
/*                                    */
/*          PRIVATE FUNCTIONS         */
/*                                    */
/* ********************************** */

/**
 * Initializes each triangles vertices
 * of the arrows patterns
 *
 * @param	v: Array of Vertices array to modify
 * */
static void defineTrianglesVertices(Assets::vertices_type v)
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
static void defineQuadsVertices(Assets::vertices_type v)
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
 * @param	count: Reference to integer where it can store amount of arrows created
 *
 * @return	Array of pre-rendered vertices
 * */
Assets::vertices_type Assets::prepareArrows(size_ref count)
{
	// Prepare vertices
	vertices_type	v = new sf::VertexArray[2 * 4];
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

	// Define counter and returns
	count = 4;
	return v;
}

/**
 * Fetch a bunch of fonts to use for texts
 * 
 * @param	count: Reference to integer where it can store amount of fonts saved
 * 
 * @return	Array of pre-loaded fonts
 * */
Assets::font_type Assets::prepareFonts(size_ref count)
{
	// Prepare array for fonts
	font_type	f = new sf::Font[1];
	if (!f) return nullptr;

	// Load fonts
	if (!f[0].loadFromFile("assets/Mario Kart DS.ttf"))
	{
		delete[] f;
		return nullptr;
	}

	// Define counter and returns
	count = 1;
	return f;
}

/**
 * Creates texts and returns them using first font as default
 * 
 * @param	count: Reference to integer where it can store the amount of texts saved
 * 
 * @return	Array of pre-loaded text
 * */
Assets::text_type Assets::prepareTexts(size_ref count)
{
	// Prevents going further if no fonts exists
	if (!fonts) return nullptr;

	// Prepare array for texts
	text_type	t = new sf::Text[4];
	if (!t) return nullptr;

	// Creates each texts
	t[0].setString("Haut");		// Up
	t[1].setString("Bas");		// Down
	t[2].setString("Gauche");	// Left
	t[3].setString("Droite");	// Right

	// Assign default font and color
	for (uint8_t i = 0; i < 4; i++)
	{
		t[i].setCharacterSize(250);
		t[i].setPosition(0.3f * config::width, 0.3f * config::height);

		t[i].setFont(fonts[0]);
		t[i].setFillColor(sf::Color::White);
	}

	// Define counter and returns
	count = 4;
	return t;
}