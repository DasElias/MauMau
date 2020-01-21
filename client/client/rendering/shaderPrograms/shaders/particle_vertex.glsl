const char* particle_vertex = R"(
#version 400 core

in vec2 position;
in vec4 texOffsets;
in mat4 modelViewMatrix;
in float blendFactor;
in vec2 displacementOnScreen;

out vec2 pass_textureCoords1;
out vec2 pass_textureCoords2;
out float pass_blendFactor;

uniform mat4 projectionMatrix;
uniform int textureRowsCount;



void main(void) {
	vec2 textureCoords = position + vec2(0.5, 0.5);
	textureCoords.y /= textureRowsCount;
	
	pass_textureCoords1 = textureCoords + texOffsets.xy;
	pass_textureCoords2 = textureCoords + texOffsets.zw;
	pass_blendFactor = blendFactor;
	
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 0, 1.0);
	gl_Position.xy += displacementOnScreen * gl_Position.w;
}


)";