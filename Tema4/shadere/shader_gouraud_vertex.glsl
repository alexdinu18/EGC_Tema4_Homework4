
#version 330

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_normal;		

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;
uniform int look_down;

out float light;

void main(){
	vec3 world_pos;
	vec3 world_normal;
	float difuza,speculara;

	int primeste_lumina=0;

	world_pos = (model_matrix * vec4(in_position,1)).xyz;

	world_normal = normalize( mat3(model_matrix) * in_normal );

	vec3  V  =  normalize  (eye_position - world_pos);
	vec3 L=normalize(light_position - world_pos);
	vec3 H=normalize(L+V);
	if(look_down == 1) {
		L.x = 0;
		L.y = -1; 
		L.z = 0; // ma uit in jos
	}
	difuza=material_kd * max(dot(world_normal,L),0);

	if(dot(L,-normalize(V)) < cos(15*3.14159265359/180))	{ 
		primeste_lumina=1;
	
		speculara= material_ks * primeste_lumina* pow(max(dot(world_normal,H),0),material_shininess);
		light=2*difuza + speculara;
	}
	else {
		light = 0;
	}

	//TODO

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position,1); 
}
