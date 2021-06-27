#pragma once
#include <cstdint>
#include <cstring>


namespace interfaces
{
	namespace system
	{
		class ISerializable
		{
		  public:
			// get the size of the value serialized
			virtual std::size_t serializedSize() const = 0;

			// print the value in a buffer at the given index and add '\0' at the end
			bool printToBuffer(uint8_t *buffer, const uint32_t index)
			{
				for (auto i = 0U; i <= serializedSize(); i++)
					buffer[i + index] = getChar(i);
				return true;
			}

			// print the value at the end of a string (use the null terminator)
			bool printToBuffer(uint8_t *buffer)
			{
				return printToBuffer(buffer, strlen(reinterpret_cast<const char *>(buffer)));
			}
			// get the char of the serialized data for the given index
			virtual char getChar(uint32_t index) const = 0;

		  private:
		};
	} // namespace system
}
