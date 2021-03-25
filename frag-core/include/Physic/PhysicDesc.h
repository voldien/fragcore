/**
	Simple physic simulation with a server-client model support.
	Copyright (C) 2016  Valdemar Lindberg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FV_PHYSIC_DESC_H_		/*	TODO determine if this is the best header name or not.	*/
#define _FV_PHYSIC_DESC_H_ 1
#include "../Renderer/Buffer.h"
#include "RigidBody.h"
#include "Collider.h"

namespace fragcore {

	/**
	 *	Constrains physic object.
	*/
	typedef struct constraints_desc {
		/**
		 *	Type of constraints
		 */
		enum ConstraintsType {
			ePoint,                        /*	*/
			eHinge,                        /*	*/
			eSpring,
			eSlider,
		};

		union {
			struct {

			} hinge;

			struct {

			} point;

			struct {
				float dampness;     /*  */
				float stiffness;    /*  */
			} spring;

			struct {

			} slider;
		};

		RigidBody *body1;                /*	First end point.	*/
		RigidBody *body2;                /*	Second end point.	*/
		ConstraintsType type;            /*	Type of constraint.	*/
		float breakforce;                /*	Force to break constraints.	*/

	}
	ConstraintsDesc;

	/**
	 * Physic description for creating collision
	 * object.
	 */
	typedef struct collision_desc_t {
		//TODO determine if relocated to add scope global of fragcore.
		enum ShapePrimitive {
			eBox,                    /*	*/
			ePlane,                    /*	*/
			eSphere,                /*	*/
			eCapsule,                /*	*/
			eMesh,                    /*	*/
			eTerrain                /*  */
		};
		ShapePrimitive Primitive;    /*	Collision type.	*/
		float center[3];            /*	Center of the collision object.	*/
		float alignscale;            /*	factor scalar.	*/

		/**/
		union {
			struct /*plane_shape*/{
				float normal[3];
				float d;
			} planeshape;

			struct /*box_shape*/{
				float boxsize[3];
			} boxshape;

			struct /*sphere_shape*/{
				float radius;
			} sphereshape;

			struct /*capsule_shape*/{
				float radius;
				float height;
			} capsuleshape;

			struct /*TODO resolve parameters.*/{
				Buffer *vertices;
				Buffer *indices;

				unsigned int stride;
			} terrainshape;

			/**
			 *
			 */
			struct /*mesh_shape.*/{
				void *vertices;                /*	*/
				void *indices;                /*	*/
				unsigned int vertexcount;    /*	*/
				unsigned int indicescount;    /*	*/
				unsigned int indicesStrip;    /*	*/
			} meshshape;
		};

	}
	CollisionDesc;

	/**
	 *	Rigidbody description.
	*/
	typedef struct rigidbody_desc_t {

		Collision *collision;            /*	Collision shape attached to rigidbody.	*/
		float mass;                        /*	Mass of the object.	*/
		float angularDrag;                /*	Drag for angular momentium, aka* rotation.	*/
		float drag;                        /*	Linear drag.	*/
		bool useGravity;                /*	Use gravity.	*/
		float inertia;                    /*	Set the intertia of the object.	*/
		bool isKinematic;                /*	*/
		/*int RigidbodyConstraints*/

		/*	transformation	*/
		//TODO resolve if a pointer to allow for direct affect on the transform for instance.
		Vector3 position;                	/*	Position in world space.	*/
		Quaternion quat;                	/*	Oritention in world space.	*/

		//Node *node;                        /*	Attached node.	*/

	}
	RigidBodyDesc;


	typedef struct character_controller_t {
		CollisionDesc collisionDesc;	//TODO remove.
		Collision *collision;
	}
	CharacterControllerDesc;

	typedef struct terrain_desc_t {

		Buffer *vertices;
		Buffer *indices;

		unsigned int stride;

	}
	TerrainDesc;

	typedef struct softbody_desc_t {

		Geometry *geometryObject;

		float *vertices;
		unsigned int nVertices;

		int *indices;
		unsigned int nIndices;


	}
	SoftbodyDesc;

	typedef struct cloth_desc_t {

		Geometry *geometryObject;

	}
	ClothDesc;

}

#endif
