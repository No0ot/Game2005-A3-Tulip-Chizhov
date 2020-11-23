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
		if (BulletManager::Instance()->numActive < BulletManager::Instance()->m_pBulletPool.size())
		{
			Bullet* bullet = BulletManager::Instance()->fetchBullet();
			if (bullet != nullptr)
			{
				bullet->spawn(glm::vec2(rand() % 1600 + 20, -50));
				BulletManager::Instance()->numActive++;
			}
		}
			m_pBulletTimer = 0;
	}

	CheckCollisions();
	BulletManager::Instance()->updatebullets(deltaTime);
	std::cout << BulletManager::Instance()->m_pBulletPool.size() << std::endl;
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
	m_pBulletTimerMax = 20;
	BulletManager::Instance()->populate(20);

	m_buildGrid();

	//// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->setParent(this);
	addChild(m_pPlayer);

	// Labels
	const SDL_Color blue = { 0, 0, 255, 255 };

	m_pDistanceLabel = new Label("Distance", "Consolas", 30, blue, glm::vec2(700, 40.0f));
	m_pDistanceLabel->setParent(this);
	addChild(m_pDistanceLabel);

	m_pVelocityLabel = new Label("Velocity", "Consolas", 30, blue, glm::vec2(700, 70.0f));
	m_pVelocityLabel->setParent(this);
	addChild(m_pVelocityLabel);

	m_pAngleLabel = new Label("Velocity", "Consolas", 30, blue, glm::vec2(700, 100.0f));
	m_pAngleLabel->setParent(this);
	addChild(m_pAngleLabel);

	m_pScaleLabel = new Label("Scale = 100 px/m", "Consolas", 30, blue, glm::vec2(700, 130.0f));
	m_pScaleLabel->setParent(this);
	addChild(m_pScaleLabel);

}

void PlayScene::resetSim()
{
	reset();
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

	if (ImGui::Button("Release"))
	{
		if (m_pGrenade->getGrenadeState() == SETUP)
			launch();
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset Box"))
	{
		reset();
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset Sim"))
	{
		resetSim();
	}

	ImGui::Separator();

	
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
