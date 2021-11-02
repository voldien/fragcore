#ifndef _FV_PHYSIC_DEBUGDRAWER_H_
#define _FV_PHYSIC_DEBUGDRAWER_H_ 1
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
