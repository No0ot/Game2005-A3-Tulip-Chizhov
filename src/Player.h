#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "Sprite.h"
#include "Label.h"

class Player final : public Sprite
{
public:
	Player();
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update(float deltatime) override;
	virtual void clean() override;

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopXMovement();
	void stopYMovement();
	void checkCollision(GameObject* pGameObject);
	void checkBounds();

	float checkDistance(GameObject* pGameObject);


private:
	const float SPEED = 100.0f;
	const float ACCELERATION = 15.0f;
	glm::vec2 m_direction;
};

#endif /* defined (__PLAYER__) */