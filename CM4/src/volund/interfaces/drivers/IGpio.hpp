#pragma once
#include "volund/interfaces/drivers/IIn.hpp"
#include "volund/interfaces/drivers/IOut.hpp"

namespace interfaces
{
	namespace drivers
	{
		class IGpio : public interfaces::drivers::IOut, public interfaces::drivers::IIn
		{
		  public:
			enum class GpioMode
			{
				input, 
				output,
			};

			virtual void setup(GpioMode mode) = 0;
			virtual void makeOutput(PullUpPullDown pupd, OutputType ot) = 0;
			virtual void makeInput() = 0;

		  private:
		};
	} // namespace drivers
}
