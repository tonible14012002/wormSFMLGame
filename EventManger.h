#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <unordered_map>
#include <fstream>


enum class StateType;
enum class EventType
{
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1,
	Mouse, Joystick

};

struct EventInfo {
	EventInfo() { m_Code = 0; }
	EventInfo(int l_EventCode) {
		m_Code = l_EventCode;
	}
	union
	{
		int m_Code;
	};
};

using EventVec = std::vector<std::pair<EventType, EventInfo>>;

struct EventDetails {

	EventDetails(const std::string& l_Name) :m_BindName(l_Name) {
		Clear();
	}

	void Clear() {
		m_KeyCode = -1;
		m_Sized = sf::Vector2u(0, 0);
		m_Mouse = sf::Vector2i(0, 0);
		m_TextEntered = 0;
		m_MouseWheelDelta = 0;
	}

	std::string m_BindName;
	sf::Vector2i m_Mouse;
	sf::Vector2u m_Sized;
	sf::Uint32 m_TextEntered;
	int m_MouseWheelDelta;
	int m_KeyCode;
};

struct Binding {

	Binding(const std::string& l_Name) :
		m_BindName(l_Name),
		m_EventDetails(l_Name), C(0) {}
	void BindEvent(EventType l_Type,
		EventInfo l_Info = EventInfo()) {
		m_Events.emplace_back(l_Type, l_Info);
	}

	EventVec m_Events;
	std::string m_BindName;
	EventDetails m_EventDetails;
	int C;
};

using BindingMap = std::unordered_map<std::string, Binding*>;

using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;

using CallbackMap = std::unordered_map<StateType, CallbackContainer>;

class EventManager {

public:
	EventManager();
	~EventManager();

	bool AddBinding(Binding* l_Bind);
	bool RemoveBinding(const std::string& l_Name);

	void SetFocus(const bool& l_Focus);

	template<class T>
	bool AddCallback(StateType l_Type, const std::string& l_Name,
		void (T::* l_Function)(EventDetails*), T* l_Instance) {

		auto Itr = m_Callbacks.emplace(l_Type, CallbackContainer()).first;
		auto Temp = std::bind(l_Function, l_Instance, std::placeholders::_1);
		return Itr->second.emplace(l_Name, Temp).second;
	}

	bool RemoveCallback(StateType l_Type, const std::string& l_Name) {
		auto Itr = m_Callbacks.find(l_Type);
		if (Itr == m_Callbacks.end()) { return false; }
		auto Itr2 = Itr->second.find(l_Name);
		if (Itr2 == Itr->second.end()) { return false; }
		Itr->second.erase(l_Name);
		return true;
	}

	void HandleEvent(sf::Event& l_Event);
	void Update();

	sf::Vector2i GetMousePos(sf::RenderWindow* l_Wind = nullptr) {
		return (l_Wind ? sf::Mouse::getPosition(*l_Wind) :
			sf::Mouse::getPosition(*l_Wind));
	}
	void SetCurrentState(const StateType& l_Type);
private:
	void LoadBinding();

	BindingMap m_Bindings;
	CallbackMap m_Callbacks;
	bool m_HasFocus;
	StateType m_CurrentState;
};
