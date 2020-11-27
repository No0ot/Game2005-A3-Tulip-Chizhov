#include "PlayScene2.h"
#include "Game.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "Util.h"

// required for IMGUI
#include "IMGUI/imgui.h"
#include "IMGUI_SDL/imgui_sdl.h"
#include "Renderer.h"


PlayScene2::PlayScene2()
{
	TextureManager::Instance()->load("../Assets/textures/background.png", "background");
	SoundManager::Instance().load("../Assets/audio/wall_hit.ogg", "wall_hit", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/hit.ogg", "hit", SOUND_SFX);
	PlayScene2::start();
}

PlayScene2::~PlayScene2()
= default;

void PlayScene2::draw()
{
	for (auto tile : m_pGrid)
		tile->draw();

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
}

void PlayScene2::update(float deltaTime)
{
	updateDisplayList(deltaTime);

	m_pBrickVelocityLabel->setText("Brick Velocity X: " + std::to_string(m_pBrick->getRigidBody()->velocity.x) + ", Y: " + std::to_string(m_pBrick->getRigidBody()->velocity.y));
	m_pBallVelocityLabel->setText("Ball Velocity X: " + std::to_string(m_pBall->getRigidBody()->velocity.x) + ", Y: " + std::to_string(m_pBall->getRigidBody()->velocity.y));
	m_pWallBounceLabel->setText("Wall Bounces: " + std::to_string(m_pBall->getWallBounces()));
	m_pBrickBounceLabel->setText("Brick Bounces: " + std::to_string(m_pBall->getBrickBounces()));
}

void PlayScene2::clean()
{
	removeAllChildren();
}

void PlayScene2::handleEvents(float deltaTime)
{
	EventManager::Instance().update();

	if (EventManager::Instance().getMouseButton(0)) {
		m_pBrick->setControlled(true);
	}
	else m_pBrick->setControlled(false);

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
}

void PlayScene2::start()
{
	m_buildGrid();

	//// Player Sprite
	m_pBrick = new Brick();
	m_pBrick->setParent(this);
	addChild(m_pBrick);

	//// Ball Sprite
	m_pBall = new Ball();
	m_pBall->setParent(this);
	addChild(m_pBall);
	m_pBrick->setBall(m_pBall);
	//defaults

	// Labels
	const SDL_Color blue = { 0, 0, 255, 255 };

	m_pBrickVelocityLabel = new Label("Brick Velocity", "Consolas", 30, blue, glm::vec2(700, 40.0f));
	m_pBrickVelocityLabel->setParent(this);
	addChild(m_pBrickVelocityLabel);

	m_pBallVelocityLabel = new Label("Ball Velocity", "Consolas", 30, blue, glm::vec2(700, 70.0f));
	m_pBallVelocityLabel->setParent(this);
	addChild(m_pBallVelocityLabel);

	m_pWallBounceLabel = new Label("Wall Bounces", "Consolas", 30, blue, glm::vec2(700, 100.0f));
	m_pWallBounceLabel->setParent(this);
	addChild(m_pWallBounceLabel);

	m_pBrickBounceLabel = new Label("Brick Bounces", "Consolas", 30, blue, glm::vec2(700, 130.0f));
	m_pBrickBounceLabel->setParent(this);
	addChild(m_pBrickBounceLabel);

}

void PlayScene2::resetSim()
{
	reset();
	m_pBall->setWallBounces(0);
	m_pBall->setBrickBounces(0);
}

void PlayScene2::reset()
{
	m_pBrick->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2, 700);
	m_pBrick->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	m_pBrick->setControlled(false);
	m_pBall->spawn(glm::vec2(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2));
}

void PlayScene2::launch()
{
}

void PlayScene2::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::SetWindowPos(ImVec2(0, 0), true);

	ImGui::Separator();

	if (ImGui::Button("Reset Positions"))
	{
		reset();
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset Sim"))
	{
		resetSim();
	}

	ImGui::Separator();

	if (ImGui::Button("Start Scene"))
	{
		resetSim();
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	ImGui::SameLine();

	if (ImGui::Button("Next Scene"))
	{
		resetSim();
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene2::m_buildGrid()
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