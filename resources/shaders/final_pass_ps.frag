#version 330 core
#define MAX_LIGHTS 16

in vec2 vTexCoord;

out vec4 outColor;		// Final output color

// G-Buffer
uniform sampler2D tex0;	// Albedo
uniform sampler2D tex1;	// Position
uniform sampler2D tex2;	// Normals

uniform sampler2D tex3;	// Post-process (SSAO + Blur)

uniform bool use_ssao;

uniform mat4 uView;

struct Light
{
	float power;
	vec3 position;
	vec3 direction;
	vec3 color;
};

uniform Light lights[MAX_LIGHTS];
uniform int light_count;

void main()
{
	vec3 albedo = texture(tex0, vTexCoord).rgb;

	vec3 light_ambient = vec3(0,0,0);
	vec3 light_diffuse = vec3(0,0,0);
	for(int i = 0; i < light_count; i++)
	{
		Light light = lights[i];
		// Ambient light
		float ambient_power = 0.1;
		light_ambient += ambient_power * light.color;

		// Diffuse light
		vec3 light_view_pos = (uView * vec4(light.position.xyz, 1)).xyz;
		vec3 light_view_dir = (uView * vec4(light.direction.xyz, 1)).xyz;

		vec3 fragment_view_pos = texture(tex1, vTexCoord).xyz;
		vec3 fragment_view_norm = texture(tex2, vTexCoord).xyz;

		vec3 D = normalize(light_view_pos - fragment_view_pos);
		float diffuse_power = max(dot(fragment_view_norm, D), 0.0);
		light_diffuse += diffuse_power * light.color;	
	}

	vec3 color = (light_ambient + light_diffuse) * albedo;
	float ssao = use_ssao ? texture(tex3, vTexCoord).r : 1;
	color = ssao * color;
	outColor = vec4(color.rgb, 1);
}