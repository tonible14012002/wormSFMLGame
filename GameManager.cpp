#include "GameManager.h"

GameManager::GameManager() :m_WindManager("MyGame!", sf::Vector2u(1000, 600)),
		m_StateManager(&m_Context)
{	
	m_Context.m_Windmgr = &m_WindManager;
	m_Context.m_EventMgr = m_WindManager.GetEventManager();
	m_StateManager.SwitchTo(StateType::Intro);


	m_WindManager.GetEventManager()->
		AddCallback(StateType(0),"Move", &GameManager::DrawRec, this);
}

GameManager::~GameManager() {

}

void GameManager::DrawRec(EventDetails* l_detail) {
	sf::Vector2i Mouspos = m_WindManager.GetEventManager()->GetMousePos(m_WindManager.GetRenderWindow());
	
	m_Shape.setRadius(50);
	m_Shape.setFillColor(sf::Color::Blue);
	m_Shape.setOrigin(50, 50);
	m_Shape.setPosition(Mouspos.x, Mouspos.y);
}

void GameManager::Update() {
	m_WindManager.Update();
	m_StateManager.Update(m_Elapsed);
}

void GameManager::LateUpdate() {
	m_StateManager.ProcessRequest();
	ResetClock();
}

void GameManager::Render() {
	m_WindManager.BeginDraw();
	//m_windManager.Draw(....);
	//Gameclass Render();
	m_WindManager.Draw(m_Shape);
	m_StateManager.Draw();
	m_WindManager.EndDraw();
}

void GameManager::HandleInput() {}

void GameManager::ResetClock() {
	m_Elapsed = m_Clock.restart();
}

sf::Time GameManager::GetElapsed() { return m_Elapsed; }

WindowManager* GameManager::GetWindowManager() {
	return &m_WindManager;
}