#pragma once
#include <queue>

#include "Bullet.h"
/* Singleton */
class BulletManager
{
public:
	static BulletManager* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new BulletManager();
		}
		return s_pInstance;
	}

	void populate(int queueMax);
	Bullet* fetchBullet();
	void returnBullet(Bullet*);
	void updatebullets(float deltaTime);
	void drawBullets();
	void CheckBulletCollisions(GameObject* collidingObject);
	int numActive;
	std::deque<Bullet*> m_pBulletPool;
	int index = 0;

private:
	BulletManager();
	~BulletManager();

	static BulletManager* s_pInstance;
};