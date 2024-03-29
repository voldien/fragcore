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
#ifndef _FV_PHYSIC_DEBUGDRAWER_H_
#define _FV_PHYSIC_DEBUGDRAWER_H_ 1
#include <FragCore.h>
#include <bullet/LinearMath/btIDebugDraw.h>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
// #include <Renderer/IRenderer.h>
// #include <Renderer/CommandList.h>
#include "internal_object_type.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC DebugDrawer : public btIDebugDraw {
	  public:
		// DebugDrawer(Ref<IRenderer>& renderer);

		DefaultColors getDefaultColors() const override;

		void setDefaultColors(const DefaultColors &colors) override;

		void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;

		void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime,
							  const btVector3 &color) override;

		void reportErrorWarning(const char *warningString) override;

		void draw3dText(const btVector3 &location, const char *textString) override;

		void setDebugMode(int debugMode) override;

		int getDebugMode() const override;

		void clearLines() override;

		void flushLines() override;

	  private:
		// Ref<IRenderer> renderer;
		// Ref<CommandList> cb;
		// TODO add list of vector cached.
	};
} // namespace fragcore

#endif
