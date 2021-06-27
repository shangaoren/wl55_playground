#pragma once
#include <cstdint>

namespace math
{
	class Vector
	{
	  public:
		constexpr Vector(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
		{
		}

		constexpr Vector() : m_x(0.f),
							 m_y(0.f),
							 m_z(0.f)
		{
		}

		template <class type>
		constexpr Vector(type x, type y, type z) : m_x(static_cast<float>(x)), 
												   m_y(static_cast<float>(y)), 
												   m_z(static_cast<float>(z))
		{
			
		}

		//getters
		float x() const
		{
			return m_x;
		}

		float y() const
		{
			return m_y;
		}

		float z() const
		{
			return m_z;
		}

		//setters
		template <typename type>
		void x(type value)
		{
			m_x = static_cast<float>(value);
		}

		template <typename type>
		void y(type value)
		{
			m_y = static_cast<float>(value);
		}

		template <typename type>
		void z(type value)
		{
			m_z = static_cast<float>(value);
		}

		Vector operator+(Vector const &right)
		{
			return (Vector(this->m_x + right.m_x,
						   this->m_y + right.m_y,
						   this->m_z + right.m_z));
		}

		Vector operator-(Vector const &right)
		{
			return (Vector(this->m_x - right.m_x,
						   this->m_y - right.m_y,
						   this->m_z - right.m_z));
		}

		template<typename T>
		Vector operator*(T const &right)
		{
			return (Vector(this->m_x * right, 
						   this->m_y * right, 
						   this->m_z * right));
		}

		template <typename T>
		Vector operator/(T const &right)
		{
			return (Vector(this->m_x / right,
						   this->m_y / right,
						   this->m_z / right));
		}

	  private:
		float m_x;
		float m_y;
		float m_z;
	};
} // namespace math
