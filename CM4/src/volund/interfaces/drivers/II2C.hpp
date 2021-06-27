#pragma once

#include <cstdint>
#include <functional>

namespace interfaces
{
	namespace drivers
	{
		class II2c
		{

		  public:
			II2c() : m_callback(nullptr)
			{
			}

			enum class TransferEndCode : uint32_t
			{
				noErrors = 0,
				nackReceived = 1,
			};

			enum class I2cState : uint32_t
			{
				idle,
				setupWrite16,
				setupWrite8,
				write,
				stopSent,

				setupRead8,
				setupRead16,
				switch2Read,
				read,
			};

			virtual bool write(const uint8_t *buffer, const uint16_t size, const uint8_t slave) = 0;
			virtual bool write(const uint8_t *buffer, const uint16_t size, const uint8_t slave, const uint8_t reg) = 0;
			virtual bool write(const uint8_t *buffer, const uint16_t size, const uint8_t slave, const uint16_t reg) = 0;
			virtual bool read(uint8_t *buffer, const uint16_t size, const uint8_t slave, const uint8_t reg) = 0;
			virtual bool read(uint8_t *buffer, const uint16_t size, const uint8_t slave, const uint16_t reg) = 0;
			virtual void reset() = 0;

			void onTransferEnd(std::function<void(TransferEndCode)> callback)
			{
				m_callback = callback;
			}

		  protected:
			void transferEnd(TransferEndCode code)
			{
				if (m_callback != nullptr)
					(m_callback)(code);
			}

		  private:
			std::function<void(TransferEndCode)> m_callback;
		};
	} // namespace drivers
} // namespace interfaces
