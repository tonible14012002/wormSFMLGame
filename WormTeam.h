#include "Worm.h"
#include "Define.h"

class WormTeam {
public:
	WormTeam(int l_Size) {
		m_TeamSize = l_Size;
	}

	bool IsTeamAlive() {

		bool AllDeath = true;
		for (auto& W : m_MemberVec) {
			if (W->m_Health > 0.0f) {
				AllDeath = false;
			}
		}
		return !AllDeath;

	}

	float GetTeamHealth() {
		float TotalHealth = 0;
		for (auto W : m_MemberVec) {
			TotalHealth += W->m_Health;
		}
		return TotalHealth;
	}

	Worm* GetNextMember() {
		do {
			m_CurrentMember++;
			if (m_CurrentMember >= m_TeamSize) m_CurrentMember = 0; 
		} while (m_MemberVec[m_CurrentMember]->m_Health <= 0);
			return m_MemberVec[m_CurrentMember];
	}

	int m_TeamSize = 0;
	int m_CurrentMember = 0;
	std::vector<Worm*> m_MemberVec;


};