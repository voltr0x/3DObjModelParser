// ==================================================================
#version 330 core

// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// Take in our previous texture coordinates.
in vec3 FragPos;
in vec2 v_texCoord;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;


// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 
uniform sampler2D u_NormalMap; 

void main()
{
	// Store the texture coordinates
	vec3 normal = texture(u_NormalMap, v_texCoord).rgb;
	vec3 color =  texture(u_DiffuseMap, v_texCoord).rgb;

	// Make normal to range of -1,1
	normal = normalize(normal * 2.0f - 1.0f);

	// Calculate ambient
	vec3 ambient = 0.1 * color;

	// Calculate diffuse
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * color;

	// Calculate specular
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);
	vec3 specular = vec3(0.2) * spec;

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
// ==================================================================