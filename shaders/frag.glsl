// ==================================================================
#version 330 core

// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// Take in our previous texture coordinates from a previous stage
// in the pipeline. In this case, texture coordinates are specified
// on a per-vertex level, so these would be coming in from the vertex
// shader.
in vec2 v_texCoord;
in vec3 myNormal;
in vec3 FragPos;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 

uniform mat4 view;
float specularStrength = 0.5f;

void main()
{
	// Our diffiuse color is now based on the texture we have loaded in.
	// We ge the fragment color by sampling from the
	// u_DiffuseMap (i.e. our texture in the sampler2D), 
	// at the s and t coordinates (i.e. v_texCoord).
	vec3 color =  texture(u_DiffuseMap, v_texCoord).rgb;

	vec3 normal = normalize(myNormal);

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