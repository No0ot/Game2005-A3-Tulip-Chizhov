#include "Bullet.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "BulletManager.h"
#include "Util.h"
#include "Player.h"
#include "Config.h"

Bullet::Bullet()
{
	TextureManager::Instance()->load("../Assets/textures/missle.png", "bullet");

	auto size = TextureManager::Instance()->getTextureSize("bullet");
	setWidth(size.x /3);
	setHeight(size.y /3);
	m_mass = 1.0f;
	m_pActive = false;

	getTransform()->position = glm::vec2(500.0f, 200.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);

	slope = nullptr;
	setGrenadeState(SETUP);
	setType(TARGET);
	rotation = 0.0f;
}

void Bullet::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	if(m_pActive)
		TextureManager::Instance()->draw("bullet", x, y, getWidth(), getHeight(), Util::Rad2Deg*rotation, 255, true);

	/*glm::vec4 green {0.0f,1.0f,0.0f,1.0f};
	Util::DrawCircle(getTransform()->position, getWidth() /2, green);*/
}

void Bullet::update(float deltaTime)
{
	if (m_pActive)
	{
		//getRigidBody()->acceleration = glm::vec2(0.0f, -5.0f);
		applyGravity(deltaTime);
		getRigidBody()->velocity -= getRigidBody()->acceleration;
		glm::vec2 pos = getTransform()->position;
		pos.x += getRigidBody()->velocity.x * deltaTime;
		pos.y += getRigidBody()->velocity.y * deltaTime;

		getTransform()->position = pos;
		/*if (getTransform()->position.y > Config::SCREEN_HEIGHT)
		{
			
			BulletManager::Instance()->returnBullet(this);
			std::cout << "dead bullet" << std::endl;
		}*/
	}
	else
	{
		reset();
	}
}

void Bullet::clean()
{
}

void Bullet::spawn(glm::vec2 position)
{
	reset();
	setActive(true);
	getTransform()->position = position;
	std::cout << "Bullet Made" << std::endl;
}

void Bullet::launch()
{
	setGrenadeState(INCLINE);
}

void Bullet::reset()
{
	getTransform()->position = glm::vec2(-50, -50);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
}

void Bullet::setActive(bool val)
{
	m_pActive = val;
}

bool Bullet::getActive()
{
	return m_pActive;
}

float Bullet::checkDistance(GameObject* pGameObject)
{
	return Util::distance(getTransform()->position, pGameObject->getTransform()->position);
}

void Bullet::CalculatePosition()
{
	getTransform()->position = Ground + (Util::rotateVector({ 0, -1 }, rotation) * (float)getHeight() * 0.5f);
}
