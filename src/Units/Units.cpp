//FEA
#include "FEA/inc/Units/Units.hpp"

namespace fea
{
	namespace units
	{
		const char* name(force unit)
		{
			const char* names[] = {"N", "kN"};
			return names[uint32_t(unit)];
		}
		const char* name(length unit)
		{
			const char* names[] = {"mm", "cm", "m"};
			return names[uint32_t(unit)];
		}
		const char* name(moment unit)
		{
			const char* names[] = {"Nmm", "Nm", "kNm"};
			return names[uint32_t(unit)];
		}
		const char* name(stress unit)
		{
			const char* names[] = {"Pa", "kPa", "MPa", "GPa"};
			return names[uint32_t(unit)];
		}
		const char* name(temperature unit)
		{
			const char* names[] = {"C", "K"};
			return names[uint32_t(unit)];
		}

		double to_SI(double scalar, force unit)
		{
			const double values[] = {1.00e+00, 1.00e+03};
			return scalar * values[uint32_t(unit)];
		}
		double to_SI(double scalar, length unit)
		{
			const double values[] = {1.00e-03, 1.00e-02, 1.00e+00};
			return scalar * values[uint32_t(unit)];
		}
		double to_SI(double scalar, moment unit)
		{
			const double values[] = {1.00e-03, 1.00e+00, 1.00e+03};
			return scalar * values[uint32_t(unit)];
		}
		double to_SI(double scalar, stress unit)
		{
			const double values[] = {1.00e+00, 1.00e+03, 1.00e+06, 1.00e+09};
			return scalar * values[uint32_t(unit)];
		}
		double to_SI(double scalar, temperature unit)
		{
			return unit == temperature::K ? scalar : scalar + 273.15;
		}

		double from_SI(double scalar, force unit)
		{
			const double values[] = {1.00e+00, 1.00e-03};
			return scalar * values[uint32_t(unit)];
		}
		double from_SI(double scalar, length unit)
		{
			const double values[] = {1.00e+03, 1.00e+02, 1.00e+00};
			return scalar * values[uint32_t(unit)];
		}
		double from_SI(double scalar, moment unit)
		{
			const double values[] = {1.00e+03, 1.00e+00, 1.00e-03};
			return scalar * values[uint32_t(unit)];
		}
		double from_SI(double scalar, stress unit)
		{
			const double values[] = {1.00e+00, 1.00e-03, 1.00e-06, 1.00e-09};
			return scalar * values[uint32_t(unit)];
		}
		double from_SI(double scalar, temperature unit)
		{
			return unit == temperature::K ? scalar : scalar - 273.15;
		}
	}
}