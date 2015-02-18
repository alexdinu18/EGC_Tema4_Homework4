#pragma once

#include <iostream>

#include "lab_mesh_loader.hpp"

struct VertexFormat{
	glm::vec3 pos;
	glm::vec3 c;

	VertexFormat(float x, float y, float z, int r, int g, int b){
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
		c[0] = r;
		c[1] = g;
		c[2] = b;
	}

};

class Objects{
public:
	Objects(int n, std::vector<unsigned int> & mesh_vbo, std::vector<unsigned int> & mesh_ibo,
		std::vector<unsigned int> & mesh_vao, std::vector<unsigned int> & mesh_num_indices,
		std::vector<unsigned int> & heights, std::vector<glm::vec3> & centers) {
		std::vector<VertexFormat> vertexes;
		std::vector<glm::uvec3> indexes;
		
		int y = 0;
		int nr_elem = 0;
		int height = 9;
		for (int i = 0; i < 2*n; i++) {
			int x = -n * 2 + i * 4 + 6;
			if (i%2)
			for (int j = 0; j < 2 * n; j++) {
				//int nr_elem = i * n + j;
				int z = -n * 2 + j * 4 + 6;
				if (j % 2) {
					height = rand() % 18 + 9;
					/*
					vertexes.push_back(VertexFormat(x,				 y,		z,				 0.6, 0.6, 0.6));
					vertexes.push_back(VertexFormat(x + (i + 1) * 4, y,		z,				 0.6, 0.6, 0.6));
					vertexes.push_back(VertexFormat(x,				 y +2*9,	z,				 0.6, 0.6, 0.6));
					vertexes.push_back(VertexFormat(x,				 y,		z + ( j + 1 ) *4,0.6, 0.6, 0.6));
					vertexes.push_back(VertexFormat(x + (i + 1) * 4, y + 2*9, z,				 0.6, 0.6, 0.6));
					vertexes.push_back(VertexFormat(x + (i + 1) * 4, y,		z + (j + 1) * 4, 0.6, 0.6, 0.6));
					vertexes.push_back(VertexFormat(x,				 y + 2*9, z + (j + 1) * 4, 0.6, 0.6, 0.6));
					vertexes.push_back(VertexFormat(x + (i + 1) * 4, y + 2*9, z + (j + 1) * 4, 0.6, 0.6, 0.6));
					*/

					vertexes.push_back(VertexFormat(x,		y,			z, 1,1,1));
					vertexes.push_back(VertexFormat(x + 4, y, z, 1, 1, 1));
					vertexes.push_back(VertexFormat(x, y + height, z, 1, 1, 1));
					vertexes.push_back(VertexFormat(x, y, z + 4, 1, 1, 1));
					vertexes.push_back(VertexFormat(x + 4, y + height, z, 1, 1, 1));
					vertexes.push_back(VertexFormat(x + 4, y, z + 4, 1, 1, 1));
					vertexes.push_back(VertexFormat(x, y + height, z + 4, 1, 1, 1));
					vertexes.push_back(VertexFormat(x + 4, y + height, z + 4, 1, 1, 1));

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

					mesh_vao.push_back(0);
					glGenVertexArrays(1, &mesh_vao[nr_elem]);
					glBindVertexArray(mesh_vao[nr_elem]);

					mesh_vbo.push_back(0);
					glGenBuffers(1, &mesh_vbo[nr_elem]);
					glBindBuffer(GL_ARRAY_BUFFER, mesh_vbo[nr_elem]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat)*vertexes.size(), &vertexes[0], GL_STATIC_DRAW);

					mesh_ibo.push_back(0);
					glGenBuffers(1, &mesh_ibo[nr_elem]);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_ibo[nr_elem]);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexes.size() * 3, &indexes[0], GL_STATIC_DRAW);

					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

					mesh_num_indices.push_back(indexes.size() * 3);
					heights.push_back(height);
					centers.push_back(glm::vec3(x + 2.0f, height / 2.0f, z + 2.0f));
					nr_elem++;
					vertexes.clear();
					indexes.clear();
				}
			}
		}
	}
};