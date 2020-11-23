#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "BulletManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "Util.h"

// required for IMGUI
#include "IMGUI/imgui.h"
#include "IMGUI_SDL/imgui_sdl.h"
#include "Renderer.h"



PlayScene::PlayScene()
{
	TextureManager::Instance()->load("../Assets/textures/background.png", "background");
	SoundManager::Instance().load("../Assets/audio/thunder.ogg", "thunder", SOUND_SFX);
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	for (auto tile : m_pGrid)
		tile->draw();

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	BulletManager::Instance()->drawBullets();
}

void PlayScene::update(float deltaTime)
{
	updateDisplayList(deltaTime);
	m_pBulletTimer++;
	if (m_pBulletTimer == m_pBulletTimerMax)
	{
		//std::cout << "tick" << std::endl;
		if (m_pbulletSpawn)
		{
			if (BulletManager::Instance()->numActive < m_pMaxActive)
			{
				Bullet* bullet = BulletManager::Instance()->fetchBullet();
				if (bullet != nullptr)
				{
					bullet->spawn(glm::vec2(rand() % 1600 + 20, -50));
					BulletManager::Instance()->numActive++;
				}
			}
		}
			m_pBulletTimer = 0;
	}

	m_pCollisionsLabel->setText("Number of Collisions: " + std::to_string(BulletManager::Instance()->numCollisions));
	m_pMissedLabel->setText("Number Missed : " + std::to_string(BulletManager::Instance()->numMissed));

	CheckCollisions();
	BulletManager::Instance()->updatebullets(deltaTime);
	std::cout << BulletManager::Instance()->numActive << std::endl;
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents(float deltaTime)
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	{
		m_pPlayer->moveLeft();
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		m_pPlayer->moveRight();
	}
	else
	{
		m_pPlayer->stopXMovement();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
	{
		m_pPlayer->moveUp();
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
	{
		m_pPlayer->moveDown();
	}
	else
	{
		m_pPlayer->stopYMovement();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
}

void PlayScene::start()
{
	BulletManager::Instance()->populate(m_pMaxBullets);
	m_pBulletTimer = 0;
	m_buildGrid();

	//// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->setParent(this);
	addChild(m_pPlayer);

	// Labels
	const SDL_Color blue = { 0, 0, 255, 255 };

	m_pScaleLabel = new Label("Scale = 100 px/m", "Consolas", 30, blue, glm::vec2(700, 40.0f));
	m_pScaleLabel->setParent(this);
	addChild(m_pScaleLabel);

	m_pCollisionsLabel = new Label("Number of Collisions", "Consolas", 30, blue, glm::vec2(700, 70.0f));
	m_pCollisionsLabel->setParent(this);
	addChild(m_pCollisionsLabel);

	m_pMissedLabel = new Label("Number Missed", "Consolas", 30, blue, glm::vec2(700, 100.0f));
	m_pMissedLabel->setParent(this);
	addChild(m_pMissedLabel);

}

void PlayScene::resetSim()
{
	m_pbulletSpawn = false;
	 m_pMaxBullets = 100;
	 m_pMaxActive = 10;
	 m_pBulletTimerMax = 10;
	 m_pBulletMass = 1.0f;
	 m_pPlayer->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2);
	 m_pPlayer->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	 m_pPlayer->getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	 BulletManager::Instance()->numCollisions = 0;
	 BulletManager::Instance()->numMissed = 0;
	 BulletManager::Instance()->cleanPool();
}

void PlayScene::reset()
{
}

void PlayScene::launch()
{
	m_pGrenade->launch();
}

void PlayScene::CheckCollisions()
{
	BulletManager::Instance()->CheckBulletCollisions(m_pPlayer);
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::SetWindowPos(ImVec2(0, 0), true);

	ImGui::Separator();

	if (ImGui::Button("Start"))
	{
		m_pbulletSpawn = true;
	}

	ImGui::SameLine();

	if (ImGui::Button("Stop"))
	{
		m_pbulletSpawn = false;
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset Sim"))
	{
		resetSim();
	}


	ImGui::Separator();
	if (ImGui::SliderInt("Max Active Bullets", &m_pMaxActive, 1, 100))
	{
		clean();
		start();
	}
	
	if (ImGui::SliderInt("Bullet Timer", &m_pBulletTimerMax, 1, 60))
	{
		clean();
		start();
	}

	if (ImGui::SliderFloat("Bullet Mass", &m_pBulletMass, 0.5f, 5.0f))
	{
		clean();
		start();
		BulletManager::Instance()->setMass(m_pBulletMass);
	}

	ImGui::Separator();

	if (ImGui::Button("Start Scene"))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	ImGui::SameLine();

	if (ImGui::Button("Next Scene"))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene::m_buildGrid()
{
	const auto size = Config::TILE_SIZE;
	const auto offset = size * 0.5f;

	m_pGrid = std::vector<Tile*>();

	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto tile = new Tile(glm::vec2(offset + size * col, offset + size * row), glm::vec2(col, row));
			//addChild(tile);
			m_pGrid.push_back(tile);
		}
	}
}
