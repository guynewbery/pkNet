#pragma once

#include <string>

class ChatBuffer
{
public:    
    ChatBuffer(int maxSize);

    ChatBuffer() = delete;
    ChatBuffer(const ChatBuffer&) = delete;
    ChatBuffer& operator=(const ChatBuffer&) = delete;

    void UpdateChatBuffer();
    void ClearChatBuffer();
    const std::string& GetChatBuffer() const { return m_Buffer; }

private:
    int m_MaxSize;
    std::string m_Buffer;
};
