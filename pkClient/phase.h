#pragma once

class pkPhase
{
public:
    virtual void EnterPhase() = 0;
    virtual void ExitPhase() = 0;
    virtual void UpdatePhase() = 0;
    virtual void DrawPhase() = 0;
};
