#pragma once

class Data
{
	private:
		Data(const Data&);

	public:
		enum Direction
		{
			Up = 0x0,
			Down = 0x1,
			Left = 0x2,
			Right = 0x3,
		};

		enum Display
		{
			None = -1,
			Arrow = 0x0,
			Text = 0x1,
		};

		bool		isActive;

		Direction	direction;
		Display		display;

		inline Data() :
			isActive(true), direction(Direction::Up),
			display(Display::None) {};
		inline ~Data() {};
};