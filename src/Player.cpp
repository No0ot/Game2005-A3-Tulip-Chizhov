#include "Player.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "Util.h"

Player::Player()
{
	TextureManager::Instance()->load("../Assets/textures/ship.png", "ship");

	auto size = TextureManager::Instance()->getTextureSize("ship");
	setWidth(size.x / 6);
	setHeight(size.y / 6);

	getTransform()->position = glm::vec2(Config::SCREEN_WIDTH /2, Config::SCREEN_HEIGHT /2);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("ship", x, y,getWidth(), getHeight(), 0, 255, true);
}

void Player::update(float deltatime)
{
	//const float deltaTime = 1.0f / 60.f;
	if (Util::magnitude(m_direction) > 0)
	{
		m_direction *= 1.0f / Util::magnitude(m_direction);

		getRigidBody()->acceleration = Util::normalize(m_direction) * ACCELERATION;

	}
	else if (Util::magnitude(getRigidBody()->velocity) > 0)
	{
		getRigidBody()->acceleration = Util::normalize(getRigidBody()->velocity) * (-ACCELERATION * 1.5f);

		if (Util::magnitude(getRigidBody()->velocity) < ACCELERATION)
		{
			getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
			getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
		}
	}
	checkBounds();

	getRigidBody()->velocity += getRigidBody()->acceleration;
	glm::vec2 pos = getTransform()->position;
	pos.x += getRigidBody()->velocity.x * deltatime;
	pos.y += getRigidBody()->velocity.y * deltatime;

	getTransform()->position = pos;

}

void Player::clean()
{

}

void Player::moveLeft()
{
	m_direction.x = -1;
}

void Player::moveRight()
{
	m_direction.x = 1;
}

void Player::moveUp()
{
	m_direction.y = -1;
}

void Player::moveDown()
{
	m_direction.y = 1;
}

void Player::stopXMovement()
{
	m_direction.x = 0;
}

void Player::stopYMovement()
{
	m_direction.y = 0;
}

void Player::checkCollision(GameObject * pGameObject)
{
	glm::vec2 ob1 = getTransform()->position;
	glm::vec2 ob2 = pGameObject->getTransform()->position;
	int halfHeights = (getHeight() + pGameObject->getHeight()) * 0.5f;

	if (Util::squaredDistance(ob1, ob2) < (halfHeights * halfHeights))
		getRigidBody()->isColliding = true;
	else
		getRigidBody()->isColliding = false;
}

void Player::checkBounds()
{
	glm::vec2 ob = getTransform()->position;

	if (ob.x > Config::SCREEN_WIDTH || ob.x < 0 )
	{

		getRigidBody()->velocity *= glm::vec2(-0.8, 0.8);
	}
	if (ob.y > Config::SCREEN_HEIGHT || ob.y < 0)
	{

		getRigidBody()->velocity *= glm::vec2(0.8, -0.8);
	}
}

float Player::checkDistance(GameObject * pGameObject)
{
	float tempDistance;

	tempDistance = Util::distance(getTransform()->position, pGameObject->getTransform()->position);

	return tempDistance;
}