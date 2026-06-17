#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace units
	{
		enum class force : uint32_t
		{
			N, kN, Last
		};
		enum class length : uint32_t
		{
			mm, cm, m, Last
		};
		enum class moment : uint32_t
		{
			Nmm, Nm, kNm, Last
		};
		enum class stress : uint32_t
		{
			Pa, kPa, MPa, GPa, Last
		};
		enum class temperature : uint32_t
		{
			C, K, Last
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