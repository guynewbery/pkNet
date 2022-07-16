#define _WINSOCK_DEPRECATED_NO_WARNINGS

#if (_WIN32_WINNT < 0x0601)
	#undef _WIN32_WINNT
    #define _WIN32_WINNT 0x0601
#endif

#define ENET_IMPLEMENTATION
#include "enet.h"

#include <stdio.h>
#include <string>
#include "netcmd.h"
#include "netconsts.h"

struct NetPlayer
{
    ENetPeer* m_Peer = nullptr;
    bool m_IsActive = false;
    std::string m_PlayerName;
    std::string m_Chat;
};

static NetPlayer s_Players[NetConsts::s_MaxPlayers];

static int GetFirstInactivePlayerId()
{
    for (int i = 0; i < NetConsts::s_MaxPlayers; ++i)
    {
        if (!s_Players[i].m_IsActive)
        {
            return i;
        }
    }
    return -1;
}

static int GetPlayerId(ENetPeer* peer)
{
    for (int i = 0; i < NetConsts::s_MaxPlayers; ++i)
    {
        if (s_Players[i].m_IsActive && s_Players[i].m_Peer == peer)
        {
            return i;
        }
    }
    return -1;
}

static void SentAcceptPlayer(ENetPeer* peer, int acceptedPlayerId)
{ 
    printf("\n\tENET_EVENT_SEND\n");
    printf("\t\tNetCmd::AcceptPlayer\n");

    uint8_t data[8] = { 0 };
    NetCmdBuffer buffer(data, sizeof(data));
    buffer.WriteInt(static_cast<int>(NetCmd::AcceptPlayer));
    buffer.WriteInt(acceptedPlayerId);

    ENetPacket* packet = enet_packet_create(buffer.GetData(), buffer.GetSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

static void SentAddPlayer(ENetPeer* peer, int addedPlayerId)
{ 
    printf("\n\tENET_EVENT_SEND\n");
    printf("\t\tNetCmd::AddPlayer\n");

    uint8_t data[64] = { 0 };
    NetCmdBuffer buffer(data, sizeof(data));
    buffer.WriteInt(static_cast<int>(NetCmd::AddPlayer));
    buffer.WriteInt(addedPlayerId);

    for (int i = 0; i <= s_Players[addedPlayerId].m_PlayerName.size(); ++i)
    {
        buffer.WriteChar(s_Players[addedPlayerId].m_PlayerName[i]);
    }

    ENetPacket* packet = enet_packet_create(buffer.GetData(), buffer.GetSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

static void SentRemovePlayer(ENetPeer* peer, int removedPlayerId)
{ 
    printf("\n\tENET_EVENT_SEND\n");
    printf("\t\tNetCmd::RemovePlayer\n");

    uint8_t data[8] = { 0 };
    NetCmdBuffer buffer(data, sizeof(data));
    buffer.WriteInt(static_cast<int>(NetCmd::RemovePlayer));
    buffer.WriteInt(removedPlayerId);

    ENetPacket* packet = enet_packet_create(buffer.GetData(), buffer.GetSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

static void SentChatToClient(ENetPeer* peer, int chattingPlayerId)
{ 
    printf("\n\tENET_EVENT_SEND\n");
    printf("\t\tNetCmd::ChatToClient\n");

    uint8_t data[512] = { 0 };
    NetCmdBuffer buffer(data, sizeof(data));
    buffer.WriteInt(static_cast<int>(NetCmd::ChatToClient));
    buffer.WriteInt(chattingPlayerId);

    for (int i = 0; i <= s_Players[chattingPlayerId].m_Chat.size(); ++i)
    {
        buffer.WriteChar(s_Players[chattingPlayerId].m_Chat[i]);
    }

    ENetPacket* packet = enet_packet_create(buffer.GetData(), buffer.GetSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

int main(void)
{    
    if (enet_initialize () != 0) 
    {
        printf("An error occurred while initializing ENet.\n");
        return 1;
    }

    ENetAddress address = { 0 };
    address.host = ENET_HOST_ANY;
    address.port = NetConsts::s_Port;

    ENetHost* server = enet_host_create(&address, NetConsts::s_MaxPlayers, 2, 0, 0);

    if (server == NULL) 
    {
        printf("An error occurred while trying to create an ENet server host.\n");
        return 1;
    }
    
    printf("ENet server started.\n");

    ENetEvent event;

    while (true)
    {    
        if (enet_host_service(server, &event, 1000) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
            {
                printf("\nENET_EVENT_TYPE_CONNECT\n");

                const int playerId = GetFirstInactivePlayerId();

                if (playerId != -1)
                {                
                    s_Players[playerId].m_Peer = event.peer;
                    s_Players[playerId].m_IsActive = true;

                    SentAcceptPlayer(event.peer, playerId);

                    for (int i = 0; i < NetConsts::s_MaxPlayers; ++i)
                    {
                        // Inform the newly accepted player of clients that are already on the server
                        if (i != playerId && s_Players[i].m_Peer)
                        {
                            SentAddPlayer(event.peer, i);
                        }
                    }
                }
                else
                {
                    enet_peer_disconnect(event.peer, 0);
                }

                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                printf("\nENET_EVENT_TYPE_RECEIVE : CHANNEL %u\n", event.channelID);

                if (event.packet->dataLength > 0)
                {
                    const int size = static_cast<int>(event.packet->dataLength);

                    NetCmdBuffer buffer(event.packet->data, size);

                    const NetCmd cmd = static_cast<NetCmd>(buffer.ReadInt());

                    switch (cmd)
                    {
                    case NetCmd::UpdatePlayerName:
                    {
                        printf("\tNetCmd::UpdatePlayerName\n");

                        const int playerId = buffer.ReadInt();
                        printf("\tplayerId = %d\n", playerId);

                        char playerName[64];
                        int playerNameOffset = 0;
                        while (buffer.GetIndex() < size)
                        {
                            playerName[playerNameOffset] = buffer.ReadChar();
                            playerNameOffset++;
                        }

                        s_Players[playerId].m_PlayerName = playerName;
                        printf("\tplayerName = %s\n", playerName);

                        for (int i = 0; i < NetConsts::s_MaxPlayers; ++i)
                        {
                            // Now that we have a name set for this player, let everyone know.
                            if (s_Players[i].m_Peer)
                            {
                                SentAddPlayer(s_Players[i].m_Peer, playerId);
                            }
                        }

                        break;
                    }

                    case NetCmd::ChatToServer:
                    {
                        printf("\tNetCmd::ChatToServer\n");

                        const int playerId = buffer.ReadInt();
                        printf("\tplayerId = %d\n", playerId);

                        char chat[256];
                        int chatOffset = 0;
                        while (buffer.GetIndex() < size)
                        {
                            chat[chatOffset] = buffer.ReadChar();
                            chatOffset++;
                        }

                        s_Players[playerId].m_Chat = chat;
                        printf("\tchat = %s\n", chat);

                        for (int i = 0; i < NetConsts::s_MaxPlayers; ++i)
                        {
                            if (s_Players[i].m_Peer)
                            {
                                SentChatToClient(s_Players[i].m_Peer, playerId);
                            }
                        }

                        break;
                    }
                    }
                }
                
                enet_packet_destroy(event.packet);

                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
            {            
                printf("\nENET_EVENT_TYPE_DISCONNECT\n");

                const int playerId = GetPlayerId(event.peer);
                printf("\tplayerId = %d\n", playerId);

                if (playerId != -1)
                {
                    s_Players[playerId].m_Peer = nullptr;
                    s_Players[playerId].m_IsActive = false;
                }

                for (int i = 0; i < NetConsts::s_MaxPlayers; ++i)
                {
                    // Remove this player from all clients that are still active on the sever

                    if (i != playerId && s_Players[i].m_Peer)
                    {
                        SentRemovePlayer(s_Players[i].m_Peer, playerId);
                    }
                }

                break;
            }

            case ENET_EVENT_TYPE_NONE:
                break;
            }
        }
    }

    enet_host_destroy(server);
    enet_deinitialize();

    return 0;
}
