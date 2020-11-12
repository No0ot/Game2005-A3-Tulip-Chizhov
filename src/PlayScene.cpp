#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

// required for IMGUI
#include "IMGUI/imgui.h"
#include "IMGUI_SDL/imgui_sdl.h"
#include "Renderer.h"



PlayScene::PlayScene()
{
	TextureManager::Instance()->load("../Assets/textures/background.png", "background");
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
}

void PlayScene::update(float deltaTime)
{
	updateDisplayList(deltaTime);

	m_pDistanceLabel->setText("Box position X: " + std::to_string(m_pGrenade->getTransform()->position.x) + ", Y: " + std::to_string(m_pGrenade->getTransform()->position.y));
	m_pVelocityLabel->setText("Box velocity on X axis: " + std::to_string(m_pGrenade->getRigidBody()->velocity.x / PX_PER_METER) + " m/s");
	m_pAngleLabel->setText("Box angle: " + std::to_string(Util::Rad2Deg * m_pGrenade->rotation) + " degrees");
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
	m_buildGrid();

	//// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->setParent(this);
	addChild(m_pPlayer);
	
	// Box Sprite
	m_pGrenade = new Bullet();
	m_pGrenade->setParent(this);
	addChild(m_pGrenade);
	resetSim();

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
