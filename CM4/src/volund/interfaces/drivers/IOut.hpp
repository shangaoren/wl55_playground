#pragma once

namespace interfaces
{
	namespace drivers
	{
		class IOut
		{
		  public:

			enum class PullUpPullDown
			{
				none,
				pullUp,
				pullDown,
			};

			enum class OutputType
			{
				pushPull,
				openDrain,
			};

			virtual void setup(PullUpPullDown pupd, OutputType ot) = 0;
			virtual void output(bool state) = 0;
			virtual bool output() = 0;

		  private:
		};
	} // namespace drivers
} // namespace interfaces
