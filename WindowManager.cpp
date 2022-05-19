#include "WindowManager.h"

WindowManager::WindowManager() {
	Setup("Window", sf::Vector2u(640, 480));
}

WindowManager::WindowManager(const std::string& l_Title, const sf::Vector2u& l_Size) {
	Setup(l_Title, l_Size);
}

WindowManager::~WindowManager() {
	Destroy();
}

void WindowManager::Setup(const std::string& l_Title, const sf::Vector2u& l_Size) {
	m_Title = l_Title;
	m_WindSize = l_Size;
	m_IsFocus = true;
	m_IsDone = false;
	m_IsFullscreen = false;

	m_EManager.AddCallback(StateType(0), "Fullscreen_toggle", &WindowManager::ToggleFullscreen, this);
	m_EManager.AddCallback(StateType(0), "Window_close", &WindowManager::Close, this);
	Create();
}

void WindowManager::Create() {
	std::cout << "Creating Window..\n";
	auto setStyle = m_IsFullscreen ? sf::Style::Fullscreen : sf::Style::Default;
	m_RenderWind.create(sf::VideoMode(m_WindSize.x, m_WindSize.y), m_Title, setStyle);
	std::cout << "Window Created!\n";
}

void WindowManager::Destroy() { m_RenderWind.close(); }

void WindowManager::ToggleFullscreen(EventDetails* l_Detail) {
	
	m_IsFullscreen = !m_IsFullscreen;
	std::cout << "Toggle Fullscreen!\n";
	Destroy();
	Create();
}

void WindowManager::BeginDraw() { m_RenderWind.clear(); }

void WindowManager::Draw(sf::Drawable& l_Drawable) {
	m_RenderWind.draw(l_Drawable);
}

void WindowManager::EndDraw() {
	m_RenderWind.display();
}

void WindowManager::Close(EventDetails* l_Detail = nullptr) {
	m_IsDone = true;
}

sf::Vector2u WindowManager::GetWindowSize() { return m_WindSize; }

bool WindowManager::IsDone() { return m_IsDone; }

bool WindowManager::IsFullscreen() { return m_IsFullscreen; }

EventManager* WindowManager::GetEventManager() {
	return &m_EManager;
}

sf::RenderWindow* WindowManager::GetRenderWindow() { return&m_RenderWind; }

void WindowManager::Update() {
	sf::Event event;
	while (m_RenderWind.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_IsDone = true;
		}
		if (event.type == sf::Event::LostFocus) {
			m_IsFocus = false;
			m_EManager.SetFocus(false);
		}
		else if (event.type == sf::Event::GainedFocus) {
			m_IsFocus = true;
			m_EManager.SetFocus(true);
		}
		m_EManager.HandleEvent(event);
	}
	m_EManager.Update();
}