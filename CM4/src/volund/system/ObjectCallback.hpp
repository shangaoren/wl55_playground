#pragma once
#include "volund/interfaces/system/ICallable.hpp"

template <class objectType,class returnType,class ... parameterType>
class ObjectCallback : public interfaces::ICallable<returnType, parameterType ...>
{
  public:
	typedef returnType (objectType::*Callbackfunction)(parameterType ... args);
	constexpr ObjectCallback(objectType& self, Callbackfunction function) : m_object(&self), m_functionToCall(function)
	{
	}
	constexpr ObjectCallback(objectType *self, Callbackfunction function) : m_object(self), m_functionToCall(function)
	{
	}
	virtual returnType execute(parameterType ... params) override final
	{
		return (*m_object.*m_functionToCall)(params...);
	}

  private:
	objectType* m_object;
	Callbackfunction m_functionToCall;
};