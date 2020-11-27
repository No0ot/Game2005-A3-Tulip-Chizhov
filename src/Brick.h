#pragma once
#ifndef __BRICK__
#define __BRICK__

#include "Sprite.h"
#include "Label.h"

class Ball;

class Brick final : public Sprite
{
public:
	Brick();
	~Brick() = default;

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update(float deltatime) override;
	virtual void clean() override;

	void move();
	void buildBoundingBox();
	void checkCircleCollision(GameObject* pGameObject);

	void setBounds(SDL_Rect n) { m_bounds = n; }
	SDL_Rect getBounds() { return m_bounds; }
	void setBall(Ball* n) { m_pBall = n; }
	Ball* getBall() { return m_pBall; }
	void setLastPosition(glm::vec2 n) { m_lastPosition = n; }
	glm::vec2 getLastPosition() { return m_lastPosition; }
	void setControlled(bool n) { controlled = n; }
	bool getControlled() { return controlled; }

private:
	bool controlled;
	int iFrames;
	Ball* m_pBall;
	glm::vec2 m_lastPosition;
	SDL_Rect m_bounds;
};

#endif /* defined (__BRICK__) */