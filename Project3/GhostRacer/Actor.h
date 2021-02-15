#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"

class StudentWorld;
class GhostRacer;

class Actor : public GraphObject
{
protected:
	Actor(int ImageID, double startX, double startY, int dir, int size, int depth, StudentWorld* game_world)
		: GraphObject(ImageID, startX, startY, dir, size, depth), m_game_world(game_world), m_alive(true) { }

public:

	void moveDelta(double x, double y) { moveTo(getX() + x, getY() + y); }
	bool alive() const { return m_alive; }
	void set_alive(bool alive) { m_alive = alive; }

	bool virtual collisionAvoidanceWorthy() const { return false; }
	bool virtual canBeHealed() const { return false; }
	bool virtual hasHealth() const { return false; }
	bool virtual canBeSpun() const { return false; }
	bool virtual interactWithProjectiles() const { return false; }

	void virtual doSomething() = 0;
protected:
	StudentWorld* m_game_world;
private:
	bool m_alive;
};


class MovingActor : public Actor
{
public:
	// REMEMBER: Y IS VERT, X IS HORIZ!!!!!!
	MovingActor(int ImageID, double startX, double startY, int dir, int size, int depth, int m_vSpeed, int m_hSpeed, StudentWorld* game_world)
		: Actor(ImageID, startX, startY, dir, size, depth, game_world), m_vSpeed(m_vSpeed), m_hSpeed(m_hSpeed) { }

	virtual void move();

	int  v_speed() const;
	void set_v_speed(const int m_v_speed) { m_vSpeed = m_v_speed; }
	int  h_speed() const { return m_hSpeed; }
	void set_h_speed(const int m_h_speed) { m_hSpeed = m_h_speed; }

private:
	int m_vSpeed;
	int m_hSpeed;
};


class HealthActor : public MovingActor
{
public:
	HealthActor(
		int ImageID, double startX, double startY, int dir, int size, int depth, int m_vSpeed, int m_hSpeed,
		int StartingHealth, StudentWorld* game_world)
		: MovingActor(ImageID, startX, startY, dir, size, depth, m_vSpeed, m_hSpeed, game_world)
	{
		this->m_health = StartingHealth;
	}

	int  get_health() const { return m_health; }
	void set_health(const int m_health);
	void change_health(const int delta_health);

private:
	int m_health;
};


class GhostRacer : public HealthActor
{
public:
	GhostRacer(StudentWorld* game_world) : HealthActor(IID_GHOST_RACER, 128, 32, 90, 4, 0, 0, 0, 100, game_world), m_holy_water(10), m_racer_speed(0) {}
	void doSomething() override;
	void move() override;
	void hurtGhostRacer(int damage);
	void spinOut();
	void getHealed(int heal_amount);
	void addHolyWater(int amount);
	void fireHolyWater();

	int  racer_speed() const { return m_racer_speed; }
	void set_racer_speed(const int racer_speed) { this->m_racer_speed = racer_speed; }

	bool collisionAvoidanceWorthy() const override { return true; }
private:
	int m_holy_water;
	int m_racer_speed;
};

#pragma region BorderLines
class BorderLine : public MovingActor
{
public:
	BorderLine(int ImageID, double startX, StudentWorld* game_world) : MovingActor(ImageID, startX, 0, 0, 2, 1, -4, 0, game_world) { }


	BorderLine(int ImageID, double startX, double startY, StudentWorld* game_world) : MovingActor(ImageID, startX, startY, 0, 2, 1, -4, 0, game_world) { }

	bool collisionAvoidanceWorthy() const override { return true; }
	void doSomething() override;
};


class YellowBorderLine : public BorderLine
{
public:
	YellowBorderLine(bool left, StudentWorld* game_world);
	YellowBorderLine(bool left, double startY, StudentWorld* game_world);
private:
	static double start_x(bool left);
};


class WhiteBorderLine : public BorderLine
{
public:
	WhiteBorderLine(bool left, StudentWorld* game_world);
	WhiteBorderLine(bool left, double startY, StudentWorld* game_world);

private:
	static double start_x(bool left);
};
#pragma endregion

#endif // ACTOR_H_
