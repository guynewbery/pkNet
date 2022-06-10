#include "phasestatemachine.h"

#include "phase.h"
#include "menuphase.h"
#include "gamephase.h"
#include "netclient.h"

PhaseStateMachine::PhaseStateMachine()
{
    m_Phases[static_cast<int>(PhaseIds::Menu)] = new MenuPhase();
    m_Phases[static_cast<int>(PhaseIds::Game)] = new GamePhase();
    m_Phases[static_cast<int>(PhaseIds::Count)] = nullptr;
}

PhaseStateMachine::~PhaseStateMachine()
{
    for (int i = 0; i < static_cast<int>(PhaseIds::Count); ++i)
    {
        delete m_Phases[i];
    }
}

void PhaseStateMachine::UpdatePhaseStateMachine()
{
    switch (m_CurrentPhase)
    {
    case PhaseIds::Menu:
    {
        if (NetClient::IsConnected())
        {
            m_NextPhase = PhaseIds::Game;
        }
        break;
    }

    case PhaseIds::Game:
    {
        if (!NetClient::IsConnected())
        {
            m_NextPhase = PhaseIds::Menu;
        }
        break;
    }
    }

    if (m_CurrentPhase != m_NextPhase)
    {
        if (m_Phases[static_cast<int>(m_CurrentPhase)])
        {
            m_Phases[static_cast<int>(m_CurrentPhase)]->ExitPhase();
        }

        m_CurrentPhase = m_NextPhase;

        if (m_Phases[static_cast<int>(m_CurrentPhase)])
        {
            m_Phases[static_cast<int>(m_CurrentPhase)]->EnterPhase();
        }
    }

    if (m_Phases[static_cast<int>(m_CurrentPhase)])
    {
        m_Phases[static_cast<int>(m_CurrentPhase)]->UpdatePhase();
    }
}

void PhaseStateMachine::DrawPhaseStateMachine()
{
    if (m_Phases[static_cast<int>(m_CurrentPhase)])
    {
        m_Phases[static_cast<int>(m_CurrentPhase)]->DrawPhase();
    }
}
