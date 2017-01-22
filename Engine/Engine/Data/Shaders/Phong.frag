#version 330 core

in vec4 v2f_color;
in vec2 v2f_texCords;
in vec3 v2f_normal;
in vec3 v2f_fragPos;

layout(location = 0) out vec4 out_color;

struct Material
{	
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	vec4 color;
}; 

#define MAX_DIRECTIONAL_LIGHTS 4
struct DirectionalLight
{
	float intensity;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define MAX_POINT_LIGHTS 32
struct PointLight
{
	float intensity;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

#define MAX_SPOT_LIGHTS 8
struct SpotLight
{
	float intensity;
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float cutoff;
	float outerCutoff;
};

uniform Material material;

uniform int dirLightCount;
uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight dLight[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pLight[MAX_POINT_LIGHTS];
uniform SpotLight sLight[MAX_SPOT_LIGHTS];

uniform vec3 viewerPos;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir);

void main()
{
	vec3 result;
	vec3 norm = normalize(v2f_normal);
	vec3 viewDir = normalize(viewerPos - v2f_fragPos);
	
	for(int i = 0; i < dirLightCount; ++i)
		result += CalculateDirectionalLight(dLight[i], norm, viewDir);

	for(int i = 0; i < pointLightCount; ++i)
		result += CalculatePointLight(pLight[i], norm, viewDir);

	for(int i = 0; i < spotLightCount; ++i)
		result += CalculateSpotLight(sLight[i], norm, viewDir);

	out_color = vec4(result, 1.0f) * material.color;
}


vec3 CalculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir)
{
	vec3 diffuseTex3 = vec3(texture(material.diffuse, v2f_texCords));
	if( diffuseTex3.r == 0 &&
		diffuseTex3.g == 0 &&
		diffuseTex3.b == 0)
	{
		diffuseTex3.r = 1;
		diffuseTex3.g = 1;
		diffuseTex3.b = 1;
	}

	// ambient 
	vec3 ambient = light.ambient * diffuseTex3;

	// diffuse
	vec3 lightDir = normalize(-light.direction);
	float diffuseStrength = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseTex3 * diffuseStrength * light.diffuse;

	// specular 
	vec3 reflectDir = reflect(-lightDir, norm);
	float specStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specStrength * vec3(texture(material.specular, v2f_texCords));

	diffuse *= light.intensity;
	specular *= light.intensity;
	return vec3(ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir)
{
	vec3 diffuseTex3 = vec3(texture(material.diffuse, v2f_texCords));
	if( diffuseTex3.r == 0 &&
		diffuseTex3.g == 0 &&
		diffuseTex3.b == 0)
	{
		diffuseTex3.r = 1;
		diffuseTex3.g = 1;
		diffuseTex3.b = 1;
	}

	// ambient 
	vec3 ambient = light.ambient * diffuseTex3;

	// diffuse
	vec3 lightDir = normalize(light.position - v2f_fragPos);
	float diffuseStrength = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseTex3 * diffuseStrength * light.diffuse;

	// specular 
	vec3 reflectDir = reflect(-lightDir, norm);
	float specStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specStrength * vec3(texture(material.specular, v2f_texCords));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = (light.cutoff - light.outerCutoff);
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	diffuse *= intensity * light.intensity;
	specular *= intensity * light.intensity;
	return vec3(ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDir)
{
	float d = length(light.position - v2f_fragPos);
	float attenuation = light.intensity / (light.constant + light.linear * d + light.quadratic * (d * d));

	vec3 diffuseTex3 = vec3(texture(material.diffuse, v2f_texCords));
	if( diffuseTex3.r == 0 &&
		diffuseTex3.g == 0 &&
		diffuseTex3.b == 0)
	{
		diffuseTex3.r = 1;
		diffuseTex3.g = 1;
		diffuseTex3.b = 1;
	}

	// ambient 
	vec3 ambient = light.ambient * diffuseTex3;

	// diffuse
	vec3 lightDir = normalize(light.position - v2f_fragPos);
	float diffuseStrength = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseTex3 * diffuseStrength * light.diffuse;

	// specular 
	vec3 reflectDir = reflect(-lightDir, norm);
	float specStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specStrength * vec3(texture(material.specular, v2f_texCords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	vec3 result = vec3(ambient + diffuse + specular);
	return result;
}




