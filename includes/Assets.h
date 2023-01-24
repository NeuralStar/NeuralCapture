#pragma once

#include "SFML/Graphics.hpp"
#include "config.h"

class Assets
{
	public:
		typedef sf::VertexArray* const	vertices_type;
		typedef sf::Text* const			text_type;
		typedef sf::Font* const			font_type;
		typedef uint32_t				size_type;
		typedef size_type&				size_ref;

	private:
		vertices_type	prepareArrows(size_ref count);
		font_type		prepareFonts(size_ref count);
		text_type		prepareTexts(size_ref count);

		Assets(const Assets&);

	public:
		// For Arrows displays
		size_type		arrow_count;
		vertices_type	arrows;

		// For Text displays
		size_type		font_count;
		size_type		text_count;
		font_type		fonts;
		text_type		texts;

		Assets();
		~Assets();
};