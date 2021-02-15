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
	MovingActor(
		int           ImageID, double startX, double startY, int dir, int size, int depth, int m_vSpeed, int m_hSpeed,
		StudentWorld* game_world)
		: Actor(ImageID, startX, startY, dir, size, depth, game_world), m_vSpeed(m_vSpeed), m_hSpeed(m_hSpeed) { }

	virtual bool move();

	int  v_speed() const;
	void set_v_speed(const int m_v_speed) { m_vSpeed = m_v_speed; }
	int  h_speed() const { return m_hSpeed; }
	void set_h_speed(const int m_h_speed) { m_hSpeed = m_h_speed; }

private:
	int m_vSpeed;
	int m_hSpeed;
};


class CollidesWithPlayer : public MovingActor
{
public:
	CollidesWithPlayer(
		int           ImageID, double startX, double startY, int dir, int size, int depth, int m_vSpeed, int m_hSpeed,
		StudentWorld* game_world)
		: MovingActor(ImageID, startX, startY, dir, size, depth, m_vSpeed, m_hSpeed, game_world) {}

	bool collidedWithPlayer() const;
};


class Goodie : public CollidesWithPlayer
{
public:
	Goodie(int ImageID, double startX, double startY, int size, StudentWorld* game_world, int depth = 2)
		: CollidesWithPlayer(ImageID, startX, startY, 0, size, depth, -4, 0, game_world) {}

	void         doSomething() override;
	virtual void doSomethingSpecific() = 0;
	virtual void handlePlayerCollision() = 0;
};


class HealingGoodie : public Goodie
{
public:
	HealingGoodie(double startX, double startY, StudentWorld* game_world)
		: Goodie(IID_HEAL_GOODIE, startX, startY, 1, game_world) {}

	bool interactWithProjectiles() const override { return true; }
	void doSomethingSpecific() override;
	void handlePlayerCollision() override;
};


class HolyWaterGoodie : public Goodie
{
public:
	HolyWaterGoodie(double startX, double startY, StudentWorld* game_world)
		: Goodie(IID_HOLY_WATER_GOODIE, startX, startY, 2, game_world) {}

	bool interactWithProjectiles() const override { return true; }
	void doSomethingSpecific() override;
	void handlePlayerCollision() override;
};


class SoulGoodie : public Goodie
{
public:
	SoulGoodie(double startX, double startY, StudentWorld* game_world)
		: Goodie(IID_SOUL_GOODIE, startX, startY, 4, game_world) {}

	void doSomethingSpecific() override;
	void handlePlayerCollision() override;
};


// Depth 1
class OilSlick : public Goodie
{
public:
	OilSlick(double startX, double startY, StudentWorld* game_world)
		: Goodie(IID_SOUL_GOODIE, startX, startY, randInt(2, 5), game_world, 1) {}

	void doSomethingSpecific() override;
	void handlePlayerCollision() override;
};


class HealthActor : public CollidesWithPlayer
{
public:
	HealthActor(
		int ImageID, double startX, double startY, int dir, int size, int depth, int m_vSpeed, int m_hSpeed,
		int StartingHealth, StudentWorld* game_world)
		: CollidesWithPlayer(ImageID, startX, startY, dir, size, depth, m_vSpeed, m_hSpeed, game_world)
	{
		this->m_health = StartingHealth;
	}

	int  get_health() const { return m_health; }
	void set_health(const int m_health);
	void change_health(const int delta_health);

private:
	int m_health;
};


// Note: this class re-implements some of the features of HealthAgent.
// This is because I could not find a way to make this work without multiple inheritance and then the diamond problem and such.
// I have spent **hours** trying to get virtual inheritance and stuff working, it came down to requiring a default and lazy initialized constructor on GraphObject, which I can't add
// The compromise is a slightly different health system here on this class, and most other things with health will share their code. Good enough for me. 
class GhostRacer : public Actor
{
public:
	GhostRacer(StudentWorld* game_world)
		: Actor(IID_GHOST_RACER, 128, 32, 90, 4, 0, game_world), m_holy_water(10), m_racer_speed(0), m_health(100) {}

	void doSomething() override;
	void move();
	void hurtGhostRacer(int damage);
	void spinOut();
	void getHealed(int heal_amount);
	void addHolyWater(int amount);
	void fireHolyWater();
	void change_health(int health) { m_health += health; }
	void set_health(int health) { m_health = health; }

	int  get_health() const { return m_health; }
	int  holy_water() const { return m_holy_water; }
	int  racer_speed() const { return m_racer_speed; }
	void set_racer_speed(const int racer_speed) { this->m_racer_speed = racer_speed; }

	bool collisionAvoidanceWorthy() const override { return true; }
private:
	int m_holy_water;
	int m_racer_speed;
	int m_health;
};

#pragma region BorderLines
class BorderLine : public MovingActor
{
public:
	BorderLine(int ImageID, double startX, StudentWorld* game_world)
		: MovingActor(ImageID, startX, 0, 0, 2, 1, -4, 0, game_world) { }


	BorderLine(int ImageID, double startX, double startY, StudentWorld* game_world)
		: MovingActor(ImageID, startX, startY, 0, 2, 1, -4, 0, game_world) { }

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
