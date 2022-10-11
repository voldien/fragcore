/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_NORMAL_DISTRIBUTION_H_
#define _FRAG_CORE_NORMAL_DISTRIBUTION_H_ 1
#include "../../FragDef.h"
#include <climits>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC NormalDistribution {
	  public:
		NormalDistribution() = default;
		NormalDistribution(const Random &other) = default;
		NormalDistribution(Random &&other) = default;
		~NormalDistribution() = default;

		template <typename U> static U Guassian() { return 0; }

		template <typename U> static U GlorotNormal() { return 0; }

		template <typename U> static U RandomNormal() { return 0; }

		template <typename U> static U RandomUniform() { return 0; }
		
	};
} // namespace fragcore

#endif
