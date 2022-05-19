#include "State_Intro.h"

State_Intro::State_Intro(StateManager* l_StateManager):BaseState(l_StateManager){}

State_Intro::~State_Intro() { }

void State_Intro::OnCreate() {

	m_TimePasted = 0.0f;
	sf::Vector2u WindowSize = m_StateMgr->GetContext()->m_Windmgr->GetWindowSize();
	if (!m_IntroTexture.loadFromFile("State/Intro/IntroTexture.png")) {
		std::cout << "Faild Loading intro texture!\n";
	}
	
	m_IntroSprite.setTexture(m_IntroTexture);
	m_IntroSprite.setOrigin(m_IntroTexture.getSize().x / 2.0f,
		m_IntroTexture.getSize().y / 2.0f);
	m_IntroSprite.setPosition(WindowSize.x / 2.0f, 0);
	
	if (!m_Font.loadFromFile("State/Intro/IntroFont.ttf")) {
		std::cout << "Faild Loading Intro Font!";
	}

	m_Text.setFont(m_Font);
	m_Text.setString("Press SPACE to continue");
	m_Text.setCharacterSize(15);

	sf::FloatRect TextRect = m_Text.getLocalBounds();

	m_Text.setOrigin(TextRect.left + TextRect.width / 2.0f,
		TextRect.top + TextRect.height / 2.0f);
	m_Text.setPosition(WindowSize.x / 2.0f, WindowSize.y / 2.0f);

	EventManager* evMgr = m_StateMgr->GetContext()->m_EventMgr;
	evMgr->AddCallback(StateType::Intro,"Intro_Continue", &State_Intro::Continue, this);

	sf::RenderWindow* RenderWin = m_StateMgr->GetContext()->m_Windmgr->GetRenderWindow();
	m_IntroView = RenderWin->getDefaultView();
}

void State_Intro::OnDestroy(){
	EventManager* evMgr = m_StateMgr->GetContext()->m_EventMgr;
	evMgr->RemoveCallback(StateType::Intro, "Intro_Continue");
}

void State_Intro::Update(const sf::Time& l_Time) {
	if (m_TimePasted < 3) {
		m_TimePasted += l_Time.asSeconds();
		m_IntroSprite.setPosition(m_IntroSprite.getPosition().x,
			m_IntroSprite.getPosition().y + (70 * (l_Time.asSeconds())));
	}
}

void State_Intro::Draw() {
	sf::RenderWindow* Wind = m_StateMgr->GetContext()->
		m_Windmgr->GetRenderWindow();

	Wind->setView(m_IntroView);

	Wind->draw(m_IntroSprite);

	if (m_TimePasted >= 3.0f) {
		Wind->draw(m_Text);
	}
}

void State_Intro::Continue(EventDetails* l_Detail) {

	if (m_TimePasted >= 3.0f) {
		m_StateMgr->SwitchTo(StateType::MainMenu);
		m_StateMgr->Remove(StateType::Intro);
	}
	else {
		m_TimePasted = 3;
		m_IntroSprite.setPosition(m_IntroSprite.getPosition().x,
			70*3);
	}
}