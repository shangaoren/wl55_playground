/*
 * utils.hpp
 *
 *  Created on: 21 avr. 2019
 *      Author: shang
 */

#pragma once

namespace utils
{

	template <typename T>
	T clamped(const T value, const T min, const T max)
	{
		if (value > max)
			return max;
		else
		{
			if (value < min)
				return min;
			else
				return value;
		}
	}

	template <typename T>
	void clamp(T &value, const T min, const T max)
	{
		if (value > max)
			value = max;

		if (value < min)
			value = min;
	}
} // namespace utils
