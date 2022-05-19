#pragma once
#include "BaseState.h"
#include "StateManager.h"

class State_MainMenu: public BaseState
{
public:

	State_MainMenu(StateManager* l_StateManager);
	~State_MainMenu();
	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_Time);
	void Draw();

	void MouseClick(EventDetails* l_Detail);

private:

	sf::Text m_Text;
	sf::Vector2f m_ButtonSize;
	sf::Vector2f m_ButtonPos;
	unsigned int m_ButtonPadding;
	sf::Vector2i Mouse;
	sf::RectangleShape m_Rects[3];
	sf::Text m_Labels[3];
	sf::Font m_Font;

	sf::View m_View;
};

