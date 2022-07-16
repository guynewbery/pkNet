#include "pknet.h"

#include <assert.h>

namespace pkNet
{
	NetCmdBuffer::NetCmdBuffer(uint8_t* data, int size) :
		m_Data(data),
		m_Size(size),
		m_Index(0)
	{
	}

	void NetCmdBuffer::WriteInt(uint32_t value)
	{
		assert( m_Index + 4 <= m_Size );

		*((uint32_t*)(m_Data + m_Index)) = value; 

		m_Index += 4;
	}

	uint32_t NetCmdBuffer::ReadInt()
	{
		assert( m_Index + 4 <= m_Size );
		uint32_t value;

		value = *((uint32_t*)(m_Data + m_Index));

		m_Index += 4;
		return value;
	}

	void NetCmdBuffer::WriteChar(uint8_t value)
	{
		assert( m_Index + 1 <= m_Size );

		*((uint8_t*)(m_Data + m_Index)) = value; 

		m_Index += 1;
	}

	uint8_t NetCmdBuffer::ReadChar()
	{
		assert( m_Index + 1 <= m_Size );
		uint8_t value;

		value = *((uint8_t*)(m_Data + m_Index));

		m_Index += 1;
		return value;
	}
}
