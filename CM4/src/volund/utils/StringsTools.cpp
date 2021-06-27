#include "StringsTools.hpp"

namespace volund
{
	bool StringsTools::compare(const char *a, const char *b)
	{
		if (strlen(a) != strlen(b))
			return false;
		for (auto i = 0U; i < strlen(a); i++)
		{
			if (a[i] != b[i])
				return false;
		}
		return true;
	}

	bool StringsTools::compare(const char *a, const char *b, uint16_t sizeOfB)
	{
		if (strlen(a) != sizeOfB)
			return false;
		for (auto i = 0U; i < strlen(a); i++)
		{
			if (a[i] != b[i])
				return false;
		}
		return true;
	}

	bool StringsTools::compare(const char *a, const uint16_t sizeA, const char *b, const uint16_t sizeB)
	{
		if (sizeA != sizeB)
			return false;
		for (auto i = 0U; i < sizeA; i++)
		{
			if (a[i] != b[i])
				return false;
		}
		return true;
	}

	void StringsTools::strCopy(const char *src, char *dest)
	{
		uint16_t index = 0;
		while (src[index] != '\0' && dest[index] != '\0')
		{
			dest[index] = src[index];
			index++;
		}
	}

	void StringsTools::strCopy(const char *src, char *dest, uint16_t destSize)
	{
		uint16_t index = 0;
		while (src[index] != '\0' && index < destSize)
		{
 			dest[index] = src[index];
			index++;
		}
	}

	/*Search if the reference substring exists in data, return the pointer to the start of reference in data if exists, nullptr otherwise*/
	char* StringsTools::contains(char *data, const char *reference)
	{
		uint16_t sizeOfReference = strlen(reference);
		uint16_t sizeOfData = strlen(data);
		uint16_t iA = 0;
		uint16_t iB = 0;
		char *start = nullptr;

		if (sizeOfReference > sizeOfData)
			return nullptr;

		// while data can contain reference

		while (!(((iA - iB) + sizeOfReference) > sizeOfData))
		{
			if (data[iA] == reference[iB])
			{
				if (iB == 0)
				{
					start = &data[iA];
				}
				iB++;
				if (iB == sizeOfReference)
					return start;
			}
			else
			{
				iB = 0;
				start = nullptr;
			}
			iA++;
		}
		return start;
	}

	const char* StringsTools::contains(const char *data, const char *reference)
	{
		uint16_t sizeOfReference = strlen(reference);
		uint16_t sizeOfData = strlen(data);
		uint16_t iA = 0;
		uint16_t iB = 0;
		const char *start = nullptr;

		if (sizeOfReference > sizeOfData)
			return nullptr;

		// while data can contain reference
		while (!((iA + sizeOfReference) > sizeOfData))
		{
			if (data[iA] == reference[iB])
			{
				if (iB == 0)
					start = data + iA;
				iB++;
				if (iB == sizeOfReference)
					return start;
			}
			else
			{
				iB = 0;
				start = nullptr;
			}
			iA++;
		}
		return start;
	}

	char StringsTools::digitToChar(const uint8_t digit)
	{
		if (digit > 9)
			return '?';
		else
			return '0' + digit;
	}

	// Convert a U8 Value into a string and write it into the given buffer
	// @return the number of written bytes, 0 otherwise
	uint16_t StringsTools::toString(uint8_t t_value, char *t_buffer, const uint16_t t_size, uint8_t t_nDigits)
	{
		const std::size_t resultMaxSize = 3; // max size = 255
		char result[resultMaxSize];
		uint16_t remain = t_value;
		uint16_t divider = 100;
		for (auto i = 0U; i < resultMaxSize; i++)
		{
			result[i] = remain / divider + '0';
			remain = remain % divider; // get the remain of the division
			divider /= 10;			   // prepare for next division
		}
		uint8_t i = 0;
		// skip zeros until end if t_nDigits = 0
		if (t_nDigits == 0)
		{
			while (result[i] == '0' && i != resultMaxSize - 1)
				i++;
		}
		else
			i = resultMaxSize - t_nDigits;

		if (t_size <= resultMaxSize - i) // need size of number+ null terminator
			return 0;				// can't write all
		// write result in buffer
		uint8_t bufferIndex = 0;
		while (i != resultMaxSize)
		{
			t_buffer[bufferIndex] = result[i];
			bufferIndex++;
			i++;
		}
		t_buffer[bufferIndex] = '\0';
		return bufferIndex;
	}

	// Convert a U16 Value into a string and write it into the given buffer
	// @return the number of written bytes, 0 otherwise
	uint16_t StringsTools::toString(uint16_t t_value, char *t_buffer, const uint16_t t_size, uint8_t t_nDigits)
	{
		const std::size_t stringMaxSize = 5; /*Maximum Size = 65535*/
		char result[stringMaxSize];
		uint16_t remain = t_value;
		uint16_t divider = 10000;
		for (auto i = 0U; i < stringMaxSize; i++)
		{
			result[i] = remain / divider + '0';
			remain = remain % divider; // get the remain of the division
			divider /= 10;			   // prepare for next division
		}
		uint8_t i = 0;
		// skip zeros until end if t_nDigits = 0
		if (t_nDigits == 0)
		{
			while (result[i] == '0' && i != stringMaxSize - 1)
				i++;
		}
		else
			i = stringMaxSize - t_nDigits;

		if (t_size <= stringMaxSize - i) // need size of number+ null terminator
			return 0;				// can't write all
		// write result in buffer
		uint8_t bufferIndex = 0;
		while (i != stringMaxSize)
		{
			t_buffer[bufferIndex] = result[i];
			bufferIndex++;
			i++;
		}
		t_buffer[bufferIndex] = '\0';
		return bufferIndex;
	}

	// Convert a U32 Value into a string and write it into the given buffer
	// @return the number of written bytes, 0 otherwise
	uint16_t StringsTools::toString(uint32_t p_value, char* p_buffer, const uint16_t p_size, uint8_t p_nDigits)
	{
		const std::size_t stringMaxSize = 10; /*Maximum Size = 4294967295*/
		char result[stringMaxSize];
		uint32_t remain = p_value;
		uint32_t divider = 1000000000;
		for (auto i = 0U; i < stringMaxSize; i++)
		{
			result[i] = remain / divider + '0';
			remain = remain % divider;  // get the remain of the division
			divider /= 10; 			   // prepare for next division
		}
		uint8_t i = 0;
		// skip zeros until end if t_nDigits = 0
		if(p_nDigits == 0)
		{
			while (result[i] == '0' && i != stringMaxSize - 1)
				i++;
		}
		else
			i = stringMaxSize - p_nDigits;

		if (p_size <= stringMaxSize - i) // need size of number+ null terminator
			return 0; 				// can't write all
		// write result in buffer
		uint8_t bufferIndex = 0;
		while (i != stringMaxSize)
		{
			p_buffer[bufferIndex] = result[i];
			bufferIndex++;
			i++;
		}
		p_buffer[bufferIndex] = '\0';
		return bufferIndex;
	}

	uint8_t StringsTools::toU8(const char *p_data, std::size_t p_size)
	{
		if (p_size > 3)
			return 0;
		uint16_t result = 0;
		for (auto i = 0U; i < p_size; i++)
		{
			result *= 10;
			result += p_data[i]-'0';
		}
		if (result > 255)
			return 255;
		else
			return result;
	}

	uint16_t StringsTools::toU16(const char *p_data, std::size_t p_size)
	{
		if (p_size > 5)
			return 0;
		uint32_t result = 0;
		for (auto i = 0; i < p_size; i++)
		{
			result *= 10;
			result += p_data[i]-'0';
		}
		if (result > 65535)
			return 65535;
		else
			return result;
	}

	int16_t StringsTools::toI16(const char *p_data, std::size_t p_size)
	{
		if (p_size > 6) // int16_t limit + sign
			p_size = 6;
		int16_t result = 0;
		int16_t index = 0;
		bool isNegative = false;
		if (p_data[index] == '-')
		{
			isNegative = true;
			index++;
		}
		if (p_data[index] == '+')
		{
			isNegative = false;
			index++;
		}
		while ((p_data[index] <= '9') && (p_data[index] >= '0') && (index < p_size))
		{
			result *= 10;
			result += (p_data[index] - '0');
			index++;
		}
		return isNegative == true ? -result : result;
	}

	uint32_t StringsTools::toU32(const char *p_data, std::size_t p_size)
	{
		if (p_size > 10)
			return 0;
		uint32_t result = 0;
		for (auto i = 0; i < p_size; i++)
		{
			if ((i == 9) && (result >= 429496729))
			{
				if ((result > 429496729) || ((p_data[i]-'0') > 5))
					return 4294967295;
			}
			result *= 10;
			result += p_data[i]-'0';
		}
		return result;
	}

	std::size_t StringsTools::skipZeros(const char *p_data, std::size_t p_size)
	{
		std::size_t index = 0;
		while (p_data[index] == '0' && index < p_size)
		{
			index++;
		}
		return index;
	}
} // namespace volund