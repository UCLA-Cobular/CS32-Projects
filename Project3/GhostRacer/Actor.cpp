#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

constexpr double m_pi = 3.1415926535;

int HumanPedestrian::flipDirection(int angle)
{
	angle += 180;
	if (angle > 360) { return angle % 360; }
	return angle;
}


bool Actor::isOutOfScreen(double x, double y) { return x < 0 || y < 0 || x > VIEW_WIDTH || y > VIEW_HEIGHT; }


/// <summary>
/// Moves the player and will return true if the object was killed for being offscreen. This is to be used to cascade back up after.
/// </summary>
/// <returns>True if the object is dead, false if it's still alive</returns>
bool MovingActor::move()
{
	moveTo(getX() + h_speed(), getY() + v_speed());
	if (Actor::isOutOfScreen(getX(), getY())) { return true; }
	return false;
}

double MovingActor::v_speed() const { return m_vSpeed - (*(*studentWorld()).ghost_racer()).racer_speed(); }


#pragma region Goodies
void Goodie::doSomething()
{
	// Try to move, break out if it's killed
	if (move())
	{
		set_alive(false);
		return;
	}
	doSomethingSpecific();
	if (collidedWithPlayer())
	{
		playSound(m_sound);
		handlePlayerCollision();
	}
}

void HealingGoodie::handlePlayerCollision()
{
	ghostRacer()->getHealed(10);
	studentWorld()->increaseScore(250);
	set_alive(false);
}

void HolyWaterGoodie::handlePlayerCollision()
{
	ghostRacer()->addHolyWater(10);
	studentWorld()->increaseScore(50);
	set_alive(false);
}

void SoulGoodie::doSomethingSpecific() { setDirection(getDirection() + 10); }

void SoulGoodie::handlePlayerCollision()
{
	studentWorld()->saveSoul();
	studentWorld()->increaseScore(100);
	set_alive(false);
}

void OilSlick::handlePlayerCollision() { ghostRacer()->spinOut(); }

void HolyWaterProjectile::doSomething()
{
	if (!alive()) { return; }

	// Check for activation
	Actor* hitActor = studentWorld()->projectileCollision(getX(), getY(), getRadius());
	if (hitActor != nullptr)
	{
		hitActor->doInteractWithProjectile(1);
		set_alive(false);
		return;
	}
	else
	{
		moveForward(SPRITE_HEIGHT);
		m_travel_distance += SPRITE_HEIGHT;
	}

	if (Actor::isOutOfScreen(getX(), getY()) || m_travel_distance >= m_max_travel_distance) { set_alive(false); }
}

/// <summary>
/// Checks if this object overlaps with the player
/// </summary>
/// <returns></returns>
bool CollidesWithPlayer::collidedWithPlayer() const
{
	const double delta_x  = abs(ghostRacer()->getX() - this->getX());
	const double delta_y  = abs(ghostRacer()->getY() - this->getY());
	const double radiuses = ghostRacer()->getRadius() + this->getRadius();
	if (delta_x < radiuses * 0.25 && delta_y < radiuses * 0.6) { return true; }
	return false;
}

#pragma endregion


#pragma region HealthActor
void HasHealthActor::set_health(const int m_health)
{
	this->m_health = m_health;
	if (m_health < 0) { set_alive(false); }
}

void HasHealthActor::change_health(const int delta_health)
{
	m_health += delta_health;
	if (m_health < 0) { set_alive(false); }
}

#pragma endregion


#pragma region MovementActors
void HumanPedestrian::doSomething()
{
	if (!alive()) { return; }
	if (collidedWithPlayer())
	{
		// Marks the level for deletion, then exits so the next thing ran is the check for this mark
		studentWorld()->shortCircuitEndLevel();
		return;
	}
	if (move())
	{
		set_alive(false);
		return;
	}
	decrement_move_plan_dist();
	if (move_plan_dist() > 0) { return; }
	createNewMovePlan();
}

void HumanPedestrian::doInteractWithProjectile(int damage)
{
	set_h_speed(-1 * h_speed());
	setDirection(flipDirection(getDirection()));
	playSound(SOUND_PED_HURT);
}


void ZombiePedestrian::doSomething()
{
	if (!alive()) { return; }
	if (collidedWithPlayer())
	{
		ghostRacer()->hurtGhostRacer(5);
		doInteractWithProjectile(2);
		return;
	}

	// Within 30 X units and above the player
	const double delta_x = ghostRacer()->getX() - getX();
	if (abs(delta_x) < 30 && getY() > ghostRacer()->getY())
	{
		setDirection(270);
		if (delta_x < 0) { set_h_speed(-1); }
		else if (delta_x > 0) { set_h_speed(+1); }
		else { set_h_speed(0); }
		m_time_until_grunt--;
		if (m_time_until_grunt <= 0)
		{
			playSound(SOUND_ZOMBIE_ATTACK);
			m_time_until_grunt = 20;
		}
	}

	if (move())
	{
		set_alive(false);
		return;
	}

	if (move_plan_dist() > 0)
	{
		decrement_move_plan_dist();
		return;
	}
	createNewMovePlan();
}

void ZombiePedestrian::doInteractWithProjectile(int damage)
{
	change_health(-damage);
	if (get_health() <= 0)
	{
		set_alive(false);
		playSound(SOUND_PED_DIE);
		if (!collidedWithPlayer()) { studentWorld()->addHealthPack(getX(), getY()); }
		studentWorld()->increaseScore(150);
	}
	else { playSound(SOUND_PED_HURT); }
}

void ZombieCab::doSomething()
{
	if (!alive()) { return; }

	// Step 2
	// If collided with player already, skip to step 3 otherwise do this step
	if (collidedWithPlayer() && !hasDamagedGhostRacer)
	{
		playSound(SOUND_VEHICLE_CRASH);
		ghostRacer()->hurtGhostRacer(20); // TODO: ensure this is handled correctly
		if (getX() <= ghostRacer()->getX())
		{
			set_h_speed(-5);
			setDirection(120 + randInt(0, 19)); // Written as [0,20), so actually 0-19 I think
		}
		else
		{
			set_h_speed(5);
			setDirection(60 - randInt(0, 19)); // Written as [0,20), so actually 0-19 I think
		}
		hasDamagedGhostRacer = true;
	}
	if (move())
	{
		set_alive(false);
		return;
	}

	// Handle speed changes for other objects
	// Cab is faster than racer
	if (v_speed() - studentWorld()->ghost_racer()->racer_speed() > 0 && StudentWorld::coordToLane(getX()) != -1)
	{
		// And there is an actor in the cab's lane in front of cab and said actor is closer than 96 pixels in front
		const double next_actor_dist = studentWorld()->collisionActorInLane(
			StudentWorld::coordToLane(getX()), getY(), false);
		if (next_actor_dist > 0 && next_actor_dist < 96)
		{
			// Decrement speed
			set_v_speed(v_speed() - 0.5);
		}
	}

	// Cab is same or slower than racer
	if (v_speed() - studentWorld()->ghost_racer()->racer_speed() < 0 && StudentWorld::coordToLane(getX()) != -1)
	{
		// And there is an actor in the cab's lane behind the cab and said actor is closer than 96 pixels
		const double prev_actor_dist = studentWorld()->collisionActorInLane(
			StudentWorld::coordToLane(getX()), getY(), true);
		if (prev_actor_dist > 0 && prev_actor_dist < 96)
		{
			// Increment speed
			set_v_speed(v_speed() + 0.5);
		}
	}

	decrement_move_plan_dist();
	if (move_plan_dist() > 0) { return; }
	set_move_plan_dist(randInt(4, 32));
	set_v_speed(v_speed() + randInt(-2, 2));
}

void ZombieCab::doInteractWithProjectile(int damage)
{
	change_health(-damage);
	if (get_health() <= 0)
	{
		set_alive(false);
		playSound(SOUND_VEHICLE_DIE);
		studentWorld()->add_oil_slick(getX(), getY());
		studentWorld()->increaseScore(200);
		return;
	}
	playSound(SOUND_PED_HURT);
}

void MovementPlanActor::createNewMovePlan()
{
	// Generate number between -3 and +2, if number >= 0, add one to get {-3,-2,-1,1,2,3}
	int num = randInt(-3, 2);
	if (num >= 0) { num++; }
	set_h_speed(num);

	set_move_plan_dist(randInt(4, 32));
	if (num < 0) { setDirection(180); }
	else { setDirection(0); }
}

#pragma endregion

#pragma region GhostRacer

void GhostRacer::doSomething()
{
	if (get_health() < 0 || !alive()) { return; }

	// Off the left side
	if (getX() < ROAD_CENTER - ROAD_WIDTH / 2.0)
	{
		if (getDirection() > 90)
		{
			hurtGhostRacer(10);
			setDirection(82);
			playSound(SOUND_VEHICLE_CRASH);
		}
	}
		// Off the right side
	else if (getX() > ROAD_CENTER + ROAD_WIDTH / 2.0)
	{
		if (getDirection() < 90)
		{
			hurtGhostRacer(10);
			setDirection(98);
			playSound(SOUND_VEHICLE_CRASH);
		}
	}
		// In the middle, allow a move
	else
	{
		// step 4 (skipped by 2 or 3 happening
		int key = -1;
		if (studentWorld()->getKey(key))
		{
			switch (key)
			{
			case KEY_PRESS_SPACE:
				fireHolyWater();
				break;
			case KEY_PRESS_LEFT:
				if (getDirection() < 114) { setDirection(getDirection() + 8); }
				break;
			case KEY_PRESS_RIGHT:
				if (getDirection() > 66) { setDirection(getDirection() - 8); }
				break;
			case KEY_PRESS_UP:
				if (racer_speed() < 5) { set_racer_speed(racer_speed() + 1); }
				break;
			case KEY_PRESS_DOWN:
				if (racer_speed() > -1) { set_racer_speed(racer_speed() - 1); }
				break;
			default:
				std::cerr << "Unexpected key input!" << std::endl;
			}
		}
	}

	// Step 5
	move();
}

void GhostRacer::move()
{
	const double direction = getDirection();
	const double delta_x   = cos(direction * m_pi / 180) * 4.0;
	moveDelta(delta_x, 0.0);
}

void GhostRacer::hurtGhostRacer(int damage)
{
	change_health(-damage);
	if (get_health() <= 0)
	{
		set_alive(false);
		playSound(SOUND_PLAYER_DIE);
	}
}

void GhostRacer::spinOut()
{
	const int sign  = randInt(0, 1) ? -1 : 1;
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

void GhostRacer::fireHolyWater()
{
	if (holy_water() > 0)
	{
		// Angle with 0 forward, positive left, negative right
		const double theta = -(getDirection() - 90.0);

		const double deltaX = sin(theta * m_pi / 180) * SPRITE_HEIGHT;
		const double spawnX = getX() + deltaX;

		const double deltaY = cos(theta * m_pi / 180) * SPRITE_HEIGHT;
		const double spawnY = getY() + deltaY;
		std::cerr << spawnX << "|" << spawnY << std::endl;
		studentWorld()->addHolyWaterSpray(spawnX, spawnY, getDirection());
		playSound(SOUND_PLAYER_SPRAY);
		m_holy_water--;
	}
}
#pragma endregion


#pragma region BorderLines
void BorderLine::doSomething()
{
	// Do the delete thing, returning right away if the move fails
	if (move())
	{
		set_alive(false);
		return;
	}
}

YellowBorderLine::YellowBorderLine(bool left, double startY, StudentWorld* game_world)
	: BorderLine(IID_YELLOW_BORDER_LINE, start_x(left), startY, game_world) {}

double YellowBorderLine::start_x(bool left)
{
	if (left) { return ROAD_CENTER - ROAD_WIDTH / 2.0; }
	return ROAD_CENTER + ROAD_WIDTH / 2.0;
}

WhiteBorderLine::WhiteBorderLine(bool left, double startY, StudentWorld* game_world)
	: BorderLine(IID_WHITE_BORDER_LINE, start_x(left), startY, game_world) {}

double WhiteBorderLine::start_x(bool left)
{
	if (left) { return (ROAD_CENTER - ROAD_WIDTH / 2.0) + ROAD_WIDTH / 3.0; }
	return (ROAD_CENTER + ROAD_WIDTH / 2.0) - ROAD_WIDTH / 3.0;
}
#pragma endregion
