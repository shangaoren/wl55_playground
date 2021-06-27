#pragma once
#include <cstdint>

namespace volund
{
	namespace utils
	{
		namespace strings
		{

			template <typename Type, typename... Args>
			static bool sscanfpp(const char *p_sentence, const char *p_format, Type p_arg0, Args *... args);

			static uint16_t getMaxChar(const char *p_data, uint16_t &p_index)
			{
				uint16_t width = 0;
				while (p_data[p_index] >= '0' && p_data[p_index] <= '9')
				{
					width *= 10;
					width = p_data[p_index];
					p_index++;
				}
				return width;
			}

			// after identifying a % call this function to get
			static void sscanfpp(const char *p_sentence, uint16_t &p_dataIndex, uint16_t *p_data)
			{
				uint16_t data = 0;
				while (p_sentence[p_dataIndex] >= '0' && p_sentence[p_dataIndex] <= '9')
				{
					data *= 10;
					data += p_sentence[p_dataIndex] -'0';
					p_dataIndex++;
				}
				*p_data = data;
			}

			// after identifying a % call this function to get
			static void sscanfpp(const char *p_sentence, uint16_t &p_dataIndex, int16_t *p_data)
			{
				uint16_t data = 0;
				bool isNegative = false;
				if (p_sentence[p_dataIndex] == '-')
				{
					isNegative = true;
					p_dataIndex++;
				}
				while (p_sentence[p_dataIndex] >= '0' && p_sentence[p_dataIndex] <= '9')
				{
					data *= 10;
					data += p_sentence[p_dataIndex] - '0';
					p_dataIndex++;
				}
				if (isNegative)
					*p_data = -data;
				else
					*p_data = data;
			}

			// after identifying a % call this function to get
			static void sscanfpp(const char *p_sentence, uint16_t &p_dataIndex, uint8_t *p_data)
			{
				uint8_t data = 0;
				while (p_sentence[p_dataIndex] >= '0' && p_sentence[p_dataIndex] <= '9')
				{
					data *= 10;
					data += p_sentence[p_dataIndex] - '0';
					p_dataIndex++;
				}
				*p_data = data;
			}

			static bool sscanfpp(const char *p_sentence, const char *p_format)
			{
				uint16_t formatIndex = 0;
				uint16_t dataIndex = 0;
				while (p_sentence[dataIndex] != '\0' && p_format[formatIndex] != '\0')
				{
					//skip spaces
					while (p_sentence[dataIndex] == ' ')
					{
						dataIndex++;
					}

					while (p_format[formatIndex] == ' ')
					{
						formatIndex++;
					}
					if (p_format[formatIndex] != p_sentence[dataIndex])
						return false;
					formatIndex++;
					dataIndex++;
				}
				return true;
			}

			template <typename Type,typename ... Args>
			static bool sscanfpp(const char *p_sentence, const char *p_format,Type p_arg0, Args* ... args)
			{
				uint16_t formatIndex = 0;
				uint16_t dataIndex = 0;
				while (p_sentence[dataIndex] != '\0' && p_format[formatIndex] != '\0')
				{
					//skip spaces
					while (p_sentence[dataIndex] == ' ')
					{
						dataIndex++;
					}

					while (p_format[formatIndex] == ' ')
					{
						formatIndex++;
					}
					// if % read data 
					if (p_format[formatIndex] == '%')
					{
						formatIndex++;
						auto maxChar = getMaxChar(p_format, formatIndex);
						formatIndex++;
						sscanfpp(p_sentence, dataIndex, p_arg0);
						return sscanfpp(&p_sentence[dataIndex], &p_format[formatIndex], args...);
					}
					else // else compare Data
					{
						if (p_format[formatIndex] != p_sentence[dataIndex])
							return false;
						formatIndex++;
						dataIndex++;
					}
				}
				return true;
			}





		} // namespace strings
	} // namespace utils
} // namespace volund
