const char* table_fragment = R"(
#version 400 core

uniform vec3 lightPosition = vec3(0, 10, 7);
uniform float coneAngle = 12.0f;
uniform vec3 coneDirection = vec3(0, -0.6, -0.4);

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
	vec3 normalizedSurfaceToLight = normalize(lightPosition.xyz - fragPosition);
	float distanceToLight = length(lightPosition.xyz - fragPosition);

	// tex color
	vec4 surfaceColor = texture(tex, pass_texCoords);

    float lightToSurfaceAngle = degrees(acos(dot(-normalizedSurfaceToLight, normalize(coneDirection))));

	const float MIN_TEX_FACTOR = 0.001;
	float texFactor = MIN_TEX_FACTOR / (MIN_TEX_FACTOR + (1 - MIN_TEX_FACTOR) * exp(-0.95 * lightToSurfaceAngle));
	texFactor = clamp(texFactor, 0, 1);
	texFactor = 1 - texFactor;
	vec3 diffuse = vec3(0, 0, 0);
	diffuse += surfaceColor.rgb * texFactor;

	out_Color = vec4(diffuse, 1);
}
)";