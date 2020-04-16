const char* defaultPostProcessing_vertex = R"(

#version 400 core

in vec2 position;

out vec2 pass_textureCoords;



void main(void) {
	gl_Position = vec4(position, 0.0, 1.0);
	
	pass_textureCoords = position * 0.5 + 0.5;

}

)";