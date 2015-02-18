//-------------------------------------------------------------------------------------------------
// Descriere: header in care este definita si implementata camera
// Nota:
//		camera este capabila de operatie de rotatie si translatie pe toate axele
//		camera este capabila de rotatii FP(first person) cat si TP(third person)
//
//-------------------------------------------------------------------------------------------------


#pragma once
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"

namespace lab{

	struct CamVertexFormat{ 
		glm::vec3 position, color; 
		CamVertexFormat(const glm::vec3 &p, const glm::vec3 &c){ 
			position=p; color=c;
		} 
	};


	class Camera{
	public:
		Camera(){
			//initializeaza camera
			position = glm::vec3(0,0,50);
			forward = glm::vec3(0,0,-1);
			up = glm::vec3(0,1,0);
			right = glm::vec3(1,0,0);

			//creaza geometry si update
			createGeometry();
			updateGeometry();
		}
		Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up){
			//set camera
			set(position, center,up);

			//update geometrie si update
			createGeometry();
			updateGeometry();
		}
		~Camera(){
			//distruge geometrie
			destroyGeometry();
		}
		glm::vec3 get_position(){
			return this->position;
		}
		void set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up){
			//update camera
			this->position = position;
			forward = glm::normalize(center-position);
			right = glm::cross(forward, up);
			this->up=glm::cross(right,forward);

			//update geometrie
			updateGeometry();
		}
	
		void view1(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 right) {
			this->position = pos;
			this->forward = forward;
			this->up = up;
			this->right = right;
			updateGeometry();
		}

		void view2(){
			set(glm::vec3(51, 120, 51), glm::vec3(50, 1, 50), glm::vec3(1, 1, 0));
		}
		
		// FPS up
		void translateForward(float distance){
			position += glm::normalize(forward) * distance;
			updateGeometry();
		}
		
		void translateUpword(float distance){
			position += glm::normalize(up) * distance;
			updateGeometry();
		}
		void translateRight(float distance){
			position += glm::normalize(right) * distance;
			updateGeometry();
		}
		void rotateFPSoX(float angle){
			up = up * cos (angle) + forward * sin (angle);
			forward = glm::cross(up,right);
			updateGeometry();
		}
		void rotateFPSoY(float angle){
			forward = forward * cos (angle) + right * sin (angle);
			right = glm::cross(forward,up);
			updateGeometry();
		}
		void rotateFPSoZ(float angle){
			right = right * cos(angle) + up*sin(angle);
			up = glm::cross(right,forward);
			updateGeometry();
		}
			
		glm::mat4 getViewMatrix(){
			return glm::lookAt(position,position + glm::normalize(forward), up);
		}

	private:
		//creeaza geometrie pentru axele camerei
		void createGeometry(){
			glGenVertexArrays(1,&vao);
			glBindVertexArray(vao);
			glGenBuffers(1,&vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glGenBuffers(1,&ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			std::vector<unsigned int> indices; for(unsigned int i=0;i<6;i++) indices.push_back(i);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int),&indices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(CamVertexFormat),(void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(CamVertexFormat),(void*)(sizeof(float)*3));	
		}
		//distruge geometria axelor camerei
		void destroyGeometry(){
			glDeleteVertexArrays(1,&vao);
			glDeleteBuffers(1,&vbo);
			glDeleteBuffers(1,&ibo);
		}
		//updateaza geometria axelor camerei
		void updateGeometry(){
			glBindVertexArray(vao);
			float axis_lenght=8;
			std::vector<CamVertexFormat> vertices;
			vertices.push_back( CamVertexFormat( position, glm::vec3(0,0,1)));
			vertices.push_back( CamVertexFormat( position + glm::normalize(forward)*axis_lenght, glm::vec3(0,0,1)));
			vertices.push_back( CamVertexFormat( position, glm::vec3(1,0,0)));
			vertices.push_back( CamVertexFormat( position + glm::normalize(right)*axis_lenght, glm::vec3(1,0,0)));
			vertices.push_back( CamVertexFormat( position, glm::vec3(0,1,0)));
			vertices.push_back( CamVertexFormat( position + glm::normalize(up)*axis_lenght, glm::vec3(0,1,0)));
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, 6*sizeof(CamVertexFormat),&vertices[0],GL_STATIC_DRAW);
		}
	public:
		//deseneaza geometria axelor camerei
		void drawGeometry(){/*
			glBindVertexArray(vao);
			glDrawElements(GL_LINES, 6,GL_UNSIGNED_INT, (void*)0);*/
		}

	public:
		//camera
		glm::vec3 position;
		glm::vec3 up;
		glm::vec3 forward;
		glm::vec3 right;
	private:
		//geometrie camera
		unsigned int vao,vbo,ibo;
	};
}