#pragma once

#include <cstdint>
#include <cstring>

namespace volund
{
	class StringsTools
	{
	public:
		/*Compare two strings null terminated strings, return true if equals false otherwise*/
	  static bool compare(const char *a, const char *b);

	  static bool compare(const char *a, const char *b, uint16_t sizeOfB);
	  static bool compare(const char *a, const uint16_t sizeA, const char *b, const uint16_t sizeB);

	  static void strCopy(const char *src, char *dest);

	  static void strCopy(const char *src, char *dest, uint16_t destSize);

	  /*Search if the reference substring exists in data, return the pointer to the start of reference in data if exists, nullptr otherwise*/
	  static char *contains(char *data, const char *reference);

	  /*Search if the reference substring exists in data, return the const pointer to the start of reference in data if exists, nullptr otherwise*/
	  static const char *contains(const char *data, const char *reference);

	  static char digitToChar(const uint8_t digit);

	  /*convert a uint8_t to a null terminated string in the given buffer*/
	  // return 0 if buffer is too small, written bytes otherwise (null terminator excluded)
	  // @param t_nDis is used to force the number of digits used, 0 to disable
	  static uint16_t toString(uint8_t t_value, char *t_buffer, const uint16_t t_size, uint8_t t_nDigits = 0);

	  // convert a uint16_t to a null terminated string in the given buffer
	  // return 0 if buffer is too small, written bytes otherwise (null terminator excluded)
	  // @param t_forceSize is used to force the number of digits used, 0 to disable
	  static uint16_t toString(uint16_t t_value, char *t_buffer, const uint16_t t_size, uint8_t t_nDigits = 0);

	  // convert a uint32_t to a null terminated string in the given buffer
	  // return 0 if buffer is too small, written bytes otherwise (null terminator excluded)
	  // @param t_forceSize is used to force the number of digits used, 0 to disable
	  static uint16_t toString(uint32_t p_value, char* p_buffer, const uint16_t p_size, uint8_t p_nDigits = 0);

	  //Convert a string into a uint8
	  static uint8_t toU8(const char *p_data, std::size_t p_size);

	  // convert a string into a uint16
	  static uint16_t toU16(const char *p_data, std::size_t p_size);

	  // Convert a string into a int16_t
	  static int16_t toI16(const char *p_data, std::size_t p_size);

	  //Convert a string into a uint32
	  static uint32_t toU32(const char *p_data, std::size_t p_size);

	  // return the number of zeros at the start of the string
	  static std::size_t skipZeros(const char *p_data, std::size_t p_size);

	}; // class stringTools
} // namespace volund