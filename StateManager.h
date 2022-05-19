#pragma once
#include<SFML/Graphics.hpp>
#include<functional>
#include"WindowManager.h"
#include"SharedContext.h"
#include "State_Intro.h"
#include "State_MainMenu.h"
#include <memory>

enum class StateType {
	Intro = 1, MainMenu, SubMenu, Game, Credits, Paused
};

using StateContainer = std::vector<std::pair<StateType,BaseState*>>;

using TypeContainer = std::vector<StateType>;

using StateFactory = std::unordered_map<
	StateType, std::function<BaseState*(void)>>;

class StateManager {
public:
	StateManager(SharedContext* l_Share);
	~StateManager();

	void Update(const sf::Time& l_Time);
	void Draw();

	void ProcessRequest();

	SharedContext* GetContext();
	bool HasState(const StateType& l_State);
	
	void SwitchTo(const StateType& l_State);
	void Remove(const StateType& l_State);



private:
	//Methods.
	void CreateState(const StateType& l_State);
	void RemoveState(const StateType& l_State);

	template<class T>
	void RegisterState(const StateType& l_State) {
		m_StateFactory[l_State] = [this](void)-> BaseState* {
			return new T(this);
		};
	};

	SharedContext* m_Shared;
	StateContainer m_StateVec;
	TypeContainer m_ToRemove;
	StateFactory m_StateFactory;
};