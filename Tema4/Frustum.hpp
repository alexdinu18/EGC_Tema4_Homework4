#pragma once

#include <iostream>

#include "lab_mesh_loader.hpp"

struct Plane{
	float a, b, c, d;
};
class Frustum {
public:
	Plane Near, Far, Left, Right, Bottom, Top;
	// colturile trunchiului de piramida
	// n* - definesc planul near
	// f* - definesc planul far
	glm::vec3 ntr, ntl, nbr, nbl, ftr, ftl, fbr, fbl;
	std::vector<Plane> planes;
	bool first = true;

	void updateFrustum(glm::vec3 p, glm::vec3 d, glm::vec3 up, glm::vec3 right) {
		float nearDist = 15.0f, farDist = 70.0f;
		float Wfar = 70.0f;
		float Hfar =  20.0f;
		float Wnear = 15.0f; 
		float Hnear = 5.0f;

		glm::vec3 fc = p + d * farDist;

		ftl = fc + (up * (Hfar / 2)) - (right * (Wfar / 2));
		ftr = fc + (up * (Hfar / 2)) + (right * (Wfar / 2));
		fbl = fc - (up * (Hfar / 2)) - (right * (Wfar / 2));
		fbr = fc - (up * (Hfar / 2)) + (right * (Wfar / 2));
		
		glm::vec3 nc = p + d * nearDist;
		
		ntl = nc + (up * (Hnear / 2)) - (right * (Wnear / 2));
		ntr = nc + (up * (Hnear / 2)) + (right * (Wnear / 2));
		nbl = nc - (up * (Hnear / 2)) - (right * (Wnear / 2));
		nbr = nc - (up * (Hnear / 2)) + (right * (Wnear / 2));

		glm::vec3 u, v, n;
		
		// top
		v = ntl - ntr;
		u = ftl - ntr;
		n = glm::cross(v, u);
		n = glm::normalize(n);
		Top.a = n.x;
		Top.b = n.y;
		Top.c = n.z;
		Top.d = glm::dot(-n, ntr);

		// bottom
		v = nbr - nbl;
		u = fbr - nbl;
		n = glm::cross(v, u);
		n = glm::normalize(n);
		Bottom.a = n.x;
		Bottom.b = n.y;
		Bottom.c = n.z;
		Bottom.d = glm::dot(-n, nbl);


		// left
		v = nbl - ntl;
		u = fbl - ntl;
		n = glm::cross(v, u);
		n = glm::normalize(n);
		Left.a = n.x;
		Left.b = n.y;
		Left.c = n.z;
		Left.d = glm::dot(-n, ntl);


		// right
		v = ntr - nbr;
		u = fbr - nbr;
		n = glm::cross(v, u);
		n = glm::normalize(n);
		Right.a = n.x;
		Right.b = n.y;
		Right.c = n.z;
		Right.d = glm::dot(-n,  nbr);


		// near
		v = ntr - ntl;
		u = nbr - ntl;
		n = glm::cross(v, u);
		n = glm::normalize(n);
		Near.a = n.x;
		Near.b = n.y;
		Near.c = n.z;
		Near.d = glm::dot(-n, ntl);


		// far
		v = ftl - ftr;
		u = fbl - ftr;
		n = glm::cross(v, u);
		n = glm::normalize(n);
		Far.a = n.x;
		Far.b = n.y;
		Far.c = n.z;
		Far.d = glm::dot(-n, ftr);


		if (first) {
			planes.push_back(Top);
			planes.push_back(Bottom);
			planes.push_back(Left);
			planes.push_back(Right);
			planes.push_back(Near);
			planes.push_back(Far);
			first = false;
		}
		else {
			planes[0] = Top;
			planes[1] = Bottom;
			planes[2] = Left;
			planes[3] = Right;
			planes[4] = Near;
			planes[5] = Far;

		}
	}
	void printPlanes() {
		std::cout << "Top: " << Top.a << " " << Top.b << " " << Top.c << " " << Top.d << "\n";
		std::cout << "Bottom: " << Bottom.a << " " << Bottom.b << " " << Bottom.c << " " << Bottom.d << "\n";
		std::cout << "Left: " << Left.a << " " << Left.b << " " << Left.c << " " << Left.d << "\n";
		std::cout << "Right: " << Right.a << " " << Right.b << " " << Right.c << " " << Right.d << "\n";
		std::cout << "Near: " << Near.a << " " << Near.b << " " << Near.c << " " << Near.d << "\n";
		std::cout << "Far: " << Far.a << " " << Far.b << " " << Far.c << " " << Far.d << "\n";

	}

	void build_frustum(unsigned int* mesh_vbo_fr, unsigned int* mesh_ibo_fr,
		unsigned int* mesh_vao_fr, unsigned int* mesh_num_indices_fr) {
		std::vector<VertexFormat> vertexes;
		std::vector<glm::uvec3> indexes;

		vertexes.push_back(VertexFormat(fbl.x, fbl.y, fbl.z, 1, 1, 1));
		vertexes.push_back(VertexFormat(fbr.x, fbr.y, fbr.z, 1, 1, 1));
		vertexes.push_back(VertexFormat(ftl.x, ftl.y, ftl.z, 1, 1, 1));
		vertexes.push_back(VertexFormat(nbl.x, nbl.y, nbl.z, 1, 1, 1));
		vertexes.push_back(VertexFormat(ftr.x, ftr.y, ftr.z, 1, 1, 1));
		vertexes.push_back(VertexFormat(nbr.x, nbr.y, nbr.z, 1, 1, 1));
		vertexes.push_back(VertexFormat(ntl.x, ntl.y, ntl.z, 1, 1, 1));
		vertexes.push_back(VertexFormat(ntr.x, ntr.y, ntr.z, 1, 1, 1));

		indexes.push_back(glm::uvec3(0, 1, 2));
		indexes.push_back(glm::uvec3(2, 4, 1));
		indexes.push_back(glm::uvec3(4, 2, 7));
		indexes.push_back(glm::uvec3(6, 7, 2));
		indexes.push_back(glm::uvec3(4, 1, 7));
		indexes.push_back(glm::uvec3(5, 1, 7));
		indexes.push_back(glm::uvec3(0, 2, 6));
		indexes.push_back(glm::uvec3(0, 6, 3));
		indexes.push_back(glm::uvec3(3, 5, 6));
		indexes.push_back(glm::uvec3(6, 5, 7));
		indexes.push_back(glm::uvec3(3, 0, 5));
		indexes.push_back(glm::uvec3(0, 5, 1));

		glGenVertexArrays(1, mesh_vao_fr);
		glBindVertexArray(*mesh_vao_fr);
	
		glGenBuffers(1, mesh_vbo_fr);
		glBindBuffer(GL_ARRAY_BUFFER, *mesh_vbo_fr);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat)*vertexes.size(), &vertexes[0], GL_STATIC_DRAW);

		glGenBuffers(1, mesh_ibo_fr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *mesh_ibo_fr);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexes.size() * 3, &indexes[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

		*mesh_num_indices_fr = indexes.size() * 3;
		vertexes.clear();
		indexes.clear();
	}
	
	// verifica daca un obiect de afla in frustum
	bool isInFrustum(float radius, glm::vec3 center) {
		float distance;

		for (int i = 0; i < 6; i++) {
			distance = (planes[i].a*center.x + planes[i].b * center.y + planes[i].c * center.z + planes[i].d);
			if (distance < -radius)
				return false;
		}
		return true;
	}
};