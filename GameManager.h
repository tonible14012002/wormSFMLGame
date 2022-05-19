

#pragma once
#include "EventManger.h"
#include "WindowManager.h"
#include "StateManager.h"
#include "SharedContext.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	void HandleInput();
	void Update();
	void Render();
	void DrawRec(EventDetails*);
	WindowManager* GetWindowManager();

	void ResetClock();
	sf::Time GetElapsed();

	void LateUpdate();

private:
	WindowManager m_WindManager;
	sf::Time m_Elapsed;
	sf::Clock m_Clock;
	sf::CircleShape m_Shape;
	StateManager m_StateManager;
	SharedContext m_Context;
};
