#pragma once
#include <SFML/Graphics.hpp>

class StateManager;

class BaseState
{
	friend class StateManager;
public:
	BaseState(StateManager* l_StateManager)
		:m_StateMgr(l_StateManager), m_Transcendent(false),
		m_Transparent(false) {};
	virtual ~BaseState() {};

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& l_Time) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool& l_Transparent) {
		m_Transparent = l_Transparent;
	}
	bool IsTransparent() { return m_Transparent; }

	void SetTrancendent(const bool& l_Transcendent) {
		m_Transcendent = l_Transcendent;
	}
	bool IsTranscendent() { return m_Transcendent; }

protected:
	StateManager* m_StateMgr;
	bool m_Transparent;
	bool m_Transcendent;
};
