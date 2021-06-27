#pragma once
#include <cstdint>
//#include "yggdrasil/framework/DualLinkedList.hpp"
#include "volund/system/Fifo.hpp"

namespace utils
{
	template<std::size_t t_bufferDepth>
	struct UartBuffer : public volund::system::FifoNode<UartBuffer<t_bufferDepth>, UartBuffer<t_bufferDepth>>
	{
		constexpr UartBuffer() : buffer {}
		{
				
		}
		char buffer[t_bufferDepth];
		constexpr std::size_t size()
		{
			return t_bufferDepth;
		}
	}; // class UartBuffer
} // namespace utils
