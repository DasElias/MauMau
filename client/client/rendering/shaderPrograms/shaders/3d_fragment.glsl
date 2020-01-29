const char* threed_fragment = R"(
#version 400 core

const vec3 lightPosition = vec3(0, 2, 7);
const vec3 lightIntensity = vec3(1, 1, 1);

in vec2 pass_texCoords;
in vec3 pass_position;
in vec3 pass_normal;

out vec4 out_Color;

uniform sampler2D tex;
uniform mat4 modelMatrix;

void main(void) {
	// calulate normal in world coordinates
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	vec3 normal = normalize(normalMatrix * pass_normal);

	// location of this fragment in world coordinates
	vec3 fragPosition = vec3(modelMatrix * vec4(pass_position, 1));

	// vector from pixels surface to the light source
	vec3 surfaceToLight = lightPosition - fragPosition;

	float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
	brightness = clamp(brightness, 0, 1);

	vec4 surfaceColor = texture(tex, pass_texCoords);
	out_Color = vec4(brightness * lightIntensity * surfaceColor.rgb, surfaceColor.a);
}
)";

const char* threed_fragment_discardInvisible = R"(
#version 400 core

in vec2 pass_texCoords;

out vec4 out_Color;

uniform sampler2D tex;

void main(void) {
	out_Color = texture(tex, pass_texCoords);
	
	if(out_Color.a < 0.5) discard;
}
)";