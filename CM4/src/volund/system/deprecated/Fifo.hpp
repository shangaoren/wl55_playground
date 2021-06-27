#pragma once
#include <cstdint>
#include <array>

namespace deprecated
{
	template <typename FifoType, uint16_t Depth>
	class Fifo
	{
	  public:
		Fifo()
		{
		}

		//return a pointer to the next available storage , return nullptr if no storage left
		FifoType *getWriteStorage()
		{
			if (m_writeIndex + 1 == Depth && m_readIndex == 0) // reached the end of carrousel and no storage was freed
				return nullptr;
			if ((m_remain == false && (m_writeIndex + 1 >= m_readIndex)) | (m_remain == true && (m_writeIndex + 1 < m_readIndex)))
			{
				m_writeIndex++;
				if (m_writeIndex == Depth)
				{
					m_writeIndex = 0;
					m_remain = true;
				}
				return &m_fifo[m_writeIndex];
			}
			else
			{
				return nullptr;
			}
		}

		//return a pointer to the next storage to read, return nullptr if no storage to read
		FifoType *getReadStorage()
		{
			if (m_readIndex + 1 == Depth && m_writeIndex == 0) //reached the end of carrousel and write index still at 0
				return nullptr;
			if ((m_remain == false && m_readIndex + 1 < m_writeIndex) | (m_remain == true && (m_readIndex + 1 >= m_writeIndex)))
			{
				m_readIndex++;
				if (m_readIndex == Depth)
				{
					m_readIndex = 0;
					m_remain = false;
				}
				return &m_fifo[m_readIndex];
			}
			else
			{
				return nullptr;
			}
		}

		FifoType *getLastWritten()
		{
			if (m_writeIndex > 0 && m_remain == false)
			{
				m_readIndex = m_writeIndex;
				return &m_fifo[m_readIndex - 1];
			}
			else
			{
				if ((m_writeIndex == 0) && (m_remain == true))
				{
					m_readIndex = m_writeIndex;
					m_remain = false;
					return &m_fifo[Depth - 1];
				}
				else
					return nullptr;
			}
		}

	  private:
		std::array<FifoType, Depth> m_fifo;
		volatile uint16_t m_writeIndex = 0; //reflect the data to be written
		volatile uint16_t m_readIndex = 0;  //reflect the index of data to be read
		volatile bool m_remain = false;		// used to know if a turn was made by write index but not by read index
	};
} // namespace deprecated
