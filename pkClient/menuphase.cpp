#include "menuphase.h"

#include "chatbuffer.h"
#include "netclient.h"
#include "raylib.h"

static ChatBuffer s_Name(32);
static ChatBuffer s_Address(16);

void MenuPhase::EnterPhase()
{
    m_MenuState = MenuState::EnterName;

    s_Name.ClearChatBuffer();
    s_Address.ClearChatBuffer();
}

void MenuPhase::ExitPhase()
{

}

void MenuPhase::UpdatePhase()
{
    switch (m_MenuState)
    {
    case MenuState::EnterName:
    {
        s_Name.UpdateChatBuffer();

        if (s_Name.GetChatBuffer().size() > 0 && IsKeyPressed(KEY_ENTER))
        {
            NetClient::SetLocalPlayerName(s_Name.GetChatBuffer().c_str());
            m_MenuState = MenuState::PostEnterNamePause;
            m_Delay = 1.f;
        }
        break;
    }

    case MenuState::PostEnterNamePause:
    {
        if (m_Delay > 0.f)
        {
            m_Delay -= .03f;
        }
        else
        {
            m_MenuState = MenuState::EnterAddress;
        }
        break;
    }

    case MenuState::EnterAddress:
    {
        s_Address.UpdateChatBuffer();

        if (s_Address.GetChatBuffer().size() > 0 && IsKeyPressed(KEY_ENTER))
        {
            NetClient::ConnectToServer(s_Address.GetChatBuffer().c_str()); //"127.0.0.1"
            m_MenuState = MenuState::PostEnterAddressPause;
        }
        break;
    }

    case MenuState::PostEnterAddressPause:
    {
        break;
    }
    }
}

void MenuPhase::DrawPhase()
{    
    switch (m_MenuState)
    {
    case MenuState::EnterName:
    {
        DrawText("What is your name?", 190, 200, 20, LIGHTGRAY);
        DrawText(s_Name.GetChatBuffer().c_str(), 300, 300, 40, MAROON);
        break;
    }

    case MenuState::PostEnterNamePause:
    {
        break;
    }

    case MenuState::EnterAddress:
    {
        DrawText("What is your destination?", 190, 200, 20, LIGHTGRAY);
        DrawText(s_Address.GetChatBuffer().c_str(), 300, 300, 40, MAROON);
        break;
    }

    case MenuState::PostEnterAddressPause:
    {
        break;
    }
    }
}
