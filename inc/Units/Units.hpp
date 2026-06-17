#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace units
	{
		enum class force : uint32_t
		{
			N, kN, last
		};
		enum class length : uint32_t
		{
			mm, cm, m, last
		};
		enum class moment : uint32_t
		{
			Nmm, Nm, kNm, last
		};
		enum class stress : uint32_t
		{
			Pa, kPa, MPa, GPa, last
		};
		enum class temperature : uint32_t
		{
			C, K, last
		};

		const char* name(force);
		const char* name(length);
		const char* name(moment);
		const char* name(stress);
		const char* name(temperature);

		double to_SI(double, force);
		double to_SI(double, length);
		double to_SI(double, moment);
		double to_SI(double, stress);
		double to_SI(double, temperature);

		double from_SI(double, force);
		double from_SI(double, length);
		double from_SI(double, moment);
		double from_SI(double, stress);
		double from_SI(double, temperature);
	}
}