#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

constexpr double m_pi = 3.1415926535;

void MovingActor::move() { moveTo(getX() + h_speed(), getY() + v_speed()); }
int  MovingActor::v_speed() const { return m_vSpeed - (*(*m_game_world).ghost_racer()).racer_speed(); }

#pragma region HealthActor
void HealthActor::set_health(const int m_health)
{
	this->m_health = m_health;
	if (m_health < 0) { set_alive(false); }
}

void HealthActor::change_health(const int delta_health)
{
	m_health += delta_health;
	if (m_health < 0) { set_alive(false); }
}

#pragma endregion


#pragma region GhostRacer
void GhostRacer::doSomething()
{
	if (get_health() < 0 || !alive()) { return; }

	if (false)
	{
		// step 2
	}
	else if (false)
	{
		// step 3
	}
	else
	{
		// step 4 (skipped by 2 or 3 happening
		int key = -1;
		if (m_game_world->getKey(key))
		{
			switch (key)
			{
			case KEY_PRESS_SPACE: fireHolyWater();
				break;
			case KEY_PRESS_LEFT: if (getDirection() < 114) { setDirection(getDirection() + 8); }
							   break;
			case KEY_PRESS_RIGHT: if (getDirection() > 66) { setDirection(getDirection() - 8); }
								break;
			case KEY_PRESS_UP: if (racer_speed() < 5) { set_racer_speed(racer_speed() + 1); }
							 break;
			case KEY_PRESS_DOWN: if (racer_speed() > -1) { set_racer_speed(racer_speed() - 1); }
							   break;
			default: std::cerr << "Unexpected key input!" << std::endl;
			}
		}
	}

	// Step 5
	move();
}

void GhostRacer::move()
{
	const double direction = getDirection();
	const double delta_x = cos(direction * m_pi / 180) * 4.0;
	moveDelta(delta_x, 0.0);
}

void GhostRacer::hurtGhostRacer(int damage)
{
	change_health(-damage);
	if (get_health() > 0)
	{
		// TODO: Find SOUND_PLAYER_HURT to put here instead
		m_game_world->playSound(SOUND_PLAYER_DIE);
	}
	else
	{
		set_alive(false);
		m_game_world->playSound(SOUND_PLAYER_DIE);
	}
}

void GhostRacer::spinOut()
{
	const int sign = randInt(0, 1) ? -1 : 1;
	const int angle = sign * randInt(5, 20);

	// Make sure the angle stays within the acceptable range
	if (getDirection() + angle < 60) { setDirection(60); }
	else if (getDirection() + angle > 120) { setDirection(120); }
	else { setDirection(getDirection() + angle); }

	// Check if there was an issue with that above logic and scream if so
	if (getDirection() < 60 || getDirection() > 120)
	{
		std::cerr << "Something has gone wrong with oil slick angles" << std::endl;
	}
}

void GhostRacer::getHealed(int heal_amount)
{
	if (get_health() + heal_amount > 100) { set_health(100); }
	else { change_health(heal_amount); }
}

void GhostRacer::addHolyWater(int amount) { m_holy_water += amount; }

// TODO: Implement holy water firing
void GhostRacer::fireHolyWater()
{
	// See page 30
	m_game_world->playSound(SOUND_PLAYER_SPRAY);
	m_holy_water--;
}
#pragma endregion


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

YellowBorderLine::YellowBorderLine(bool left, StudentWorld* game_world)
	: BorderLine(IID_YELLOW_BORDER_LINE, start_x(left), game_world) {}

YellowBorderLine::YellowBorderLine(bool left, double startY, StudentWorld* game_world)
	: BorderLine(IID_YELLOW_BORDER_LINE, start_x(left), startY, game_world) {}

double YellowBorderLine::start_x(bool left)
{
	if (left) { return ROAD_CENTER - ROAD_WIDTH / 2.0; }
	return ROAD_CENTER + ROAD_WIDTH / 2.0;
}

WhiteBorderLine::WhiteBorderLine(bool left, StudentWorld* game_world)
	: BorderLine(IID_WHITE_BORDER_LINE, start_x(left), game_world) {}

WhiteBorderLine::WhiteBorderLine(bool left, double startY, StudentWorld* game_world)
	: BorderLine(IID_WHITE_BORDER_LINE, start_x(left), startY, game_world) {}

double WhiteBorderLine::start_x(bool left)
{
	if (left) { return (ROAD_CENTER - ROAD_WIDTH / 2.0) + ROAD_WIDTH / 3.0; }
	return (ROAD_CENTER + ROAD_WIDTH / 2.0) - ROAD_WIDTH / 3.0;
}
#pragma endregion
