#include "Target.h"
#include "TextureManager.h"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/Stormtrooper.png","trooper");

	const auto size = TextureManager::Instance()->getTextureSize("trooper");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("circle", x, y, getWidth(), getHeight(), 0, 255, true);
}

void Target::update(float deltaTime)
{
	m_move(deltaTime);
	m_checkBounds();
}

void Target::clean()
{
}

void Target::m_move(float deltaTime)
{
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * 5.0f * deltaTime;
}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}
