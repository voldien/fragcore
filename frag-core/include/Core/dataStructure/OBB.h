
#ifndef _FRAG_CORE_ORIENTED_BOUNDING_BOX_H_
#define _FRAG_CORE_ORIENTED_BOUNDING_BOX_H_ 1
#include "ONB.h"

namespace fragcore {
	/**
	 *	Oriented bounding box.
	 */
	struct FVDECLSPEC OBB {
	  public:
		OBB() = default;

		OBB(const OBB &obb) { *this = obb; }

		OBB(const Vector3 &u, const Vector3 &v, const Vector3 &w) { this->onb.set(u, v, w); }

		OBB(const Vector3 &u, const Vector3 &v, const Vector3 &w, const Vector3 &size) {
			this->onb.set(u, v, w);
			this->setSize(size);
		}

		OBB(const Vector3 &u, const Vector3 &v, const Vector3 &w, const Vector3 &center, const Vector3 &size){

		}

		/**
		 * Get U axis.
		 * @return normalized axis.
		 */
		const Vector3 & getU() const { return onb.u(); }

		/**
		 *	Get V axis.
		 * @return normalized axis.
		 */

		/**
		 *
		 * @return
		 */
		const Vector3 & getV() const { return onb.v(); }

		/**
		 * Get W axis.
		 * @return normalized axis.
		 */
		const Vector3 & getW() const { return onb.w(); }

		/**
		 * Set size of box.
		 * @param size
		 */
		void setSize(const Vector3 &size) { this->size = size; }

		/**
		 * Get half size.
		 * @return non-negative vector of the size.
		 */
		inline Vector3 getSize() { return size; }

		/**
		 *
		 * @return
		 */
		inline const Vector3 &getSize() const { return this->size; }

		/**
		 *
		 * @param size
		 */
		inline void setCenter(const Vector3 &size) {}

		/**
		 *
		 * @return
		 */
		inline Vector3 getCenter() const { return Vector3(this->halfu, this->halfv, this->halfw); }

		OBB &operator=(const OBB &obb) {
			this->onb = obb.onb;
			this->setSize(obb.getSize());
			return *this;
		}

		/**
		 *
		 * @param is
		 * @param t
		 * @return stream reference.
		 */
		friend std::istream &operator>>(std::istream &is, OBB &t) { return is; }

		/**
		 *
		 * @param is
		 * @param t
		 * @return stream reference.
		 */
		friend std::ostream &operator<<(std::ostream &os, const OBB &t) { return os; }

	  private:		  /*	Attributes.	*/
		ONB onb;	  /*	*/
		Vector3 size; /*	*/
		float halfu;  /*	*/
		float halfv;  /*	*/
		float halfw;  /*	*/
	};
} // namespace fragcore

#endif
