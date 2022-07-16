#include "gamephase.h"

#include "chatbuffer.h"
#include "netclient.h"
#include "netconsts.h"
#include "raylib.h"

static ChatBuffer s_Chat(256);

void GamePhase::EnterPhase()
{
    s_Chat.ClearChatBuffer();
}

void GamePhase::ExitPhase()
{

}

void GamePhase::UpdatePhase()
{
    s_Chat.UpdateChatBuffer();

    if (s_Chat.GetChatBuffer().size() > 0 && IsKeyPressed(KEY_ENTER))
    {
        NetClient::SendChat(s_Chat.GetChatBuffer().c_str());
        s_Chat.ClearChatBuffer();
    }
}

void GamePhase::DrawPhase()
{    
    if (NetClient::IsConnected())
    {
        int yPos = 100;

        for (int i = 0; i < NetConsts::s_MaxPlayers; ++i)
        {
            if (NetClient::IsPlayerActive(i))
            {
                DrawText(NetClient::GetPlayerName(i), 50, yPos, 20, MAROON);
                DrawText(NetClient::GetPlayerChat(i), 250, yPos, 20, LIGHTGRAY);
                yPos += 50;
            }
        }

        DrawText(s_Chat.GetChatBuffer().c_str(), 100, 400, 20, LIGHTGRAY);
    }
}
