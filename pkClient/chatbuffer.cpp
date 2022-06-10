#include "chatbuffer.h"

#include "raylib.h"

ChatBuffer::ChatBuffer(int maxSize) :
    m_MaxSize(maxSize)
{

}

void ChatBuffer::UpdateChatBuffer()
{
    int key = GetCharPressed();

    // Check if more characters have been pressed on the same frame
    while (key > 0)
    {
        if ((key >= 32) && (key <= 125) && (m_Buffer.size() < m_MaxSize))
        {
            m_Buffer.push_back(static_cast<char>(key));
        }

        key = GetCharPressed();  // Check next character in the queue
    }

    if (m_Buffer.size() > 0 && IsKeyPressed(KEY_BACKSPACE))
    {
        m_Buffer.pop_back();
    }
}

void ChatBuffer::ClearChatBuffer()
{
    m_Buffer.clear();
}
