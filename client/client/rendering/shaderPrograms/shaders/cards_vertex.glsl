const char* cards_vertex = R"(
#version 400 core

in vec3 position;
in vec2 texCoords;
in mat4 modelViewProjectionMatrix;
in int textureId;

out vec3 pass_texCoords;

void main(void) {
	pass_texCoords = vec3(texCoords, textureId);
	
	gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
	//gl_Position = vec4(position, 1.0);
}

)";