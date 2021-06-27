#pragma once
#include <cstdint>
namespace volund
{
	namespace utils
	{
		namespace string
		{
			
			enum class DataType
			{
				type_char,
				type_short,
				type_long,
				type_long_long,
				type_long_double,
				none,
			};
			
			/* retrieve length if exists, update index*/
			DataType getLength(const char* p_data, uint16_t& p_index)
			{
				
				switch (p_data[p_index])
				{
				case 'h':
					if (p_data[p_index + 1] == 'h')
					{
						p_index += 2;
						return volund::utils::string::DataType::type_char;
					}
					else
					{
						p_index++;
						return volund::utils::string::DataType::type_short;
					}
					break;
				case 'l':
					if (p_data[p_index + 1] == 'l')
					{
						p_index += 2;
						return volund::utils::string::DataType::type_long_long;
					}
					else
					{
						p_index++;
						return volund::utils::string::DataType::type_long;
					}
					break;
				case 'j':
					p_index++;
					return volund::utils::string::DataType::none;
					break;
				case 'z':
					p_index++;
					return volund::utils::string::DataType::none;
					break;
				case 't':
					p_index++;
					return volund::utils::string::DataType::none;
					break;
				default :
					return volund::utils::string::DataType::none;
					break;
							
				} // switch
			}	//	function getLength
			
			
			// get max length 
			uint16_t getMaxChar(const char* p_data, uint16_t& p_index)
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
			
			template<class ...Args>
			bool sscanf(const char* p_sentence, const char* p_format, Args...args)
				{
					uint16_t formatIndex = 0, sentenceIndex = 0;
					uint16_t width = 0;
					uint16_t length;
					DataType type;
					while (p_format[formatIndex] != '\0')
					{
						if (p_format[formatIndex] == '%')
						{
							// compute max width if exists
							width = getMaxChar(p_format, formatIndex);
							// get data 
							type = getLength(p_sentence, formatIndex);
							// get format 
							switch (p_format[formatIndex])
							{
							case 'i':
								switch (type)
								{
								case volund::utils::string::DataType::type_char:
								default:
									break;
								}
								break;
							case 'd':
								break;
							case 'u':
								break;
							case 'o':
								break;
							case 'x':
								break;
							case 'f':
								break;
							case 'e':
								break;
							case 'g':
								break;
							case 'a':
								break;
							case 'c':
								break;
							case 's':
								break;
							case 'p':
								break;
							case 'n':
								break;
							case '%':
								break;
							}
						}
					}
					return true;
				}
			
			template<class ReturnType>
				ReturnType getData(const char* p_data)
				{
					uint16_t index = 0;
					ReturnType data = 0;
					bool isNegative = false;
					if (p_data[index] == '-')
						isNegative = true;
					while (p_data[index] >= '0' && p_data[index] <= '9')
					{
						data *= 10;
						data += p_data[index];
						index++;
					}
					return isNegative == true ? -data : data;
				}
			

		} // namespace string
	} // namespace utils
} // namespace volund
