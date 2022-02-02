#ifndef _FRAG_CORE_RAY_CAST_H_
#define _FRAG_CORE_RAY_CAST_H_ 1
#include "Ray.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC VDRaycastHit {
	  public:
		VDRaycastHit();
		VDRaycastHit(const VDRaycastHit &hit);

		/**
		 *	Get point in worldspace of the hit.
		 */
		VDVector3 point() const;

		/*
		 *	Get normal surface the ray hit.
		 */
		VDVector3 normal() const;

		/**
		 *	Get distance between the ray caster and point
		 *	hitted.
		 */
		float distance() const;

		/**
		 *	Get barycentric coordinate of the triangle hitted.
		 */
		Vector2 barycentric() const;
	};

} // namespace fragcore

#endif
