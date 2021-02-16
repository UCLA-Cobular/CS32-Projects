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

	void          moveDelta(double x, double y) { moveTo(getX() + x, getY() + y); }
	bool          alive() const { return m_alive; }
	void          setAlive(bool alive) { m_alive = alive; }
	int static    flipDirection(int angle);
	void          playSound(int sound) const { m_game_world->playSound(sound); }
	GhostRacer*   ghostRacer() const { return m_game_world->ghost_racer(); }
	StudentWorld* studentWorld() const { return m_game_world; }

	// Property flags for different things. Set to true in a class if these are true for that class, and other functions will check these to see what to do.
	bool virtual collisionAvoidanceWorthy() const { return false; }
	bool virtual canBeHealed() const { return false; }
	bool virtual hasHealth() const { return false; }
	bool virtual canBeSpun() const { return false; }
	bool virtual canInteractWithProjectiles() const { return false; }

	// If canInteractWithProjectiles is true, then this function can be called when hit by a projectile. 
	void virtual doInteractWithProjectile(int damage = 0) {}

	void virtual doSomething() = 0;
private:
	StudentWorld* m_game_world;
	bool          m_alive;
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

#pragma region Goodies
class Goodie : public CollidesWithPlayer
{
public:
	Goodie(
		int ImageID, double startX, double startY, int size, StudentWorld* game_world, int sound = SOUND_GOT_GOODIE,
		int depth                                                                                = 2)
		: CollidesWithPlayer(ImageID, startX, startY, 0, size, depth, -4, 0, game_world), m_sound(sound) {}

	void         doSomething() override;
	virtual void doSomethingSpecific() {}
	virtual void handlePlayerCollision() = 0;
private:
	int m_sound;
};


class HealingGoodie : public Goodie
{
public:
	HealingGoodie(double startX, double startY, StudentWorld* game_world)
		: Goodie(IID_HEAL_GOODIE, startX, startY, 1, game_world) {}

	bool canInteractWithProjectiles() const override { return true; }
	void handlePlayerCollision() override;
};


class HolyWaterGoodie : public Goodie
{
public:
	HolyWaterGoodie(double startX, double startY, StudentWorld* game_world)
		: Goodie(IID_HOLY_WATER_GOODIE, startX, startY, 2, game_world) {}

	bool canInteractWithProjectiles() const override { return true; }
	void handlePlayerCollision() override;
};


class SoulGoodie : public Goodie
{
public:
	SoulGoodie(double startX, double startY, StudentWorld* game_world)
		: Goodie(IID_SOUL_GOODIE, startX, startY, 4, game_world, SOUND_GOT_SOUL) {}

	void doSomethingSpecific() override;
	void handlePlayerCollision() override;
};


// Depth 1
class OilSlick : public Goodie
{
public:
	OilSlick(double startX, double startY, StudentWorld* game_world)
		: Goodie(IID_OIL_SLICK, startX, startY, randInt(2, 5), game_world, SOUND_OIL_SLICK, 1) {}

	void handlePlayerCollision() override;
};

#pragma endregion


class HasHealthActor : public CollidesWithPlayer
{
public:
	HasHealthActor(
		int ImageID, double startX, double startY, int dir, int size, int depth, int m_vSpeed, int m_hSpeed,
		int starting_health, StudentWorld* game_world)
		: CollidesWithPlayer(ImageID, startX, startY, dir, size, depth, m_vSpeed, m_hSpeed, game_world),
		  m_health(starting_health) { }

	int  get_health() const { return m_health; }
	void set_health(const int m_health);
	void change_health(const int delta_health);

private:
	int m_health;
};


class MovementPlanActor : public HasHealthActor
{
public:
	MovementPlanActor(
		int ImageID, double      startX, double startY, int dir, int size, int depth, int m_vSpeed, int m_hSpeed,
		int starting_health, int move_plan_dist, StudentWorld* game_world)
		: HasHealthActor(ImageID, startX, startY, dir, size, depth, m_vSpeed, m_hSpeed, starting_health, game_world),
		  m_move_plan_dist(move_plan_dist) {}

	int  move_plan_dist() const { return m_move_plan_dist; }
	void decrement_move_plan_dist() { m_move_plan_dist--; }
	void set_move_plan_dist(int dist) { m_move_plan_dist = dist; }
	void createNewMovePlan();
private:
	int m_move_plan_dist;
};


class HumanPedestrian : public MovementPlanActor
{
public:
	HumanPedestrian(double startX, double startY, StudentWorld* game_world)
		: MovementPlanActor(IID_HUMAN_PED, startX, startY, 0, 2, 0, -4, 0, 2, 0, game_world) {}

	void doSomething() override;
	bool canInteractWithProjectiles() const override { return true; }
	void doInteractWithProjectile(int damage = 0) override;
};


class ZombiePedestrian : public MovementPlanActor
{
public:
	ZombiePedestrian(double startX, double startY, StudentWorld* game_world)
		: MovementPlanActor(IID_ZOMBIE_PED, startX, startY, 0, 3, 0, -4, 0, 2, 0, game_world), m_time_until_grunt(0) {}

	void doSomething() override;
	bool canInteractWithProjectiles() const override { return true; }
	void doInteractWithProjectile(int damage = 0) override;

private:
	int m_time_until_grunt;
};


class ZombieCab : public MovementPlanActor
{
public:
	ZombieCab(double startX, double startY, StudentWorld* game_world)
		: MovementPlanActor(IID_ZOMBIE_CAB, startX, startY, 9, 4, 0, 0, 0, 3, 0, game_world), hasDamagedGhostRacer(false) {}

	bool collisionAvoidanceWorthy() const override { return true; }
private:
	bool hasDamagedGhostRacer;
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

	bool canBeSpun() const override { return true; }
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
