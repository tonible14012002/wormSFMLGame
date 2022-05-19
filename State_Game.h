#pragma once
#include "BaseState.h"
#include "StateManager.h"
#include "World.h"

class State_Game : public BaseState
{
public:
	State_Game(StateManager* l_Base);
	~State_Game();

	void OnCreate();
	void OnDestroy();
	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_Time);
	void Draw();

	void Back_to_Menu(EventDetails* l_Detial);
	void Paused(EventDetails* l_Detail);

	void AddDebris(EventDetails* l_Detail);
	
	void ControlView(EventDetails *l_Detail);

	void ControlCrosshair(EventDetails* l_Detail);
	void Jump(EventDetails* l_Detail);
	void FireMissile(EventDetails* l_detial);
private:

	World m_GameWorld;

};

