const char* gaussianBlur_horizontal_vertex = R"(

#version 400 core

in vec2 position;

out vec2[11] blurTextureCoords;

uniform float fboWidth;

void main(void) {
	gl_Position = vec4(position, 0.0, 1.0);
	
	vec2 centerTexCoords = position * 0.5 + 0.5;
	float pixelSize = 1.0 / fboWidth;
	
	for(int counter = -5; counter <= 5; counter++) {
		blurTextureCoords[counter + 5] = centerTexCoords + vec2(pixelSize * counter, 0.0);
	}
}

)";