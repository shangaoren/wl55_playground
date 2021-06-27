#pragma once
#include <cstdint>

namespace interfaces
{
	namespace system
	{
		class IAllocator
		{
		public:
			virtual void* allocate(std::size_t size) = 0;
			virtual bool deAllocate(void* ptr) = 0;
		};
	}// end namespace system
}	// end namespace interfaces
