#include "StateManager.h"
#include "State_Intro.h"
#include "State_MainMenu.h"
#include "State_Game.h"
#include "State_Pause.h"
#include "State_SubMenu.h"

StateManager::StateManager(SharedContext* l_Shared) :m_Shared(l_Shared) {
//Register State here.
	RegisterState<State_Intro>(StateType::Intro);
	RegisterState<State_MainMenu>(StateType::MainMenu);
	RegisterState<State_Game>(StateType::Game);
	RegisterState<State_Pause>(StateType::Paused);
	RegisterState<State_SubMenu>(StateType::SubMenu);
}

StateManager::~StateManager() {
	for (auto& Itr : m_StateVec) {
		Itr.second->OnDestroy();
	}
}

void StateManager::Draw() {
	if (m_StateVec.empty()) return;
	if (m_StateVec.back().second->IsTransparent()
		&& m_StateVec.size() > 1) {
		
		auto Itr = m_StateVec.end();
		while (Itr != m_StateVec.begin()) {
			if (Itr != m_StateVec.end()) {
				if (!Itr->second->IsTransparent()) {
					break;
				}
			}
			--Itr;
		}
		for (; Itr != m_StateVec.end(); ++Itr) {
			Itr->second->Draw();
		}
	}
	else {
		m_StateVec.back().second->Draw();
	}
}

void StateManager::Update(const sf::Time& l_Time) {
	if (m_StateVec.empty()) { return; }
	if (m_StateVec.back().second->IsTranscendent()
		&& m_StateVec.size() > 1) {

		auto Itr = m_StateVec.end();
		while (Itr != m_StateVec.begin()) {
			if (Itr != m_StateVec.end()) {
				if (!Itr->second->IsTranscendent()) {
					break;
				}
			}
			--Itr;
		}
		for (; Itr != m_StateVec.end(); ++Itr);
		Itr->second->Update(l_Time);
	}
	else {
		m_StateVec.back().second->Update(l_Time);
	}
}

SharedContext* StateManager::GetContext() {
	return m_Shared;
}

bool StateManager::HasState(const StateType& l_Type){
	for (auto Itr = m_StateVec.begin(); Itr != m_StateVec.end(); Itr++) {
		if (Itr->first == l_Type) {
			auto removed = std::find(m_ToRemove.begin(), m_ToRemove.end(), l_Type);
			if (removed == m_ToRemove.end()) return true;
			return false;
		}
	}
	return false;
}

void StateManager::Remove(const StateType& l_Type) {
	m_ToRemove.push_back(l_Type);
}

void StateManager::ProcessRequest() {

	while (m_ToRemove.begin() != m_ToRemove.end()) {

		RemoveState(*m_ToRemove.begin());
		m_ToRemove.erase(m_ToRemove.begin());

	}
}

void StateManager::SwitchTo(const StateType& l_Type) {
	m_Shared->m_EventMgr->SetCurrentState(l_Type);

	for (auto Itr = m_StateVec.begin(); Itr != m_StateVec.end(); Itr++) {
		if (Itr->first == l_Type) {
			
			m_StateVec.back().second->Deactivate();
			StateType Tmp_Type = Itr->first;
			BaseState* Tmp_Base = Itr->second;
			m_StateVec.erase(Itr);
			m_StateVec.emplace_back(Tmp_Type, Tmp_Base);
			Tmp_Base->Activate();
			
			return;
		}
	}

	//State wasnt found
	if (!m_StateVec.empty()) { m_StateVec.back().second->Deactivate(); }
	CreateState(l_Type);
	m_StateVec.back().second->Activate();

}

void StateManager::CreateState(const StateType& l_Type) {
	auto NewState = m_StateFactory.find(l_Type);
	if (NewState == m_StateFactory.end()) return;
	BaseState* State = NewState->second();
	m_StateVec.emplace_back(l_Type, State);
	State->OnCreate();
}

void StateManager::RemoveState(const StateType& l_Type) {
	for (auto Itr = m_StateVec.begin();
		Itr != m_StateVec.end(); Itr++) 
	{
		if (Itr->first == l_Type) {
			Itr->second->OnDestroy();
			m_StateVec.erase(Itr);
			return;
		}
	}
}