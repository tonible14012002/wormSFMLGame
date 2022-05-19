#include "EventManger.h"

EventManager::EventManager() :m_HasFocus(true) {
	LoadBinding();
}

EventManager::~EventManager() {
	for (auto& Itr : m_Bindings) {
		delete Itr.second;
		Itr.second = nullptr;
	}
}

void EventManager::SetFocus(const bool& l_Focus) { m_HasFocus = l_Focus; }

void EventManager::SetCurrentState(const StateType& l_Type) {
	m_CurrentState = l_Type;
}

bool EventManager::AddBinding(Binding* l_Bind) {
	if (m_Bindings.find(l_Bind->m_BindName) != m_Bindings.end()) {
		return false;
	}
	return m_Bindings.emplace(l_Bind->m_BindName,
		l_Bind).second;
}

bool EventManager::RemoveBinding(const std::string& l_Name) {
	auto Itr = m_Bindings.find(l_Name);
	if (Itr == m_Bindings.end()) return false;
	delete Itr->second;
	m_Bindings.erase(Itr);
	return true;
}

void EventManager::HandleEvent(sf::Event& l_Event) {
	//Handling SFML event.
	for (auto& Itr : m_Bindings) {
		auto& BindCur = Itr.second;
		for (auto& EventCur : BindCur->m_Events) {
			EventType sfmlEvent = (EventType)l_Event.type;
			if (EventCur.first != sfmlEvent) { continue; }
			if (EventCur.first == EventType::KeyDown ||
				EventCur.first == EventType::KeyUp) {
				if (EventCur.second.m_Code == l_Event.key.code) {
					if (BindCur->m_EventDetails.m_KeyCode != -1) {
						BindCur->m_EventDetails.m_KeyCode =
							EventCur.second.m_Code;
					}
					++(BindCur->C);
					break;
				}
			}
			else if (EventCur.first == EventType::MButtonDown ||
				EventCur.first == EventType::MButtonUp) {
				if (EventCur.second.m_Code == l_Event.mouseButton.button) {
					BindCur->m_EventDetails.m_Mouse.x = l_Event.mouseButton.x;
					BindCur->m_EventDetails.m_Mouse.y = l_Event.mouseButton.y;
					if (BindCur->m_EventDetails.m_KeyCode != -1) {
						BindCur->m_EventDetails.m_KeyCode = EventCur.second.m_Code;
					}
					++(BindCur->C);
					break;
				}
			}
			else {
				if (EventCur.first == EventType::MouseWheel) {
					BindCur->m_EventDetails.m_MouseWheelDelta =
						l_Event.mouseWheel.delta;
				}
				else if (EventCur.first == EventType::WindowResized) {
					BindCur->m_EventDetails.m_Sized.x = l_Event.size.width;
					BindCur->m_EventDetails.m_Sized.y = l_Event.size.height;
				}
				else if (EventCur.first == EventType::TextEntered) {
					BindCur->m_EventDetails.m_TextEntered =
						l_Event.text.unicode;
				}
				++(BindCur->C);
			}
		}
	}


}

void EventManager::Update() {
	if (!m_HasFocus) return;
	for (auto& Itr : m_Bindings) {
		Binding* BindCur = Itr.second;
		for (auto& Ecur : BindCur->m_Events) {
			switch (Ecur.first)
			{
			case(EventType::Keyboard):
				if (sf::Keyboard::isKeyPressed(
					sf::Keyboard::Key(Ecur.second.m_Code))) {
					if (BindCur->m_EventDetails.m_KeyCode != -1) {
						BindCur->m_EventDetails.m_KeyCode = Ecur.second.m_Code;
					}
					++(BindCur->C);
				}
				break;
			case(EventType::Mouse):
				if (sf::Mouse::isButtonPressed(
					sf::Mouse::Button(Ecur.second.m_Code))) {
					if (BindCur->m_EventDetails.m_KeyCode != -1) {
						BindCur->m_EventDetails.m_KeyCode = Ecur.second.m_Code;
					}
					++(BindCur->C);
				}
				break;
			case(EventType::Joystick):
				break;
			}
		}
		if (BindCur->m_Events.size() == BindCur->C) {
			auto  StateCallbacks = m_Callbacks.find(m_CurrentState);
			auto OtherCallbacks = m_Callbacks.find(StateType(0));

			if (StateCallbacks != m_Callbacks.end()) {
				auto CallItr = StateCallbacks->second.find(BindCur->m_BindName);
				if (CallItr != StateCallbacks->second.end()) {
					CallItr->second(&BindCur->m_EventDetails);
				}
			}
			if (OtherCallbacks != m_Callbacks.end()) {
				auto CallItr = OtherCallbacks->second.find(BindCur->m_BindName);
				if (CallItr != OtherCallbacks->second.end()) {
					CallItr->second(&BindCur->m_EventDetails);
				}
			}
			
		}
		BindCur->C = 0;
		BindCur->m_EventDetails.Clear();
	}
}

void EventManager::LoadBinding() {
	std::string delimeter = ":";

	std::fstream BindFile;
	BindFile.open("config/keyconfig.cfg");
	if (!BindFile.is_open()) {
		std::cout << "Can not open config file!\n"; 
		return;
	}
	std::string Line;
	while (std::getline(BindFile, Line)) {
		std::stringstream Keystream(Line);
		std::string CallbackName;
		Keystream >> CallbackName;
		Binding* Bind = new Binding(CallbackName);
		while (!Keystream.eof()) {
			std::string KeyVal;
			Keystream >> KeyVal;
			int Start = 0;
			int End = KeyVal.find(delimeter);
			if (End == std::string::npos) {
				delete Bind;
				Bind = nullptr;
				break;
			}
			EventType Type = EventType(
				std::stoi(KeyVal.substr(Start, End - Start)));
			int Code = std::stoi(KeyVal.substr(End + delimeter.length(),
				KeyVal.find(delimeter, End + delimeter.length())));
			EventInfo EInfo;
			EInfo.m_Code = Code;

			Bind->BindEvent(Type, EInfo);
		}
		if (!AddBinding(Bind)) { delete Bind; }
		Bind = nullptr;
	}
	BindFile.close();
}