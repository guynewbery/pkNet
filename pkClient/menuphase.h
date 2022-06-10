#pragma once

#include "phase.h"

class MenuPhase : public pkPhase
{
public:
    virtual void EnterPhase() override;
    virtual void ExitPhase() override;
    virtual void UpdatePhase() override;
    virtual void DrawPhase() override;

private:
    enum class MenuState : int
    {
        EnterName,
        PostEnterNamePause,
        EnterAddress,
        PostEnterAddressPause,
    };

    MenuState m_MenuState = MenuState::EnterName;

    float m_Delay = 0.f;
};
