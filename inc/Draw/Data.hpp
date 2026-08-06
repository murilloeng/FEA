#pragma once

//std
#include <cstdint>

//FEA
#include "FEA/inc/Draw/Colors.hpp"

//Canvas
#include "Canvas/inc/Buffers/IBO.hpp"
#include "Canvas/inc/Buffers/VBO.hpp"

namespace fea
{
	namespace draw
	{
		struct Data
		{
			uint32_t m_step;
			const Colors& m_colors;

			uint32_t& m_index_dots;
			uint32_t& m_index_edges;
			uint32_t& m_index_faces;
			uint32_t& m_index_vertices;

			uint32_t& m_counter_dots;
			uint32_t& m_counter_edges;
			uint32_t& m_counter_faces;
			uint32_t& m_counter_vertices;

			canvas::buffers::IBO& m_ibo;
			canvas::buffers::VBO& m_vbo;

			const float* m_positions_data;
			const float* m_rotations_data;
		};
	}
}