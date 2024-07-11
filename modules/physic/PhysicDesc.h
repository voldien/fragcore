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
#ifndef _FV_PHYSIC_DESC_H_
#define _FV_PHYSIC_DESC_H_ 1
#include "Collider.h"
#include "Core/Math3D.h"
#include "RigidBody.h"

namespace fragcore {

	/**
	 *	Constrains physic object.
	 */
	using ConstraintsDesc = struct constraints_desc {
		/**
		 *	Type of constraints
		 */
		enum ConstraintsType {
			ePoint, /*	*/
			eHinge, /*	*/
			eSpring,
			eSlider,
		};

		union {
			struct {

			} hinge;

			struct {

			} point;

			struct {
				float dampness;	 /*  */
				float stiffness; /*  */
			} spring;

			struct {

			} slider;
		};

		RigidBody *body1;	  /*	First end point.	*/
		RigidBody *body2;	  /*	Second end point.	*/
		ConstraintsType type; /*	Type of constraint.	*/
		float breakforce;	  /*	Force to break constraints.	*/
	};

	/**
	 * Physic description for creating collision
	 * object.
	 */
	using CollisionDesc = struct collision_desc_t {
		enum ShapePrimitive {
			Box,	 /*	*/
			Plane,	 /*	*/
			Sphere,	 /*	*/
			Capsule, /*	*/
			Mesh,	 /*	*/
			Terrain	 /*  	*/
		};
		ShapePrimitive Primitive = Plane; /*	Collision type.	*/
		float center[3] = {0, 0, 0};	  /*	Center of the collision object.	*/
		float alignscale = 1.0f;		  /*	factor scalar.	*/

		/*	*/
		union {
			struct /*plane_shape*/ {
				float normal[3];
				float d;
			} planeshape;

			struct /*box_shape*/ {
				float boxsize[3];
			} boxshape;

			struct /*sphere_shape*/ {
				float radius;
			} sphereshape;

			struct /*capsule_shape*/ {
				float radius;
				float height;
			} capsuleshape;

			struct /*TODO resolve parameters.*/ {
				//	Buffer *vertices;
				//	Buffer *indices;

				unsigned int stride;
			} terrainshape;

			/**
			 *
			 */
			struct /*mesh_shape.*/ {
				void *vertices;			   /*	*/
				void *indices;			   /*	*/
				unsigned int vertexcount;  /*	*/
				unsigned int indicescount; /*	*/
				unsigned int indicesStrip; /*	*/
			} meshshape;
		};
	};

	/**
	 *	Rigidbody description.
	 */
	using RigidBodyDesc = struct rigidbody_desc_t {

		Collision *collision = nullptr; /*	Collision shape attached to rigidbody.	*/
		float mass = 1.0f;				/*	Mass of the object.	*/
		float angularDrag = 0.05f;		/*	Drag for angular momentium, aka* rotation.	*/
		float drag = 0.05f;				/*	Linear drag.	*/
		bool useGravity = true;			/*	Use gravity.	*/
		float inertia = 1.0;			/*	Set the intertia of the object.	*/
		bool isKinematic = false;		/*	*/
		/*int RigidbodyConstraints*/

		/*	transformation	*/
		// TODO resolve if a pointer to allow for direct affect on the transform for instance.
		Vector3 position; /*	Position in world space.	*/
		Quaternion quat;  /*	Oritention in world space.	*/

		// Node *node;                        /*	Attached node.	*/
	};

	using CharacterControllerDesc = struct character_controller_t {
		CollisionDesc collisionDesc; // TODO remove.
		Collision *collision = nullptr;
	};

	using TerrainDesc = struct terrain_desc_t {

		//	Buffer *vertices;
		//		Buffer *indices;
		//
		unsigned int stride;
	};

	using SoftbodyDesc = struct softbody_desc_t {

		//	Geometry *geometryObject;

		float *vertices;
		unsigned int nVertices;

		int *indices;
		unsigned int nIndices;
	};

	using ClothDesc = struct cloth_desc_t {

		//	Geometry *geometryObject;
	};

} // namespace fragcore

#endif
