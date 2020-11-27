#include "Brick.h"
#include "Ball.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Util.h"

Brick::Brick()
{
	TextureManager::Instance()->load("../Assets/textures/Brick.png", "brick");

	auto size = TextureManager::Instance()->getTextureSize("brick");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2, 700);
	setLastPosition(getTransform()->position);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(BRICK);
	buildBoundingBox();

	controlled = false;
}

void Brick::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("brick", x, y, getWidth(), getHeight(), 0, 255, true);
}

void Brick::update(float deltatime)
{
	iFrames--;
	if (iFrames < 0) iFrames = 0;
	move();
	if (iFrames == 0) checkCircleCollision(getBall());
}

void Brick::clean()
{
	m_pBall = nullptr;
}

void Brick::move()
{
	if (getControlled()) {
		getTransform()->position = EventManager::Instance().getMousePosition();
		getRigidBody()->velocity = getTransform()->position - getLastPosition();
		buildBoundingBox();
	}
	else {
		getRigidBody()->velocity = { 0.0f, 0.0f };
	}
	setLastPosition(getTransform()->position);
}

void Brick::buildBoundingBox()
{
	SDL_Rect temp;
	temp.x = getTransform()->position.x - getWidth() / 2;
	temp.y = getTransform()->position.y - getHeight() / 2;
	temp.w = getWidth();
	temp.h = getHeight();
	setBounds(temp);
}

void Brick::checkCircleCollision(GameObject* pGameObject)
{
	glm::vec2 lineStart;
	glm::vec2 lineEnd;
	//top collision
	lineStart = { m_bounds.x, m_bounds.y };
	lineEnd = { m_bounds.x + m_bounds.w, m_bounds.y };
	if (CollisionManager::circleLineCheck(lineStart, lineEnd, pGameObject)) {
		getBall()->reflectY();
		getBall()->applyForce(getRigidBody()->velocity);
		getBall()->setBrickBounces(getBall()->getBrickBounces() + 1);
		SoundManager::Instance().playSound("hit", 0);
		iFrames = 15;
	}
	//bottom collision
	lineStart.y += m_bounds.h;
	lineEnd.y += m_bounds.h;
	if (CollisionManager::circleLineCheck(lineStart, lineEnd, pGameObject)) {
		getBall()->reflectY();
		getBall()->applyForce(getRigidBody()->velocity);
		getBall()->setBrickBounces(getBall()->getBrickBounces() + 1);
		SoundManager::Instance().playSound("hit", 0);
		iFrames = 15;
	}
	//left collision
	lineEnd.x = lineStart.x;
	lineEnd.y -= m_bounds.h;
	if (CollisionManager::circleLineCheck(lineStart, lineEnd, pGameObject)) {
		getBall()->reflectX();
		getBall()->applyForce(getRigidBody()->velocity);
		getBall()->setBrickBounces(getBall()->getBrickBounces() + 1);
		SoundManager::Instance().playSound("hit", 0);
		iFrames = 15;
	}
	//right collision
	lineStart.x += m_bounds.w;
	lineEnd.x += m_bounds.w;
	if (CollisionManager::circleLineCheck(lineStart, lineEnd, pGameObject)) {
		getBall()->reflectX();
		getBall()->applyForce(getRigidBody()->velocity);
		getBall()->setBrickBounces(getBall()->getBrickBounces() + 1);
		SoundManager::Instance().playSound("hit", 0);
		iFrames = 15;
	}
}
