#pragma once
#include "EventManger.h"
#include <string>

class WindowManager
{
public:
	WindowManager(const std::string& l_Title, const sf::Vector2u& l_Size);
	WindowManager();
	~WindowManager();

	void BeginDraw();
	void Draw(sf::Drawable& l_Drawable);
	void EndDraw();

	bool IsDone();
	bool IsFullscreen();
	void ToggleFullscreen(EventDetails*);
	void Close(EventDetails*);

	sf::Vector2u GetWindowSize();
	EventManager* GetEventManager();
	sf::RenderWindow* GetRenderWindow();

	void Update();
private:
	void Create();
	void Setup(const std::string& l_Title, const sf::Vector2u& l_Size);
	void Destroy();


	std::string m_Title;
	sf::Vector2u m_WindSize;
	sf::RenderWindow m_RenderWind;
	bool m_IsFocus;
	bool  m_IsDone;
	bool m_IsFullscreen;
	EventManager m_EManager;
};

