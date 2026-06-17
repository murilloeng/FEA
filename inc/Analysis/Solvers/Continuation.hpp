#pragma once

//std
#include <cstdio>
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Nonlinear;
		}
	}
}

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Continuation
			{
			private:
				//constructor
				Continuation(Nonlinear*);

				//destructor
				~Continuation(void);

				//serialization
				void load(FILE*);
				void save(FILE*) const;

			public:
				//types
				enum class Type : uint32_t
				{
					MinimalNorm,
					ControlLoad,
					ControlState,
					ArcLengthSpherical,
					ArcLengthCylindrical,
					Last
				};

				//data
				Type type(Type);
				Type type(void) const;

				//continuation
				double predictor(void) const;
				double corrector(void) const;

				//type
				const char* type_name(void) const;
				static const char* type_name(Type);

			private:
				//size
				uint32_t size(void) const;

				//continuation
				double predictor_minimal_norm(void) const;
				double corrector_minimal_norm(void) const;
				double predictor_control_load(void) const;
				double corrector_control_load(void) const;
				double predictor_control_state(void) const;
				double corrector_control_state(void) const;
				double predictor_arc_length_spherical(void) const;
				double corrector_arc_length_spherical(void) const;
				double predictor_arc_length_cylindrical(void) const;
				double corrector_arc_length_cylindrical(void) const;

				//data
				Type m_type;
				Nonlinear* m_solver;

				//friends
				friend class Nonlinear;
			};
		}
	}
}