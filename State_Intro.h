#pragma once
#include <SFML/Graphics.hpp>
#include "StateManager.h"
#include "BaseState.h"

class State_Intro : public BaseState
{
public:
	State_Intro(StateManager* l_StateManager);
	~State_Intro();
	void OnCreate();
	void OnDestroy();

	void Activate() {};
	void Deactivate() {};
	void Update(const sf::Time& l_Time);
	void Draw();
	void Continue(EventDetails* l_Detail);
private:
	sf::Texture m_IntroTexture;
	sf::Sprite m_IntroSprite;
	sf::Text m_Text;
	float m_TimePasted;
	sf::Font m_Font;
	sf::View m_IntroView;
};

