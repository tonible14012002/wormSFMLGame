#include "State_Game.h"
#include "EventManger.h"
#include <ctime>

State_Game::State_Game(StateManager* l_State):BaseState(l_State),
	m_GameWorld(m_StateMgr->GetContext()->m_Windmgr->GetWindowSize().x,1,
		m_StateMgr->GetContext()->m_Windmgr->GetRenderWindow()){}

State_Game::~State_Game() {}

void State_Game::OnCreate() {

	EventManager* l_EMgr = m_StateMgr->GetContext()->m_EventMgr;

	l_EMgr->AddCallback(StateType::Game,"Key_Escape", &State_Game::Back_to_Menu, this);
	l_EMgr->AddCallback(StateType::Game, "Key_P", &State_Game::Paused, this);
	l_EMgr->AddCallback(StateType::Game, "Key_N", &State_Game::AddDebris, this);
	l_EMgr->AddCallback(StateType::Game, "Key_M_Fullview", &State_Game::ControlView, this);
	l_EMgr->AddCallback(StateType::Game, "Key_M_Release", &State_Game::ControlView, this);

	l_EMgr->AddCallback(StateType::Game, "Arrow_Left_Pressed", &State_Game::ControlCrosshair, this);
	l_EMgr->AddCallback(StateType::Game, "Arrow_Left_Released", &State_Game::ControlCrosshair, this);
	l_EMgr->AddCallback(StateType::Game, "Arrow_Right_Pressed", &State_Game::ControlCrosshair, this);
	l_EMgr->AddCallback(StateType::Game, "Arrow_Right_Released", &State_Game::ControlCrosshair, this);
	l_EMgr->AddCallback(StateType::Game, "Arrow_Up_Pressed", &State_Game::Jump, this);
	l_EMgr->AddCallback(StateType::Game, "Space_Pressed", &State_Game::FireMissile, this);
	l_EMgr->AddCallback(StateType::Game, "Space_Release", &State_Game::FireMissile, this);
	
}

void State_Game::OnDestroy() {
	EventManager* l_EMgr = m_StateMgr->GetContext()->m_EventMgr;
	l_EMgr->RemoveCallback(StateType::Game, "Mouse_Left");
	l_EMgr->RemoveCallback(StateType::Game, "Key_Escape");
	l_EMgr->RemoveCallback(StateType::Game, "Key_N");
	l_EMgr->RemoveCallback(StateType::Game, "Arrow_Left_Pressed");
	l_EMgr->RemoveCallback(StateType::Game, "Arrow_Left_Released");
	l_EMgr->RemoveCallback(StateType::Game, "Arrow_Right_Released");
	l_EMgr->RemoveCallback(StateType::Game, "Arrow_Right_Pressed");
	l_EMgr->RemoveCallback(StateType::Game, "Arrow_Up_Pressed");
	l_EMgr->RemoveCallback(StateType::Game, "Space_Pressed");
	l_EMgr->RemoveCallback(StateType::Game, "Space_Release");
	l_EMgr->RemoveCallback(StateType::Game, "Key_M_Fullview");
	l_EMgr->RemoveCallback(StateType::Game, "Key_M_Release");
}

void State_Game::Activate() {
	
}

void State_Game::Deactivate() {
	
}

void State_Game::Update(const sf::Time& l_Time) {
	m_GameWorld.Update(l_Time);
}

void State_Game::Draw() {
	m_GameWorld.Render();
}

void State_Game::Back_to_Menu(EventDetails* l_Detail) {
	m_StateMgr->SwitchTo(StateType::SubMenu);
	m_StateMgr->Remove(StateType::Game);
}

void State_Game::Paused(EventDetails* l_Detail) {
	m_StateMgr->SwitchTo(StateType::Paused);
}

void State_Game::AddDebris(EventDetails* l_Detail) {
	m_GameWorld.SpawnWorm();
}

void State_Game::ControlView(EventDetails *l_Detail) {
	if (l_Detail->m_BindName == "Key_M_Fullview")
	m_GameWorld.ShowFullview(true);
	else
	{
		m_GameWorld.ShowFullview(false);
	}
}

void State_Game::ControlCrosshair(EventDetails* l_Detail) {

	if (!m_GameWorld.IsUnderControl()) return;

	std::string EventName = l_Detail->m_BindName;
	if (EventName == "Arrow_Left_Pressed") m_GameWorld.SetRotate(true, 0);
	else if (EventName == "Arrow_Right_Pressed") m_GameWorld.SetRotate(true, 1);
	else m_GameWorld.SetRotate(false, 0);

}

void State_Game::Jump(EventDetails* l_Detail) {
	m_GameWorld.Jump();
}

void State_Game::FireMissile(EventDetails* l_Detail) {
	if (l_Detail->m_BindName == "Space_Pressed" && m_GameWorld.IsUnderControl())
		m_GameWorld.SetFire(true);
	else {
		m_GameWorld.Fire();
	}
}