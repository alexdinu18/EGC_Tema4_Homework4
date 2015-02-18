

#version 330
layout(location = 0) out vec4 out_color;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;

in vec3 world_pos;
in vec3 world_normal;
void main(){
	float light = 0;
	float difuza,speculara;
	float d = distance(world_normal, light_position);

	int primeste_lumina=0;

	vec3  V  =  normalize  (eye_position - world_pos);
	vec3 L=normalize(light_position - world_pos);
	vec3 H=normalize(L+V);

	difuza=material_kd * max(dot(world_normal,L),0);

	if(dot(world_normal,L) > 0) primeste_lumina=1;
	else primeste_lumina=0;
	speculara= material_ks * primeste_lumina* pow(max(dot(world_normal,H),0),material_shininess);
	light=2*difuza + speculara;
	float atenuare = 1 / (d * d);

	light=atenuare * (difuza + speculara) + difuza * 1.2;

	//TODO

	out_color = vec4(light,light, light,1);
}
