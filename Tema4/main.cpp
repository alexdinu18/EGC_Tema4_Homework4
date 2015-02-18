//-------------------------------------------------------------------------------------------------
// Descriere: fisier main
//
// Autor: student
// Data: today
//-------------------------------------------------------------------------------------------------

//incarcator de meshe
#include "lab_mesh_loader.hpp"

//geometrie: drawSolidCube, drawWireTeapot...
#include "lab_geometry.hpp"

//incarcator de shadere
#include "lab_shader_loader.hpp"

//interfata cu glut, ne ofera fereastra, input, context opengl
#include "lab_glut.hpp"

#include "Objects.hpp"

//time
#include <ctime>

//camera
#include "lab_camera.hpp"

// frustum
#include "Frustum.hpp"

int camera = 1, view = 0, look_down = 0;
float z_cam = -200.1f, angle_Ox = 0.0f, angle_Oy = 0.0f;
unsigned int mesh_vbo_fr, mesh_ibo_fr, mesh_vao_fr, mesh_num_indices_fr;
glm::vec3 pos, up, forward, right;

class Laborator : public lab::glut::WindowListener{

//variabile
private:
	Frustum frustum;
	lab::Camera cam;
	glm::mat4 model_matrix, view_matrix, projection_matrix;								//matrici 4x4 pt modelare vizualizare proiectie
	unsigned int gl_program_shader_gouraud;												//id obiect shader gouraud
	unsigned int gl_program_shader_phong;												//id obiect shader phong
	unsigned int gl_program_shader_curent;												//id obiect shader curent
	unsigned int gl_program_shader_spotlight;											//id obiect shader spotlight

	unsigned int mesh_vbo, mesh_ibo, mesh_vao, mesh_num_indices;						//containere opengl pentru vertecsi, indecsi si stare

	unsigned int mesh_vbo_sphere, mesh_ibo_sphere, mesh_vao_sphere, mesh_num_indices_sphere;	//containere opengl pentru vertecsi, indecsi si stare pentru sfera ce va fi pusa la pozitia luminii

	unsigned int mesh_vbo_ground, mesh_ibo_ground, mesh_vao_ground, mesh_num_indices_ground;	//containere opengl pentru vertecsi, indecsi si stare pentru sol

	glm::vec3 sphere_position;
	glm::vec3 light_position;
	glm::vec3 eye_position;
	unsigned int material_shininess;
	float material_kd;
	float material_ks;

	std::vector<unsigned int> mesh_vbo_scene, mesh_ibo_scene, mesh_vao_scene, mesh_num_indices_scene;
	std::vector<unsigned int> heights;
	std::vector<glm::vec3> centers;
	Objects *scene;

	// dimensiunea matricii în care se află obiectele este n*n
	int n;
//metode
public:
	
	//constructor .. e apelat cand e instantiata clasa
	Laborator(){
		//setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		
		//incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader_gouraud = lab::loadShader("shadere\\shader_gouraud_vertex.glsl", "shadere\\shader_gouraud_fragment.glsl");
		gl_program_shader_phong = lab::loadShader("shadere\\shader_phong_vertex.glsl", "shadere\\shader_phong_fragment.glsl");
		gl_program_shader_curent = gl_program_shader_gouraud;

		//incarca o sfera
		lab::loadObj("resurse\\sphere.obj",mesh_vao_sphere, mesh_vbo_sphere, mesh_ibo_sphere, mesh_num_indices_sphere);	

		// genereaza scena
		n = 20;
		scene = new Objects(n, mesh_vbo_scene, mesh_ibo_scene, mesh_vao_scene, mesh_num_indices_scene, heights, centers);

		//lumina & material
		eye_position = glm::vec3(50,30,200);
		sphere_position = eye_position;
		light_position = glm::vec3(10,7,25);
		material_shininess = 100;
		material_kd = 0.5;
		material_ks = 0.5;


		//matrici de modelare si vizualizare
		model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		view_matrix = glm::lookAt(eye_position, glm::vec3(50,0,0), glm::vec3(0,1,0));
		
		// camera inițială
		cam.set(eye_position, eye_position + glm::vec3(0, 0, -40), glm::vec3(0, 1, 0));
		cam.translateForward(0.1);
		sphere_position = cam.get_position();
		sphere_position.z -= 40;
		sphere_position.y -= 10;
		pos = cam.get_position();
		up = cam.up;
		right = cam.right;
		forward = cam.forward;


		// frustum inițial
		frustum.updateFrustum(cam.get_position(), cam.forward, cam.up, cam.right);
		
		//desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//destructor .. e apelat cand e distrusa clasa
	~Laborator(){
		//distruge shadere
		glDeleteProgram(gl_program_shader_gouraud);
		glDeleteProgram(gl_program_shader_phong);

		//distruge mesh incarcat
		glDeleteBuffers(1,&mesh_vbo);
		glDeleteBuffers(1,&mesh_ibo);
		glDeleteVertexArrays(1,&mesh_vbo);

		//distruge sfera
		glDeleteBuffers(1,&mesh_vbo_sphere);
		glDeleteBuffers(1,&mesh_ibo_sphere);
		glDeleteVertexArrays(1,&mesh_vbo_sphere);

		//distruge solul
		glDeleteBuffers(1,&mesh_vbo_ground);
		glDeleteBuffers(1,&mesh_ibo_ground);
		glDeleteVertexArrays(1,&mesh_vbo_ground);
	}


	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame(){
	
	}
	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){
		//bufferele din framebuffer sunt aduse la valorile initiale (setate de clear color si cleardepth)
		//adica se sterge ecranul si se pune culoare (si alte propietati) initiala
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (camera == 1) {
			cam.view1(pos, forward, up, right);
		}
		else
			cam.view2();



		//foloseste shaderul
		glUseProgram(gl_program_shader_curent);
		light_position = sphere_position;
		light_position.x += 5;
		//eye_position = sphere_position;
		//eye_position.x += 15;
		//eye_position.y += 6;
		//trimite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "model_matrix"),1,false,glm::value_ptr(model_matrix));
		glm::mat4 view_matrix = cam.getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "view_matrix"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "light_position"),light_position.x, light_position.y, light_position.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "eye_position"),eye_position.x, eye_position.y, eye_position.z);
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "material_shininess"),material_shininess);
		glUniform1f(glGetUniformLocation(gl_program_shader_curent, "material_kd"),material_kd);
		glUniform1f(glGetUniformLocation(gl_program_shader_curent, "material_ks"),material_ks);
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "look_down"), look_down);

		// update frustum
		frustum.updateFrustum(sphere_position, forward, up, right);

		for (int i = 0; i < n * n; i++) {
			// view = 0, camera FPS
			// view = 1, camera de sus, obiect in frustum
			// view = 2, camera de sus, obiectul nu e in frustum
			if (frustum.isInFrustum(heights[i] / 2.0f, centers[i])) {
				if (camera == 2) {
					view = 1;
				}
				else {
					view = 0;
				}
				glUniform1i(glGetUniformLocation(gl_program_shader_curent, "view"), view);
				glBindVertexArray(mesh_vao_scene[i]);
				glDrawElements(GL_TRIANGLES, mesh_num_indices_scene[i], GL_UNSIGNED_INT, 0);
			}
			else {
				if (camera == 2) {
					view = 2;
					glUniform1i(glGetUniformLocation(gl_program_shader_curent, "view"), view);
					glBindVertexArray(mesh_vao_scene[i]);
					glDrawElements(GL_TRIANGLES, mesh_num_indices_scene[i], GL_UNSIGNED_INT, 0);
				}
			}
		}

		glm::mat4 matrice_translatie = glm::translate(model_matrix, glm::vec3(sphere_position.x, sphere_position.y, sphere_position.z));
		glm::mat4 matrice_scalare = glm::scale(model_matrix, glm::vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "model_matrix"), 1, false, glm::value_ptr(matrice_translatie*matrice_scalare));
		
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "view"), 0);
		glBindVertexArray(mesh_vao_sphere);
		glDrawElements(GL_TRIANGLES, mesh_num_indices_sphere, GL_UNSIGNED_INT, 0);

	}
	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame(){}
	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height){
		//reshape
		if(height==0) height=1;
		glViewport(0,0,width,height);
		projection_matrix = glm::perspective(90.0f, (float)width/(float)height,0.1f, 10000.0f);
	}


	//--------------------------------------------------------------------------------------------
	//functii de input output --------------------------------------------------------------------
	
	//tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == 27) lab::glut::close();	//ESC inchide glut si 
		if(key_pressed == 32) {
			//SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader_gouraud);
			glDeleteProgram(gl_program_shader_phong);
			gl_program_shader_gouraud = lab::loadShader("shadere\\shader_gouraud_vertex.glsl", "shadere\\shader_gouraud_fragment.glsl");
			gl_program_shader_phong = lab::loadShader("shadere\\shader_phong_vertex.glsl", "shadere\\shader_phong_fragment.glsl");
			gl_program_shader_curent = gl_program_shader_gouraud;
		}
		if(key_pressed == 'q'){
			static bool wire =true;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_LINE:GL_FILL));
			wire = !wire;
		}
		if(key_pressed == '1'){
			std::cout << "nr_elem = " << mesh_vbo_scene.size() << "\n";
			gl_program_shader_curent = gl_program_shader_gouraud;
		}
		if (key_pressed == '2'){
			if (look_down == 1)
				look_down = 0;
			else
				look_down = 1;
		}
		if (key_pressed == 'c'){
			if (camera == 1)
				camera = 2;
			else
				camera = 1;
			std::cout << "camera = " << camera << "\n";
		}
		// mișcare sferă
		// înainte
		if (key_pressed == 'w') {
			if (camera == 1) {
				cam.translateForward(0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
				std::cout << sphere_position.z << " " << pos.z << "\n";
			}
			else {
				cam.view1(pos, forward, up, right);
				cam.translateForward(0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
				cam.view2();
			}
		}
		// înapoi
		if (key_pressed == 's') {
			if (camera == 1) {
				cam.translateForward(-0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
			}
			else {
				cam.view1(pos, forward, up, right);
				cam.translateForward(-0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
				cam.view2();
			}
		}
		// dreapta
		if (key_pressed == 'a') {
			if (camera == 1) {
				cam.translateRight(-0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
			}
			else {
				cam.view1(pos, forward, up, right);
				cam.translateRight(-0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
				cam.view2();
			}
		}
		// stânga
		if (key_pressed == 'd') {
			if (camera == 1) {
				cam.translateRight(0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
			}
			else {
				cam.view1(pos, forward, up, right);
				cam.translateRight(0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
				cam.view2();
			}
		}
		// sus
		if (key_pressed == 'z') {
			if (camera == 1) {
				cam.translateUpword(0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
			}
			else {
				cam.view1(pos, forward, up, right);
				cam.translateUpword(0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
				cam.view2();
			}
		}
		// jos
		if (key_pressed == 'x') {
			if (camera == 1) {
				cam.translateUpword(-0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
			}
			else {
				cam.view1(pos, forward, up, right);
				cam.translateUpword(-0.5);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
				cam.view2();
			}
		}
		// rotație la dreapta pe Oy
		if (key_pressed == 'u') {
			if (camera == 1) {
				cam.rotateFPSoY(0.05f);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
			}
			else {
				cam.view1(pos, forward, up, right);
				cam.rotateFPSoY(0.05f);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
				cam.view2();
			}
		}
		// rotație la stânga pe Oy
		if (key_pressed == 'i') {
			if (camera == 1) {
				cam.rotateFPSoY(-0.05f);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
			}
			else {
				cam.view1(pos, forward, up, right);
				cam.rotateFPSoY(-0.05f);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
				cam.view2();
			}
		}
		// rotație în sus pe Ox
		if (key_pressed == 'o') {
			if (camera == 1) {
				cam.rotateFPSoX(0.05f);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
			}
			else {
				cam.view1(pos, forward, up, right);
				cam.rotateFPSoX(0.05f);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
				cam.view2();
			}
		}
		// rotație în jos pe Ox
		if (key_pressed == 'p') {
			if (camera == 1) {
				cam.rotateFPSoX(-0.05f);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
			}
			else {
				cam.view1(pos, forward, up, right);
				cam.rotateFPSoX(-0.05f);
				sphere_position = cam.get_position();
				sphere_position.z -= 40;
				sphere_position.y -= 10;
				pos = cam.get_position();
				up = cam.up;
				right = cam.right;
				forward = cam.forward;
				cam.view2();
			}
		}
	}
	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){	}
	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();
	}
	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y){}
	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ std::cout<<"Mouse scroll"<<std::endl;}

};

int main(){
	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("Tema 4"),800,600,100,100,true);
	lab::glut::ContextInfo context(2,1,false);
	lab::glut::FramebufferInfo framebuffer(true,true,true,true);
	lab::glut::init(window,context, framebuffer);

	//initializeaza GLEW (ne incarca functiile openGL, altfel ar trebui sa facem asta manual!)
	glewExperimental = true;
	glewInit();
	std::cout<<"GLEW:initializare"<<std::endl;

	//creem clasa noastra si o punem sa asculte evenimentele de la GLUT
	//DUPA GLEW!!! ca sa avem functiile de OpenGL incarcate inainte sa ii fie apelat constructorul (care creeaza obiecte OpenGL)
	Laborator mylab;
	lab::glut::setListener(&mylab);

	//taste
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl<<"\tw ... toggle wireframe"<<std::endl;
	std::cout<<"\t1 ... foloseste shader Gouraud"<<std::endl<<"\t2 ... foloseste shader Phong"<<std::endl;

	//run
	lab::glut::run();

	return 0;
}