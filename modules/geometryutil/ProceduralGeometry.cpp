#include "ProceduralGeometry.h"

using namespace fragcore;

void ProceduralGeometry::generateCube(float scale, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {

	const std::vector<Vertex> vertice = {
		{-1.0f, -1.0f, -1.0f, 0, 0},							 // triangle 1 : begin
		{-1.0f, -1.0f, 1.0f, 0, 1},	 {-1.0f, 1.0f, 1.0f, 1, 1},	 // triangle 1 : end
		{1.0f, 1.0f, -1.0f, 1, 1},								 // triangle 2 : begin
		{-1.0f, -1.0f, -1.0f, 1, 0}, {-1.0f, 1.0f, -1.0f, 0, 0}, // triangle 2 : end
		{1.0f, -1.0f, 1.0f, 0, 0},	 {-1.0f, -1.0f, -1.0f, 0, 1}, {1.0f, -1.0f, -1.0f, 1, 1},
		{1.0f, 1.0f, -1.0f, 0, 0},	 {1.0f, -1.0f, -1.0f, 1, 1},  {-1.0f, -1.0f, -1.0f, 1, 0},
		{-1.0f, -1.0f, -1.0f, 0, 0}, {-1.0f, 1.0f, 1.0f, 0, 1},	  {-1.0f, 1.0f, -1.0f, 1, 1},
		{1.0f, -1.0f, 1.0f, 0, 0},	 {-1.0f, -1.0f, 1.0f, 1, 1},  {-1.0f, -1.0f, -1.0f, 0, 1},
		{-1.0f, 1.0f, 1.0f, 0, 0},	 {-1.0f, -1.0f, 1.0f, 0, 1},  {1.0f, -1.0f, 1.0f, 1, 1},
		{1.0f, 1.0f, 1.0f, 0, 0},	 {1.0f, -1.0f, -1.0f, 1, 1},  {1.0f, 1.0f, -1.0f, 1, 0},
		{1.0f, -1.0f, -1.0f, 0, 0},	 {1.0f, 1.0f, 1.0f, 0, 1},	  {1.0f, -1.0f, 1.0f, 1, 1},
		{1.0f, 1.0f, 1.0f, 0, 0},	 {1.0f, 1.0f, -1.0f, 1, 1},	  {-1.0f, 1.0f, -1.0f, 0, 1},
		{1.0f, 1.0f, 1.0f, 0, 0},	 {-1.0f, 1.0f, -1.0f, 0, 1},  {-1.0f, 1.0f, 1.0f, 1, 1},
		{1.0f, 1.0f, 1.0f, 0, 0},	 {-1.0f, 1.0f, 1.0f, 1, 1},	  {1.0f, -1.0f, 1.0f, 1, 0}};

	vertices = vertice;
}