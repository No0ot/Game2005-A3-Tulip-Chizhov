#include "Bullet.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "Util.h"
#include "Player.h"
#include "Config.h"

Bullet::Bullet()
{
	TextureManager::Instance()->load("../Assets/textures/bullet.png", "bullet");

	auto size = TextureManager::Instance()->getTextureSize("bullet");
	setWidth(size.x);
	setHeight(size.y);
	m_mass = 5;

	getTransform()->position = glm::vec2(500.0f, 200.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);

	slope = nullptr;
	setGrenadeState(SETUP);
	rotation = 0.0f;
}

void Bullet::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	TextureManager::Instance()->draw("bullet", x, y, getWidth(), getHeight(), Util::Rad2Deg*rotation, 255, true);

	/*glm::vec4 green {0.0f,1.0f,0.0f,1.0f};
	Util::DrawCircle(getTransform()->position, getWidth() /2, green);*/
}

void Bullet::update(float deltaTime)
{
	applyGravity(deltaTime);

	getRigidBody()->velocity -= getRigidBody()->acceleration;
	glm::vec2 pos = getTransform()->position;
	pos.x += getRigidBody()->velocity.x * deltaTime;
	pos.y += getRigidBody()->velocity.y * deltaTime;

	getTransform()->position = pos;
}

void Bullet::clean()
{
}

void Bullet::spawn(glm::vec2 position)
{
	Ground = position;
	CalculatePosition();
	getRigidBody()->acceleration = { 0.0f, 0.0f };
	getRigidBody()->velocity = { 0.0f, 0.0f };
	setGrenadeState(SETUP);
}

void Bullet::launch()
{
	setGrenadeState(INCLINE);
}

float Bullet::checkDistance(GameObject* pGameObject)
{
	return Util::distance(getTransform()->position, pGameObject->getTransform()->position);
}

void Bullet::CalculatePosition()
{
	getTransform()->position = Ground + (Util::rotateVector({ 0, -1 }, rotation) * (float)getHeight() * 0.5f);
}
