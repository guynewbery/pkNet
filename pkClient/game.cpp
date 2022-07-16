#include "game.h"

#include <assert.h>
#include "netclient.h"
#include "phasestatemachine.h"
#include "raylib.h"

static int constexpr screenWidth = 960;
static int constexpr screenHeight = 540;

struct Game
{
    PhaseStateMachine m_PhaseStateMachine;
};

static Game * theGame;

void BeginGame()
{
    theGame = new Game();
    assert(theGame);

    NetClient::StartUpNetClient();

    InitWindow(screenWidth, screenHeight, "client");
    SetTargetFPS(60);
}

void EndGame()
{
    CloseWindow();

    NetClient::ShutDownNetClient();

    delete theGame;
}

void UpdateGame()
{
    NetClient::UpdateNetClient();

    theGame->m_PhaseStateMachine.UpdatePhaseStateMachine();    
}

void DrawGame()
{
    BeginDrawing();

    ClearBackground(BLACK);
    theGame->m_PhaseStateMachine.DrawPhaseStateMachine();
    
    EndDrawing();
}

bool GameShouldClose()
{
    return WindowShouldClose(); // Detect window close button or ESC key
}
