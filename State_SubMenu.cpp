#include "State_SubMenu.h"


State_SubMenu::State_SubMenu(StateManager* l_Base) : BaseState(l_Base){}

State_SubMenu::~State_SubMenu(){}

void State_SubMenu::OnCreate() {

	m_Font.loadFromFile("State/MainMenu/MenuFont.ttf");
	m_Text.setFont(m_Font);
	m_Text.setString(sf::String("PLAY MODE"));
	m_Text.setCharacterSize(30);

	sf::Vector2u WindSize = m_StateMgr->GetContext()->
		m_Windmgr->GetWindowSize();

	sf::FloatRect textRect = m_Text.getLocalBounds();
	m_Text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	m_Text.setPosition(WindSize.x / 2.0f, 200);

	m_ButtonSize = sf::Vector2f(500, 50);
	m_ButtonPos = sf::Vector2f(WindSize.x / 2.0f, 300);
	m_ButtonPadding = 4; // 4px

	std::string str[4];
	str[0] = "2Player";
	str[1] = "3Player (not available)";
	str[2] = "VS AI (not available)";
	str[3] = "Back";

	for (int i = 0; i < 4; i++) {
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

	EventManager* l_EMgr = m_StateMgr->GetContext()->m_EventMgr;
	l_EMgr->AddCallback(StateType::SubMenu, "Mouse_Left", &State_SubMenu::MouseClick, this);

	m_View = m_StateMgr->GetContext()->m_Windmgr->GetRenderWindow()->getDefaultView();
}

void State_SubMenu::OnDestroy() {
	EventManager* evMgr = m_StateMgr->GetContext()->m_EventMgr;
	evMgr->RemoveCallback(StateType::MainMenu, "Mouse_Left");
}

void State_SubMenu::Activate() {

}

void State_SubMenu::Deactivate() {

}

void State_SubMenu::Update(const sf::Time& l_Time) {
	Mouse = m_StateMgr->GetContext()->m_EventMgr->
		GetMousePos(m_StateMgr->GetContext()->m_Windmgr->GetRenderWindow());

	float halfX = m_ButtonSize.x / 2.0f;
	float halfY = m_ButtonSize.y / 2.0f;

	for (int i = 0; i < 4; i++) {
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
}

void State_SubMenu::Draw() {
	sf::RenderWindow* Wind = m_StateMgr->GetContext()->
		m_Windmgr->GetRenderWindow();
	Wind->setView(m_View);

	Wind->draw(m_Text);
	for (int i = 0; i < 4; i++) {
		Wind->draw(m_Rects[i]);
		Wind->draw(m_Labels[i]);
	}
}

void State_SubMenu::MouseClick(EventDetails* l_Detail) {

	sf::Vector2i MousePos = l_Detail->m_Mouse;

	float halfX = m_ButtonSize.x / 2.0f;
	float halfY = m_ButtonSize.y / 2.0f;

	for (int i = 0; i < 4; i++) {
		if (MousePos.x >= m_Rects[i].getPosition().x - halfX &&
			MousePos.x <= m_Rects[i].getPosition().x + halfX &&
			MousePos.y >= m_Rects[i].getPosition().y - halfY &&
			MousePos.y <= m_Rects[i].getPosition().y + halfY)
		{
			if (i == 3) m_StateMgr->SwitchTo(StateType::MainMenu);
			else if (i == 0)
			{
				m_StateMgr->SwitchTo(StateType::Game);
			}
		}
	}
}