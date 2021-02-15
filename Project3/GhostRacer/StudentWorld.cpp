#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath) { return new StudentWorld(assetPath); }

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath), lastAddedWhiteLine(nullptr), m_ghost_racer(nullptr) {}

StudentWorld::~StudentWorld() { cleanUp(); }

void StudentWorld::initialize_lines()
{
	for (int i = 0; i < VIEW_HEIGHT / SPRITE_HEIGHT; i++)
	{
		// Static cast seems to be required to fix a possible issue with overflow when int multiplication is cast to double
		actorList.push_back(new YellowBorderLine(false, i * static_cast<double>(SPRITE_HEIGHT), this));
		actorList.push_back(new YellowBorderLine(true, i * static_cast<double>(SPRITE_HEIGHT), this));
	}

	// Create the two white lines every 4 units
	for (int i = 0; i < (VIEW_HEIGHT / SPRITE_HEIGHT) / 4; i++)
	{
		actorList.push_back(new WhiteBorderLine(false, i * 4.0 * static_cast<double>(SPRITE_HEIGHT), this));
		auto* const white_border_line = new WhiteBorderLine(true, i * 4.0 * static_cast<double>(SPRITE_HEIGHT), this);
		actorList.push_back(white_border_line);
		lastAddedWhiteLine = white_border_line;
	}
}

int StudentWorld::init()
{
	// Create the two yellow border lines on the left and the right
	initialize_lines();

	// Create ghost rider
	auto* ghost_racer = new GhostRacer(this);
	actorList.push_back(ghost_racer);
	m_ghost_racer = ghost_racer;

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::add_new_lines()
{
	const int    newBorderY = VIEW_HEIGHT - SPRITE_HEIGHT;
	const double deltaY     = newBorderY - lastAddedWhiteLine->getY();

	if (deltaY >= SPRITE_HEIGHT)
	{
		actorList.push_back(new YellowBorderLine(false, newBorderY, this));
		actorList.push_back(new YellowBorderLine(true, newBorderY, this));
	}
	if (deltaY >= 4.0 * SPRITE_HEIGHT)
	{
		actorList.push_back(new WhiteBorderLine(false, newBorderY, this));
		auto* const border_line = new WhiteBorderLine(true, newBorderY, this);
		actorList.push_back(border_line);
		lastAddedWhiteLine = border_line;
	}
}

int StudentWorld::move()
{
	// Add new actors
	// Loop over every actor, and doSomething
	// Remove dead actors


	// Add new actors
	{
		add_new_lines();
	}

	// Do the doSomethings
	{
		for (auto actorIterator = actorList.begin(); actorIterator < actorList.end(); ++actorIterator)
		{
			(*actorIterator)->doSomething();
		}
	}

	// Remove dead actors
	{
		for (auto actorIterator = actorList.begin(); actorIterator < actorList.end(); ++actorIterator)
		{
			if (!(*actorIterator)->alive())
			{
				delete (*actorIterator);
				actorIterator = actorList.erase(actorIterator);
			}
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

/// <summary>
/// Called by framework on loss of life or level complete
/// </summary>
void StudentWorld::cleanUp()
{
	for (auto actorIterator = actorList.begin(); actorIterator < actorList.end(); ++actorIterator)
	{
		delete*actorIterator;
	}
}
