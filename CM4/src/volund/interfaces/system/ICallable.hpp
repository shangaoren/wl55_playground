#pragma once
namespace interfaces
{
	template <class returnType, class ... parameterType>
	class ICallable
	{
		public:
		virtual returnType execute(parameterType ... args) = 0;
	};
}