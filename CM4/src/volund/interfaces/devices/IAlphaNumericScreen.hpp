#pragma once
#include <cstdint>

namespace interfaces
{
	namespace devices
	{
		class IAlphaNumericScreen
		{
		  public:
			virtual void init() = 0;
			virtual bool write(const char *text, uint16_t size, uint16_t column = 0, uint16_t row = 0) =0;
			virtual bool write(const char text, uint16_t column = 0, uint16_t row = 0) = 0;
			virtual bool putChar(const char text) = 0;
			virtual bool putString(const char *string, uint16_t size) = 0;
		};
	} // namespace devices
}
