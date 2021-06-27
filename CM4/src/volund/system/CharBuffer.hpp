#pragma once
#include <cstdint>
#include <cstring>


template<uint32_t BufferSize>
class CharBuffer
{
  public:
	/*get the buffer*/
	char *ref()
	{
		return m_buffer;
	}

	/*Get the number of char in the buffer*/
	constexpr uint32_t size()
	{
		return BufferSize; // send buffer index as it is the next empty byte
	}
	
	uint32_t usedSize()
	{
		return m_bufferIndex; // send buffer index as it is the next empty byte
	}

	// print a null terminated string
	bool print(const char *buffer)
	{
		uint32_t index = 0;
		while (m_bufferIndex < BufferSize && index < strlen(buffer))
		{
			m_buffer[m_bufferIndex] = buffer[index];
			m_bufferIndex++;
			index++;
		}
		if (index != strlen(buffer))
			return false;
		else
			return true;
	}

	/*Print the given buffer in the char buffer, if too long sentence will be truncated and function return false*/
	bool print(const char* buffer,const uint32_t size)
	{
		uint32_t index = 0;
		if (m_bufferIndex + size <= BufferSize)
		{
			memcpy((m_buffer + m_bufferIndex), buffer, size);
			m_bufferIndex += size;
			return true;
		}
		else
			return true;
	}

	/*put a char in the buffer, if buffer already full, return false*/
	bool print(const char c)
	{
		if (m_bufferIndex < BufferSize)
		{
			m_buffer[m_bufferIndex] = c;
			m_bufferIndex++;
			return true;
		}
		return false;
	}

	// print a number in the buffer
	bool printNumber(const uint16_t number)
	{
		char result[5]; // maximum 65535
		uint16_t remain = number;
		uint16_t divider = 10000;
		for (auto i = 0U; i < 5; i++)
		{
			result[i] = remain / divider + '0';
			remain = remain%divider; // get the remain of the division
			divider /= 10; // prepare for next division
		}
		uint8_t i = 0;
		// skip zeros until end
		while (result[i] == '0' && i != 4)
		{
			i++;
		}
		// write result in buffer
		while (i != 5)
		{
			if (BufferSize - m_bufferIndex < 5 - i)
				return false; // can't write all
			m_buffer[m_bufferIndex] = result[i];
			m_bufferIndex++;
			i++;
		}
		return true;
	}

	/*clear the buffer*/
	void clear()
	{
		m_bufferIndex = 0;
	}

  private:
	char m_buffer[BufferSize];
	uint32_t m_bufferIndex;
};
