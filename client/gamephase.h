#pragma once

#include "phase.h"

class GamePhase : public pkPhase
{
public:
    virtual void EnterPhase() override;
    virtual void ExitPhase() override;
    virtual void UpdatePhase() override;
    virtual void DrawPhase() override;

private:
};
