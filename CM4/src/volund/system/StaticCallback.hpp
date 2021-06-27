#pragma once

#include "volund/interfaces/system/ICallable.hpp"

template <class ReturnType, class ... ParameterType>
	class StaticCallback : public interfaces::ICallable < ReturnType,ParameterType...>
{
  public:
	typedef ReturnType (*CallbackFunction)(ParameterType ...);

	constexpr StaticCallback(CallbackFunction function) : m_function(function)
	{
	}

	virtual ReturnType execute(ParameterType ... args) override final
	{
		return m_function(args...);
	}


  private:
	CallbackFunction m_function;
};