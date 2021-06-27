#include <cstdint>

namespace utils
{
	class Converters
	{
	public:
		constexpr static uint16_t bufferToU16(const uint8_t* t_buffer, const bool t_isLittleEndian = true)
		{
			if (t_isLittleEndian)
				return static_cast<uint16_t>(((t_buffer[0] << 8) & 0xFF00) | t_buffer[1]);
			else
				return static_cast<uint16_t>(((t_buffer[1] << 8) & 0xFF00) | t_buffer[0]);
		}
		
		constexpr static uint32_t bufferToU24(const uint8_t* t_buffer, const bool t_isLittleEndian = true)
		{
			if (t_isLittleEndian)
				return static_cast<uint32_t>(((t_buffer[0] << 16) & 0xFF0000) | ((t_buffer[1] << 8) & 0x00FF00) | ((t_buffer[2] << 0) & 0x0000FF));
			else
				return static_cast<uint32_t>(((t_buffer[2] << 16) & 0xFF0000) | ((t_buffer[1] << 8) & 0x00FF00) | ((t_buffer[0] << 0) & 0x0000FF));
		}
		
		constexpr static uint32_t bufferToU32(const uint8_t* t_buffer, const bool t_isLittleEndian = true)
		{
			if (t_isLittleEndian)
				return static_cast<uint32_t>(((t_buffer[0] << 24) & 0xFF000000) | ((t_buffer[1] << 16) & 0x00FF0000) | ((t_buffer[2] << 8) & 0x0000FF00) | ((t_buffer[3] << 0) & 0x000000FF));
			else
				return static_cast<uint32_t>(((t_buffer[3] << 24) & 0xFF000000) | ((t_buffer[2] << 16) & 0x00FF0000) | ((t_buffer[1] << 8) & 0x0000FF00) | ((t_buffer[0] << 0) & 0x000000FF));
		}
		
		constexpr static int16_t bufferToI16(const uint8_t* t_buffer, const bool t_isLittleEndian = true)
		{
			if (t_isLittleEndian)
				return static_cast<int16_t>(((t_buffer[0] << 8) & 0xFF00) | t_buffer[1]);
			else
				return static_cast<int16_t>(((t_buffer[1] << 8) & 0xFF00) | t_buffer[0]);
		}
		
		constexpr static int32_t bufferToI24(const uint8_t* t_buffer, const bool t_isLittleEndian = true)
		{
			if (t_isLittleEndian)
				return static_cast<int32_t>(((t_buffer[0] << 24) & 0xFF0000) | ((t_buffer[1] << 16) & 0x00FF00) | ((t_buffer[2] << 8) & 0x0000FF)) >> 8;
			else
				return static_cast<int32_t>(((t_buffer[2] << 24) & 0xFF0000) | ((t_buffer[1] << 16) & 0x00FF00) | ((t_buffer[0] << 8) & 0x0000FF)) >> 8;
		}
		
		constexpr static int32_t bufferToI32(const uint8_t* t_buffer, const bool t_isLittleEndian = true)
		{
			if (t_isLittleEndian)
				return static_cast<int32_t>(((t_buffer[0] << 24) & 0xFF000000) | ((t_buffer[1] << 16) & 0x00FF0000) | ((t_buffer[2] << 8) & 0x0000FF00) | ((t_buffer[3] << 0) & 0x000000FF));
			else
				return static_cast<int32_t>(((t_buffer[3] << 24) & 0xFF000000) | ((t_buffer[2] << 16) & 0x00FF0000) | ((t_buffer[1] << 8) & 0x0000FF00) | ((t_buffer[0] << 0) & 0x000000FF));
		}

		static float bufferToFloat(const uint8_t *t_buffer, const bool t_isLittleEndian = true)
		{
			float result = 0;
			uint8_t *floatBuffer = reinterpret_cast<uint8_t *>(&result);
			if (t_isLittleEndian)
			{
				floatBuffer[0] = t_buffer[3];
				floatBuffer[1] = t_buffer[2];
				floatBuffer[2] = t_buffer[1];
				floatBuffer[3] = t_buffer[0];
			}
			else
			{
				floatBuffer[3] = t_buffer[3];
				floatBuffer[2] = t_buffer[2];
				floatBuffer[1] = t_buffer[1];
				floatBuffer[0] = t_buffer[0];
			}
			return result;
		}

		constexpr static void toBuffer(uint8_t * t_buffer,const uint16_t t_data, const bool t_isLittleEndian = true)
		{
			if (t_isLittleEndian)
			{
				t_buffer[0] = ((t_data >> 8) & 0xFF);
				t_buffer[1] = ((t_data)&0xFF);
			}
			else
			{
				t_buffer[1] = ((t_data >> 8) & 0xFF);
				t_buffer[0] = ((t_data)&0xFF);
			}
		}

		constexpr static void toBuffer(uint8_t *t_buffer, const int16_t t_data, const bool t_isLittleEndian = true)
		{
			if (t_isLittleEndian)
			{
				t_buffer[0] = ((t_data >> 8) & 0xFF);
				t_buffer[1] = ((t_data)&0xFF);
			}
			else
			{
				t_buffer[1] = ((t_data >> 8) & 0xFF);
				t_buffer[0] = ((t_data)&0xFF);
			}
		}

		constexpr static void toBuffer(uint8_t *t_buffer, const uint32_t t_data, const bool t_isLittleEndian = true)
		{
			if (t_isLittleEndian)
			{
				t_buffer[0] = ((t_data >> 24) & 0xFF);
				t_buffer[1] = ((t_data >> 16) & 0xFF);
				t_buffer[2] = ((t_data >> 8) & 0xFF);
				t_buffer[3] = ((t_data)&0xFF);
			}
			else
			{
				t_buffer[3] = ((t_data >> 24) & 0xFF);
				t_buffer[2] = ((t_data >> 16) & 0xFF);
				t_buffer[1] = ((t_data >> 8) & 0xFF);
				t_buffer[0] = ((t_data)&0xFF);
			}
		}

		constexpr static void toBuffer(uint8_t *t_buffer, const int32_t t_data, const bool t_isLittleEndian = true)
		{
			if (t_isLittleEndian)
			{
				t_buffer[0] = ((t_data >> 24) & 0xFF);
				t_buffer[1] = ((t_data >> 16) & 0xFF);
				t_buffer[2] = ((t_data >> 8) & 0xFF);
				t_buffer[3] = ((t_data)&0xFF);
			}
			else
			{
				t_buffer[3] = ((t_data >> 24) & 0xFF);
				t_buffer[2] = ((t_data >> 16) & 0xFF);
				t_buffer[1] = ((t_data >> 8) & 0xFF);
				t_buffer[0] = ((t_data)&0xFF);
			}
		}

		static void toBuffer(uint8_t *t_buffer, float t_data, const bool t_isLittleEndian = true)
		{
			uint8_t *data = reinterpret_cast<uint8_t *>(&t_data);
			if (t_isLittleEndian)
			{
				t_buffer[0] = data[3];
				t_buffer[1] = data[2];
				t_buffer[2] = data[1];
				t_buffer[3] = data[0];
			}
			else
			{
				t_buffer[3] = data[3];
				t_buffer[2] = data[2];
				t_buffer[1] = data[1];
				t_buffer[0] = data[0];
			}
		}
	}; // class Converters
	} // namespace utils