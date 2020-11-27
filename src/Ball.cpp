#include "Ball.h"
#include "TextureManager.h"
#include "SoundManager.h"

Ball::Ball()
{
	TextureManager::Instance()->load("../Assets/textures/planet.png", "ball");
	getTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(BALL);
	setWidth(40);
	setHeight(40);

	setWallBounces(0);
	setBrickBounces(0);
}

Ball::~Ball()
{
}

void Ball::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("ball", x, y, getWidth(), getHeight(), 0, 255, true);
}

void Ball::update(float deltatime)
{
	move();
	checkBoundsCircle();
}

void Ball::clean()
{
}

void Ball::spawn(glm::vec2 position)
{
	getTransform()->position = position;
	getRigidBody()->velocity = { 0.0f, 0.0f };
}

void Ball::launch()
{
}

void Ball::move()
{
	getRigidBody()->velocity += getRigidBody()->acceleration;
	getTransform()->position += getRigidBody()->velocity;
	getRigidBody()->acceleration = { 0.0f, 0.0f };
	checkBoundsCircle();
}

void Ball::checkBoundsCircle()
{
	//lower
	if (getTransform()->position.y + getHeight() / 2 > Config::SCREEN_HEIGHT) {
		getTransform()->position.y = Config::SCREEN_HEIGHT - getHeight() / 2;
		reflectY();
		getRigidBody()->velocity *= 0.9f;
		setWallBounces(getWallBounces() + 1);
		SoundManager::Instance().playSound("wall_hit", 0);
	}
	//upper
	if (getTransform()->position.y - getHeight() / 2 < 0.0f) {
		getTransform()->position.y = getHeight() / 2;
		reflectY();
		getRigidBody()->velocity *= 0.9f;
		setWallBounces(getWallBounces() + 1);
		SoundManager::Instance().playSound("wall_hit", 0);
	}
	//left
	if (getTransform()->position.x - getWidth() / 2 < 0.0f) {
		getTransform()->position.x = getWidth() / 2;
		reflectX();
		getRigidBody()->velocity *= 0.9f;
		setWallBounces(getWallBounces() + 1);
		SoundManager::Instance().playSound("wall_hit", 0);
	}
	//right
	if (getTransform()->position.x + getWidth() / 2 > Config::SCREEN_WIDTH) {
		getTransform()->position.x = Config::SCREEN_WIDTH - getWidth() / 2;
		reflectX();
		getRigidBody()->velocity *= 0.9f;
		setWallBounces(getWallBounces() + 1);
		SoundManager::Instance().playSound("wall_hit", 0);
	}
}

void Ball::applyForce(glm::vec2 force)
{
	getRigidBody()->acceleration += force;
}

void Ball::reflectX()
{
	getRigidBody()->velocity.x *= -1;
}

void Ball::reflectY()
{
	getRigidBody()->velocity.y *= -1;
}
