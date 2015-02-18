#version 330
layout(location = 0) out vec4 out_color;

in float light;
uniform int view;

void main(){
	if (view == 0)
		out_color = vec4(light,light,light,1);
	else if (view == 1)
		out_color = vec4(0,1,0,1);
	else
		out_color = vec4(1,0,0,1);
}