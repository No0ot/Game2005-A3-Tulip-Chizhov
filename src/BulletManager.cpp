#include "BulletManager.h"
#include "CollisionManager.h"
#include <iostream>

void BulletManager::populate(int queueMax)
{
	for (int i = 0; i < queueMax; i++)
	{	
		m_pBulletPool.emplace_back(new Bullet());
		
	}
}

Bullet* BulletManager::fetchBullet()
{
	Bullet* bullet = NULL;
	if (index < m_pBulletPool.size()-1)
	{
		bullet = m_pBulletPool[index];
		index++;
		return bullet;
	}
	else
	{
		index = 0;
		return nullptr;
	}
}

void BulletManager::returnBullet(Bullet* bullet)
{
	bullet->setActive(false);
	bullet->reset();
	numActive--;
	//m_pBulletPool.push_back(bullet);
}

void BulletManager::cleanPool()
{
	for (auto bullet : m_pBulletPool)
	{
		bullet->setActive(false);
	}
	numActive = 0;
}

void BulletManager::setMass(float mass)
{
	for (auto bullet : m_pBulletPool)
	{
		bullet->m_mass = mass;
	}
}

void BulletManager::updatebullets(float deltaTime)
{
	for (int i = 0; i < m_pBulletPool.size(); i++)
	{
		if (m_pBulletPool[i]->getActive())
			m_pBulletPool[i]->update(deltaTime);
		else
			m_pBulletPool[i]->reset();
	}
}

void BulletManager::drawBullets()
{
	for (int i = 0; i < m_pBulletPool.size(); i++)
	{
		m_pBulletPool[i]->draw();
	}
}

void BulletManager::CheckBulletCollisions(GameObject* collidingObject)
{
	for (int i = 0; i < m_pBulletPool.size(); i++)
	{
		if (m_pBulletPool[i]->getActive())
		{
			if (CollisionManager::circleAABBCheck(collidingObject, m_pBulletPool[i]))
			{
				returnBullet(m_pBulletPool[i]);
				numCollisions++;
			}
			if (m_pBulletPool[i]->getTransform()->position.y > Config::SCREEN_HEIGHT)
			{
				numMissed++;
				returnBullet(m_pBulletPool[i]);
				std::cout << "dead bullet" << std::endl;
			}
		}

	}
}

BulletManager::BulletManager()
= default;

BulletManager::~BulletManager()
= default;

BulletManager* BulletManager::s_pInstance = nullptr;
