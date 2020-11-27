#pragma once
#ifndef __BALL__
#define __BALL__

#include "PhysicsObject.h"
#include "ShapeType.h"

class Ball final : public PhysicsObject
{
public:
	Ball();
	~Ball();

	virtual void draw() override;
	virtual void update(float deltatime) override;
	virtual void clean() override;

	void spawn(glm::vec2 position) override;
	void launch() override;

	void move();
	void checkBoundsCircle();
	void applyForce(glm::vec2 force);
	void reflectX();
	void reflectY();

	void setWallBounces(int n) { m_wallBounces = n; }
	int getWallBounces() { return m_wallBounces; }
	void setBrickBounces(int n) { m_brickBounces = n; }
	int getBrickBounces() { return m_brickBounces; }

private:
	int m_wallBounces;
	int m_brickBounces;
};

#endif