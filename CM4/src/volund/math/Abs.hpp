#pragma once

namespace math
{
	template <typename type>
	static type abs(type number)
	{
		if (number < 0)
			return -number;
		else
			return number;
	}
}
