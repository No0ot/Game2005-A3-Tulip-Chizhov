#pragma once
#ifndef __GRENADE__
#define __GRENADE__

#include "PhysicsObject.h"

class Player;

enum GrenadeState {
	SETUP,
	INCLINE,
	FLAT,
	STOP
};

class Bullet : public PhysicsObject
{
private:
	GrenadeState m_state;
	bool m_pActive;
public:
	Bullet();
	~Bullet() = default;

	void draw();
	void update(float deltaTime);
	void clean();

	void spawn(glm::vec2 position) override;
	void launch() override;
	void reset();

	void setActive(bool val);
	bool getActive();
	float checkDistance(GameObject* pGameObject);

	GrenadeState getGrenadeState() { return m_state; }
	void setGrenadeState(GrenadeState n) { m_state = n; }

	//a2 sim members
	Player* slope;
	glm::vec2 Ground;
	float rotation;
	void CalculatePosition();
};
#endif 