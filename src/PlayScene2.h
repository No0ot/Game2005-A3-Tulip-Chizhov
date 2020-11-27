#pragma once

#ifndef __PLAY_SCENE_2__
#define __PLAY_SCENE_2__

#include "Scene.h"
#include "Button.h"
#include "Brick.h"
#include "Ball.h"
#include "Tile.h"
#include <cassert>

class PlayScene2 : public Scene
{
public:
	PlayScene2();
	~PlayScene2();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update(float deltaTime) override;
	virtual void clean() override;
	virtual void handleEvents(float deltaTime) override;
	virtual void start() override;

	void resetSim();
	void reset();
	void launch();

	//scene getters
	Brick* getBrick() {
		assert(m_pBrick != nullptr);
		return m_pBrick;
	}

	Ball* getBall() {
		assert(m_pBall != nullptr);
		return m_pBall;
	}
private:
	// IMGUI Function
	void GUI_Function();

	void m_buildGrid();

	bool m_inputValid;

	std::vector<Tile*> m_pGrid;
	Brick* m_pBrick;
	Ball* m_pBall;

	Label* m_pBrickVelocityLabel;
	Label* m_pBallVelocityLabel;
	Label* m_pWallBounceLabel;
	Label* m_pBrickBounceLabel;

	Button* m_pBackButton;
	Button* m_pNextButton;

};

#endif /* defined (__PLAY_SCENE_2__) */