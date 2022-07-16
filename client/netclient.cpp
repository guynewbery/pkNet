#include "netclient.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#if (_WIN32_WINNT < 0x0601)
	#undef _WIN32_WINNT
    #define _WIN32_WINNT 0x0601
#endif

#define ENET_IMPLEMENTATION
#include "enet.h"

#include "netcmd.h"
#include "netconsts.h"
#include <assert.h>
#include <string>

struct NetPlayer
{
    bool m_IsActive = false;
    std::string m_PlayerName;
    std::string m_Chat;
};

struct NetClientData
{
    ENetAddress m_Address = { 0 };
    ENetHost* m_Client = { 0 };
    ENetPeer* m_Server = nullptr;
    int m_LocalPlayerId = -1;
    std::string m_LocalPlayerName;
    NetPlayer m_Players[NetConsts::s_MaxPlayers];
};

static NetClientData* s_NetClientData;

static void SendUpdatePlayerName()
{
    printf("\n\tENET_EVENT_SEND\n");
    printf("\t\tNetCmd::UpdatePlayerName\n");

    uint8_t data[64] = { 0 };
    NetCmdBuffer buffer(data, sizeof(data));
    buffer.WriteInt(static_cast<int>(NetCmd::UpdatePlayerName));
    buffer.WriteInt(s_NetClientData->m_LocalPlayerId);

    for (int i = 0; i <= s_NetClientData->m_LocalPlayerName.size(); ++i)
    {
        buffer.WriteChar(s_NetClientData->m_LocalPlayerName[i]);
    }

    ENetPacket* packet = enet_packet_create(buffer.GetData(), buffer.GetSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(s_NetClientData->m_Server, 0, packet);
}

static void SendChatToServer(std::string chat)
{
    printf("\n\tENET_EVENT_SEND\n");
    printf("\t\tNetCmd::ChatToServer\n");

    uint8_t data[512] = { 0 };
    NetCmdBuffer buffer(data, sizeof(data));
    buffer.WriteInt(static_cast<int>(NetCmd::ChatToServer));
    buffer.WriteInt(s_NetClientData->m_LocalPlayerId);

    for (int i = 0; i <= chat.size(); ++i)
    {
        buffer.WriteChar(chat[i]);
    }

    ENetPacket* packet = enet_packet_create(buffer.GetData(), buffer.GetSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(s_NetClientData->m_Server, 0, packet);
}

int NetClient::StartUpNetClient()
{
    if (enet_initialize() != 0) 
    {
        assert("An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    s_NetClientData = new NetClientData();

    s_NetClientData->m_Client = enet_host_create(NULL, 1, 2, 0, 0);

    if (s_NetClientData->m_Client == NULL) 
    {
        assert("An error occurred while trying to create an ENet client host.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void NetClient::ShutDownNetClient()
{
    if (s_NetClientData != nullptr)
    {
        if (s_NetClientData->m_Server != nullptr)
        {
            enet_peer_disconnect(s_NetClientData->m_Server, 0);
        }

        if (s_NetClientData->m_Client != nullptr)
        {
            enet_host_destroy(s_NetClientData->m_Client);
        }

        s_NetClientData->m_Server = nullptr;
        s_NetClientData->m_Client = nullptr;

        enet_deinitialize();

        delete s_NetClientData;
        s_NetClientData = nullptr;
    }
}

int NetClient::ConnectToServer(const char *ipAdress) //"127.0.0.1"
{
    if (s_NetClientData == nullptr)
    {
        assert("NetClient uninitialised!");
    }

    enet_address_set_host(&s_NetClientData->m_Address, ipAdress);
    s_NetClientData->m_Address.port = NetConsts::s_Port;
    
    s_NetClientData->m_Server = enet_host_connect(s_NetClientData->m_Client, &s_NetClientData->m_Address, 2, 0);

    if (s_NetClientData->m_Server == NULL)
    {
        assert("No available server for initiating an ENet connection.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void NetClient::UpdateNetClient()
{   
    if (s_NetClientData == nullptr)
    {
        assert("NetClient uninitialised!");
    }

    ENetEvent event;

    if (enet_host_service(s_NetClientData->m_Client, &event, 0) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
        {         
            printf("\nENET_EVENT_TYPE_CONNECT\n");
            break;
        }

        case ENET_EVENT_TYPE_RECEIVE:
        {
            printf("\nENET_EVENT_TYPE_RECEIVE : CHANNEL %u\n", event.channelID);
            if (event.packet->dataLength > 0)
            {
                const int size = static_cast<int>(event.packet->dataLength);

                NetCmdBuffer buffer(event.packet->data, size);

                NetCmd cmd = static_cast<NetCmd>(buffer.ReadInt());

                if (s_NetClientData->m_LocalPlayerId == -1)
                {
                    if (cmd == NetCmd::AcceptPlayer)
                    {
                        printf("\tNetCmd::AcceptPlayer\n");

                        const int playerId = buffer.ReadInt();
                        printf("\tplayerId = %d\n", playerId);

                        s_NetClientData->m_LocalPlayerId = playerId;
                        s_NetClientData->m_Players[playerId].m_IsActive = true;

                        SendUpdatePlayerName();
                    }
                }
                else
                {
                    switch (cmd)
                    {
                    case NetCmd::AddPlayer:
                    {
                        printf("\tNetCmd::AddPlayer\n");

                        const int playerId = buffer.ReadInt();
                        printf("\tplayerId = %d\n", playerId);

                        s_NetClientData->m_Players[playerId].m_IsActive = true;

                        char playerName[64];
                        int playerNameOffset = 0;
                        while (buffer.GetIndex() < size)
                        {
                            playerName[playerNameOffset] = buffer.ReadChar();
                            playerNameOffset++;
                        }

                        s_NetClientData->m_Players[playerId].m_PlayerName = playerName;
                        break;
                    }

                    case NetCmd::RemovePlayer:
                    {
                        printf("\tNetCmd::RemovePlayer\n");

                        const int playerId = buffer.ReadInt();
                        printf("\tplayerId = %d\n", playerId);
                        
                        s_NetClientData->m_Players[playerId].m_IsActive = false;
                        break;
                    }

                    case NetCmd::ChatToClient:
                    {
                        printf("\tNetCmd::ChatToClient\n");

                        const int playerId = buffer.ReadInt();
                        printf("\tplayerId = %d\n", playerId);

                        char chat[512];
                        int chatOffset = 0;
                        while (buffer.GetIndex() < size)
                        {
                            chat[chatOffset] = buffer.ReadChar();
                            chatOffset++;
                        }
                        
                        s_NetClientData->m_Players[playerId].m_Chat = chat;
                        break;
                    }
                    }
                }
            }

            enet_packet_destroy(event.packet);
            break;
        }

        case ENET_EVENT_TYPE_DISCONNECT:
        case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
        {     
            s_NetClientData->m_Server = nullptr;
            s_NetClientData->m_LocalPlayerId = -1;       
            break;
        }

        case ENET_EVENT_TYPE_NONE:
            break;
        }
    }
}

bool NetClient::IsConnected()
{
    if (s_NetClientData != nullptr && 
        s_NetClientData->m_LocalPlayerId >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool NetClient::IsPlayerActive(int playerIdx)
{
    if (s_NetClientData != nullptr && s_NetClientData->m_Players[playerIdx].m_IsActive)
    {
        return true;
    }
    else
    {
        return false;
    }
}

const char* NetClient::GetPlayerName(int playerIdx)
{
    if (s_NetClientData == nullptr)
    {
        assert("NetClient uninitialised!");
        return nullptr;
    }

    if (!s_NetClientData->m_Players[playerIdx].m_IsActive)
    {
        assert("Player inactive!");
        return nullptr;
    }

    return s_NetClientData->m_Players[playerIdx].m_PlayerName.c_str();
}

const char* NetClient::GetPlayerChat(int playerIdx)
{
    if (s_NetClientData == nullptr)
    {
        assert("NetClient uninitialised!");
        return nullptr;
    }

    if (!s_NetClientData->m_Players[playerIdx].m_IsActive)
    {
        assert("Player inactive!");
        return nullptr;
    }

    return s_NetClientData->m_Players[playerIdx].m_Chat.c_str();
}

void NetClient::SetLocalPlayerName(const char* name)
{
    if (s_NetClientData == nullptr)
    {
        assert("NetClient uninitialised!");
    }

    s_NetClientData->m_LocalPlayerName = name;
}

void NetClient::SendChat(const char* chat)
{
    SendChatToServer(chat);
}
