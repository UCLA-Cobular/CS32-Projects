#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int ImageID, double startX, double startY, int dir, int size, int depth)
		: GraphObject(ImageID, startX, startY, dir, size, depth) { }

	void moveDelta(int x, int y) { moveTo(getX() + x, getY() + y); }
	bool alive() const { return m_alive; }
	void set_alive(bool alive) { m_alive = alive; }

	bool virtual collisionAvoidanceWorthy() const { return false; }
	bool virtual canBeHealed() const { return false; }
	bool virtual canBeSpun() const { return false; }
	bool virtual interactWithProjectiles() const { return true; }

	void virtual doSomething() = 0;
private:
	bool m_alive = true;
};


// TODO: Need to factor in Ghost Racer's speed here
class MovingActor : public Actor
{
public:
	// REMEMBER: Y IS VERT, X IS HORIZ!!!!!!
	MovingActor(int ImageID, double startX, double startY, int dir, int size, int depth, int m_vSpeed, int m_hSpeed)
		: Actor(ImageID, startX, startY, dir, size, depth), m_vSpeed(m_vSpeed), m_hSpeed(m_hSpeed) { }

	void move();

	int  v_speed() const { return m_vSpeed; }
	void set_v_speed(const int m_v_speed) { m_vSpeed = m_v_speed; }
	int  h_speed() const { return m_hSpeed; }
	void set_h_speed(const int m_h_speed) { m_hSpeed = m_h_speed; }

private:
	int m_vSpeed;
	int m_hSpeed;
};

class GhostRacer {};

#pragma region BorderLines
class BorderLine : public MovingActor
{
public:
	BorderLine(int ImageID, double startX) : MovingActor(ImageID, startX, 0, 0, 2, 1, -4, 0) { }
	BorderLine(int ImageID, double startX, double startY) : MovingActor(ImageID, startX, startY, 0, 2, 1, -4, 0) { }

	bool collisionAvoidanceWorthy() const override { return true; }
	bool interactWithProjectiles() const override { return false; }
	void doSomething() override;
};


class YellowBorderLine : public BorderLine
{
public:
	YellowBorderLine(bool left);
	YellowBorderLine(bool left, double startY);
};


class WhiteBorderLine : public BorderLine
{
public:
	WhiteBorderLine(bool left);
	WhiteBorderLine(bool left, double startY);
};
#pragma endregion

#endif // ACTOR_H_
