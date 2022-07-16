#pragma once

#include <stdint.h>

enum class NetCmd
{
    AcceptPlayer,
    AddPlayer,
    RemovePlayer,
    UpdatePlayerName,
    ChatToServer,
    ChatToClient,
};

class NetCmdBuffer
{
public:
    NetCmdBuffer(uint8_t* data, int size);

    void WriteInt(uint32_t value);
    uint32_t ReadInt();

    void WriteChar(uint8_t value);
    uint8_t ReadChar();

    uint8_t* GetData() const { return m_Data; }
    int GetSize() const { return m_Size; }
    int GetIndex() const { return m_Index; }

private:
    uint8_t* m_Data;
    int m_Size;
    int m_Index;
};
