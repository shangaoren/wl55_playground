#pragma once

#include <cstdint>
#include <cstring>
#include <array>

template<typename BufferType, std::size_t BufferSize>
	class SecureBuffer
	{
	public:
		SecureBuffer()
		{
		}
		
		void flush()
		{
			m_index = 0;
		}
		
		bool add(const BufferType* t_buffer, std::size_t t_size)
		{
			if (t_size + m_index <= BufferSize)
			{
				memcpy(reinterpret_cast<void*>(&m_data[m_index]), reinterpret_cast<const void*>(t_buffer), t_size);
				m_index += t_size;
				return true;
			}
			return false;
		}
		
		bool add(const BufferType t_data)
		{
			if (1 + m_index <= BufferSize)
			{
				m_data[m_index] = t_data;
				m_index++;
				return true;
			}
			return false;
		}
		
		bool isEmpty() const
		{
			return m_index == 0 ? true : false;
		}
		
		constexpr std::size_t size() const
		{
			return BufferSize;
		}
		
		std::size_t used() const
		{
			return m_index;
		}
		
		const BufferType* get() const
		{
			return m_data.data();
		}
		
		
	private:
		std::array<BufferType, BufferSize> m_data;
		std::size_t m_index = 0;
	};
