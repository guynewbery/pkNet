#pragma once

namespace NetClient
{
    int StartUpNetClient();
    void ShutDownNetClient();

    int ConnectToServer(const char *ipAdress); //"127.0.0.1"
    bool IsConnected();
    void UpdateNetClient();

    bool IsPlayerActive(int playerIdx);
    const char* GetPlayerName(int playerIdx);
    const char* GetPlayerChat(int playerIdx);

    void SetLocalPlayerName(const char *name);
    void SendChat(const char *name);
};
