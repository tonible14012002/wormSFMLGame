#include "State_Pause.h"
#include "EventManger.h"

State_Pause::State_Pause(StateManager* l_Manager):BaseState(l_Manager){}

State_Pause::~State_Pause(){}

void State_Pause::OnCreate() {
	SetTransparent(true);
	m_Font.loadFromFile("State/Share/Font.ttf");
	m_Text.setFont(m_Font);
	m_Text.setString(sf::String("PAUSED"));
	m_Text.setCharacterSize(15);
	sf::FloatRect TextRect = m_Text.getLocalBounds();
	m_Text.setOrigin(TextRect.left + TextRect.width / 2.0f,
		TextRect.top +  TextRect.height / 2.0f);

	sf::Vector2u l_WindSize = m_StateMgr->GetContext()
		->m_Windmgr->GetWindowSize();
	m_Text.setPosition(l_WindSize.x / 2, l_WindSize.y / 2);
	m_Text.setFillColor(sf::Color::White);

	m_BackGround.setSize(sf::Vector2f(l_WindSize));
	m_BackGround.setFillColor(sf::Color(0, 0, 0, 150));
	m_BackGround.setPosition(0, 0);

	//Add callbacks here.
	EventManager* l_EvMgr = m_StateMgr->GetContext()->m_EventMgr;
	l_EvMgr->AddCallback(StateType::Paused, "Key_P", &State_Pause::Resume, this);
}

void State_Pause::OnDestroy() {
	//remove callbacks here.
	EventManager* l_EvMgr = m_StateMgr->GetContext()->m_EventMgr;
	l_EvMgr->RemoveCallback(StateType::Paused, "Key_P");
}

void State_Pause::Activate(){}
void State_Pause::Deactivate(){}

void State_Pause::Update(const sf::Time& l_Time) {}

void State_Pause::Draw() {
	sf::RenderWindow *Wind = m_StateMgr->GetContext()->m_Windmgr->GetRenderWindow();
	Wind->setView(Wind->getDefaultView());
	Wind->draw(m_BackGround);
	Wind->draw(m_Text);
}

void State_Pause::Resume(EventDetails* l_Detail) {
	m_StateMgr->SwitchTo(StateType::Game);

}