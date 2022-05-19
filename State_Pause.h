#pragma once
#include "StateManager.h"
#include "BaseState.h"

class State_Pause : public BaseState
{
public:
	State_Pause(StateManager* l_Manager);
	~State_Pause();

	void OnCreate();
	void OnDestroy();

	void Update(const sf::Time& l_Time);
	void Activate();
	void Deactivate();

	void Draw();
	void Resume(EventDetails* l_Detail);

private:
	sf::Text m_Text;
	sf::Font m_Font;
	sf::RectangleShape m_BackGround;
};

