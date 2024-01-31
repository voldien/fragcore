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
#ifndef _FRAG_CORE_GL_QUERY_H_
#define _FRAG_CORE_GL_QUERY_H_ 1
#include "../Query.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC GLQueryObject : public QueryObject {
	  public:
		~GLQueryObject() override;

	  public:
		/**
		 *
		 * @param target
		 * @param index
		 */
		void begin(Target target, unsigned int index) override;

		// TODO add for fetching the result.
		/**
		 *
		 * @param target
		 * @param index
		 */
		void end(Target target, unsigned int index) override;

		long int getResult(int index) override;

		/**
		 *
		 * @param target
		 */
		void beginConditionalRenderer(Condition target) override;

		/**
		 *
		 */
		void endConditionalRenderer() override;

		intptr_t getNativePtr() const override;

	  private:
		unsigned int query[8];
	};
} // namespace fragcore
#endif
