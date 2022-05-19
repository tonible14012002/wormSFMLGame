#include "State_MainMenu.h"
#include "EventManger.h"

State_MainMenu::State_MainMenu(StateManager* l_StateManager) :BaseState(l_StateManager) {};

State_MainMenu::~State_MainMenu(){}

void State_MainMenu::OnCreate() {

	m_Font.loadFromFile("State/MainMenu/MenuFont.ttf");
	m_Text.setFont(m_Font);
	m_Text.setString(sf::String("MAIN MENU:"));
	m_Text.setCharacterSize(30);

	sf::Vector2u WindSize = m_StateMgr->GetContext()->
		m_Windmgr->GetWindowSize();

	sf::FloatRect textRect = m_Text.getLocalBounds();
	m_Text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	m_Text.setPosition(WindSize.x/2.0f, 200);

	m_ButtonSize = sf::Vector2f(500, 50);
	m_ButtonPos = sf::Vector2f(WindSize.x/2.0f, 300);
	m_ButtonPadding = 4; // 4px

	std::string str[3];
	str[0] = "PLAY";
	str[1] = "CREDITS";
	str[2] = "EXIT";

	for(int i = 0; i < 3; i++) {
		sf::Vector2f ButtonPosition(m_ButtonPos.x, m_ButtonPos.y +
			(i * (m_ButtonSize.y + m_ButtonPadding)));
		m_Rects[i].setSize(m_ButtonSize);

		m_Rects[i].setOrigin(m_ButtonSize.x / 2.0f, m_ButtonSize.y / 2.0f);
		m_Rects[i].setPosition(ButtonPosition);

		m_Labels[i].setFont(m_Font);
		m_Labels[i].setCharacterSize(20);
		m_Labels[i].setString(sf::String(str[i]));

		sf::FloatRect rect = m_Labels[i].getLocalBounds();
		m_Labels[i].setOrigin(rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);

		m_Labels[i].setPosition(ButtonPosition);
	}

	EventManager* evMgr = m_StateMgr->GetContext()->m_EventMgr;
	evMgr->AddCallback(StateType::MainMenu, "Mouse_Left",
		&State_MainMenu::MouseClick, this);

	m_View = m_StateMgr->GetContext()->m_Windmgr->GetRenderWindow()->getDefaultView();

}

void State_MainMenu::OnDestroy() {
	EventManager* evMgr = m_StateMgr->GetContext()->m_EventMgr;
	evMgr->RemoveCallback(StateType::MainMenu, "Mouse_Left");
}

void State_MainMenu::Activate() {

	if (m_StateMgr->HasState(StateType::Game) &&
		m_Labels[0].getString() == "PLAY") {
		
		m_Labels[0].setString(sf::String("RESUME"));
		sf::FloatRect rect = m_Labels[0].getLocalBounds();
		m_Labels[0].setOrigin(rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);
	}
}

void State_MainMenu::Deactivate() {};

void State_MainMenu::Update(const sf::Time& l_Time) {
	Mouse = m_StateMgr->GetContext()->m_EventMgr->
		GetMousePos(m_StateMgr->GetContext()->m_Windmgr->GetRenderWindow());

	float halfX = m_ButtonSize.x / 2.0f;
	float halfY = m_ButtonSize.y / 2.0f;

	for (int i = 0; i < 3; i++) {
		if (Mouse.x >= m_Rects[i].getPosition().x - halfX &&
			Mouse.x <= m_Rects[i].getPosition().x + halfX &&
			Mouse.y >= m_Rects[i].getPosition().y - halfY &&
			Mouse.y <= m_Rects[i].getPosition().y + halfY)
		{
			m_Rects[i].setFillColor(sf::Color(145, 6, 6));
		}
		else
		{
			m_Rects[i].setFillColor(sf::Color::Red);
		}
	}
};

void State_MainMenu::MouseClick(EventDetails* l_Detail) {
		
	sf::Vector2i MousePos = l_Detail->m_Mouse;

	float halfX = m_ButtonSize.x / 2.0f;
	float halfY = m_ButtonSize.y / 2.0f;

	for (int i = 0; i < 3; i++) {
		if (MousePos.x >= m_Rects[i].getPosition().x - halfX &&
			MousePos.x <= m_Rects[i].getPosition().x + halfX &&
			MousePos.y >= m_Rects[i].getPosition().y - halfY &&
			MousePos.y <= m_Rects[i].getPosition().y + halfY) 
		{

			if (i == 0 /*&& m_StateMgr->HasState(StateType::Game)*/) {
				m_StateMgr->SwitchTo(StateType::SubMenu);
			}
			else if (i == 1) {
				//State Credits.
			}
			else if (i == 2) {
				m_StateMgr->GetContext()->m_Windmgr->Close(l_Detail);
			}
		}
	}
}

void State_MainMenu::Draw() {
	sf::RenderWindow* Wind = m_StateMgr->GetContext()->
		m_Windmgr->GetRenderWindow();
	Wind->setView(m_View);

	Wind->draw(m_Text);
	for (int i = 0; i < 3; i++) {
		Wind->draw(m_Rects[i]);
		Wind->draw(m_Labels[i]);
	}
}