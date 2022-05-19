#pragma once
#include"WindowManager.h"
#include "EventManger.h"

struct SharedContext {
	SharedContext() : m_Windmgr(nullptr), m_EventMgr(nullptr) {};
	WindowManager* m_Windmgr;
	EventManager* m_EventMgr;
};
