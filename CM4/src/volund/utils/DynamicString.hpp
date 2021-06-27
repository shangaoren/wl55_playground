#pragma once
#include "volund/utils/StringsTools.hpp"
#include <cstdint>
#include <cstring>

namespace volund
{
	template <std::size_t Capacity>
	class EmbeddedString
	{
	  public:
		constexpr EmbeddedString()
		{
			m_index = 0;
			m_data[m_index] = '\0';
		}

		constexpr EmbeddedString(const char *t_data, const uint16_t t_length)
		{
			std::size_t copySize = t_length;
			if (copySize > Capacity)
				copySize = Capacity;
			for (auto i = 0U; i < copySize; i++)
				m_data[i] = t_data[i];
			m_index = copySize;
			m_data[m_index] = '\0'; // null terminator
		}

		// add a null terminated string
		constexpr EmbeddedString &operator<<(const char *t_data)
		{
			uint16_t stringSize = strlen(t_data);
			if ((Capacity - size()) >= strlen(t_data))
			{
				memcpy(m_data + m_index, t_data, strlen(t_data));
				m_index += stringSize;
				m_data[m_index] = '\0';
			}
			return *this;
		}

		EmbeddedString &operator<<(const uint32_t p_data)
		{
			auto written = StringsTools::toString(p_data, m_data + m_index, Capacity - size());
			m_index += written;
			auto length = strlen(m_data);
			if ((m_index > Capacity) | (m_index != length))
				asm volatile("bkpt 0");
			return *this;
		}

		//get the number of char storable in string
		constexpr std::size_t capacity() const
		{
			return Capacity;
		}

		//get the current size of string
		constexpr std::size_t size() const
		{
			return m_index;
		}

		operator const char *() const
		{
			return m_data;
		}

		constexpr EmbeddedString &operator<<(char *t_string)
		{
			append(t_string);
			return *this;
		}

		constexpr bool append(const char *t_string)
		{
			auto length = strlen(t_string);
			//check if capacity of string can handle string and null terminator
			if (length + size() <= Capacity)
			{
				for (auto i = 0U; i < length; i++)
				{
					m_data[m_index + i] = t_string[i];
				}
				m_index += length;
				m_data[m_index] = '\0';
				return true;
			}
			return false;
		}

		constexpr bool append(const char *t_string, std::size_t t_size)
		{
			//check if capacity of string can handle string and null terminator
			if (t_size + size() <= Capacity)
			{
				for (auto i = 0U; i < t_size; i++)
				{
					m_data[m_index + i] = t_string[i];
				}
				m_index += t_size;
				m_data[m_index] = '\0';
				return true;
			}
			return false;
		}

		// Fill the string with a given char
		// don't care about index
		constexpr bool fill(const char p_value)
		{
			for (auto i = 0; i < Capacity; i++)
			{
				m_data[i] = p_value;
			}
			m_index = Capacity;
			m_data[m_index] = '\0';
			return true;
		}

		// fill the rest of the string with a given char
		constexpr bool completeWith(const char p_value)
		{
			for (auto i = m_index; i < Capacity; i++)
			{
				m_data[i] = p_value;
			}
			m_index = Capacity;
			m_data[m_index] = '\0';
			return true;
		}

		constexpr void flush()
		{
			m_data[0] = '\0';
			m_index = 0;
		}

		constexpr char *getRawPointer()
		{
			return m_data;
		}

		constexpr uint16_t &getRawIndex()
		{
			return m_index;
		}

		void forceIndex(std::size_t p_index)
		{
			if (p_index <= Capacity)
			{
				m_index = p_index;
				m_data[m_index] = '\0';
			}
		}

		template <std::size_t LeftSize>
		EmbeddedString &operator=(const EmbeddedString<LeftSize> p_left)
		{
			if (p_left.size() <= this->capacity())
			{
				flush();
				append(p_left, p_left.size());
			}
			return *this;
		}

		EmbeddedString &operator=(const char *p_left)
		{
			if (strlen(p_left) <= this->capacity())
			{
				flush();
				append(p_left, strlen(p_left));
			}
			return *this;
		}

		template <std::size_t OtherSize>
		bool operator==(EmbeddedString<OtherSize> p_other) const
		{
			if (m_index != p_other.size())
				return false; // size is not the same
			const char *other = p_other;
			for (auto i = 0U; i < m_index; i++)
			{
				if (m_data[i] != other[i])
					return false;
			}
			return true;
		}

		bool operator==(const char *p_other) const
		{
			if (size() != strlen(p_other))
				return false; // size is not the same
			for (auto i = 0U; i < m_index; i++)
			{
				if (m_data[i] != p_other[i])
					return false;
			}
			return true;
		}

		template <std::size_t OtherSize>
		bool operator!=(EmbeddedString<OtherSize> p_other) const
		{
			return !(*this == p_other);
		}

		bool operator!=(const char *p_other) const
		{
			return !(*this == p_other);
		}

	  private:
		char m_data[Capacity + 1]; // +1 to get null terminator
		uint16_t m_index;
	};
}