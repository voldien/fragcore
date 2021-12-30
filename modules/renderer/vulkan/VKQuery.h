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
#ifndef _FRAG_CORE_VK_QUERY_H_
#define _FRAG_CORE_VK_QUERY_H_ 1
#include "../Query.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC VKQueryObject : public QueryObject {
	  public:
		virtual ~VKQueryObject();

	  public:


		virtual void begin(Target target, unsigned int index) override;


		virtual void end(Target target, unsigned int index) override;

		virtual long int getResult(int index) override;

		virtual void beginConditionalRenderer(Condition target) override;

		virtual void endConditionalRenderer()override;

		intptr_t getNativePtr() const override;
		private:
	};
} // namespace fragcore
#endif
