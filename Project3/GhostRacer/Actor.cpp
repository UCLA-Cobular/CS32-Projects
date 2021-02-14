#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


void MovingActor::move()
{
	moveTo(getX() + h_speed(), getY() + v_speed());
}


#pragma region BorderLines
void BorderLine::doSomething()
{
	// Move according to speed and stuff
	move();

	// Flag the object for deletion if it's off the screen
	if (getX() < 0 || getY() < 0)
	{
		set_alive(false);
		return;
	}
}

YellowBorderLine::YellowBorderLine(bool left)
	: BorderLine(IID_YELLOW_BORDER_LINE, left ? ROAD_CENTER - ROAD_WIDTH / 2 : ROAD_CENTER + ROAD_WIDTH / 2) {}

YellowBorderLine::YellowBorderLine(bool left, double startY)
	: BorderLine(IID_YELLOW_BORDER_LINE, left ? ROAD_CENTER - ROAD_WIDTH / 2 : ROAD_CENTER + ROAD_WIDTH / 2, startY) {}

WhiteBorderLine::WhiteBorderLine(bool left)
	: BorderLine(IID_WHITE_BORDER_LINE,
	             left
		             ? (ROAD_CENTER - ROAD_WIDTH / 2) + ROAD_WIDTH / 3
		             : (ROAD_CENTER + ROAD_WIDTH / 2) - ROAD_WIDTH / 3) {}

WhiteBorderLine::WhiteBorderLine(bool left, double startY)
	: BorderLine(IID_WHITE_BORDER_LINE,
	             left
		             ? (ROAD_CENTER - ROAD_WIDTH / 2) + ROAD_WIDTH / 3
		             : (ROAD_CENTER + ROAD_WIDTH / 2) - ROAD_WIDTH / 3, startY) {}
#pragma endregion
