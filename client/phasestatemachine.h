#pragma once

class pkPhase;

enum class PhaseIds
{
    Menu,
    Game,
    Count
};

class PhaseStateMachine
{
public:
    PhaseStateMachine();
    ~PhaseStateMachine();

    void UpdatePhaseStateMachine();
    void DrawPhaseStateMachine();

private:
    pkPhase * m_Phases[static_cast<int>(PhaseIds::Count)];
    PhaseIds m_CurrentPhase = PhaseIds::Count;
    PhaseIds m_NextPhase = PhaseIds::Menu;
};
