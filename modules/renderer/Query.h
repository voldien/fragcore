/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAGCORE_QUERY_H_
#define _FRAGCORE_QUERY_H_ 1
#include "RenderObject.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC QueryObject : public RenderObject {
	  public:
		~QueryObject() override = default;

		/**
		 *
		 */
		enum Target {
			Samples, /*  */
			AnySamples,
			AnySamplesInverted,

			// GL_ANY_SAMPLES_PASSED_CONSERVATIVE
			// GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
			PrimitiveGenerate, /*  */
			TimePassed,		   /*  */
			eCurrentTime,
			// GL_TIME_ELAPSED
			// GL_TIMESTAMP
		};

		enum Condition {
			eNoWait = 0x1
			// GL_QUERY_NO_WAIT​
			// GL_QUERY_WAIT​
			// GL_QUERY_BY_REGION_WAIT
			// GL_QUERY_BY_REGION_NO_WAIT
		};

		/**
		 *
		 * @param target
		 * @param index
		 */
		virtual void begin(Target target, unsigned int index) = 0;

		// TODO add for fetching the result.
		/**
		 *
		 * @param target
		 * @param index
		 */
		virtual void end(Target target, unsigned int index) = 0;

		virtual long int getResult(int index) = 0;

		/**
		 *
		 * @param target
		 */
		virtual void beginConditionalRenderer(Condition target) = 0;

		/**
		 *
		 */
		virtual void endConditionalRenderer() = 0;
	};
} // namespace fragcore
#endif
