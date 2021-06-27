#pragma once
#include <cstdint>

namespace interfaces
{
	namespace drivers
	{
		class IIn
		{
		  public:
			virtual bool inputState() = 0;
		};
	} // namespace drivers
}
