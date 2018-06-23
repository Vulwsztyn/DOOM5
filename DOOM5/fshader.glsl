#version 330
<<<<<<< HEAD

struct Material {
    vec4 ambient;
    sampler2D diffuseMap;
    sampler2D specMap;
	sampler2D normalMap;
	sampler2D heightMap;
    float shininess;
	float roughness;
}; 
struct Light {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	vec4 lightColor;
};

uniform Light light;  
uniform vec4 viewPos; 
uniform Material material;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 FragPos; 
in vec4 Normal; 
in vec2 iTexCoord0; //wspolrzedne teksturowania
in vec4 TangentLightPos;
in vec4 TangentViewPos;
in vec4 TangentFragPos;

vec2 ParallaxMapping(vec2 texCoords, vec4 viewDir)
{ 
    // number of depth layers
    const float numLayers = 200;
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * material.roughness; 
    vec2 deltaTexCoords = P / numLayers;
	vec2  currentTexCoords     = texCoords;
float currentDepthMapValue = texture(material.heightMap, currentTexCoords).r;
  
while(currentLayerDepth < currentDepthMapValue)
{
    // shift texture coordinates along direction of P
    currentTexCoords += deltaTexCoords;
    // get depthmap value at current texture coordinates
    currentDepthMapValue = texture(material.heightMap, currentTexCoords).r;  
    // get depth of next layer
    currentLayerDepth += layerDepth;  
}
//return currentTexCoords;
// get texture coordinates before collision (reverse operations)
vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

// get depth after and before collision for linear interpolation
float afterDepth  = currentDepthMapValue - currentLayerDepth;
float beforeDepth = texture(material.heightMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
// interpolation of texture coordinates
float weight = afterDepth / (afterDepth - beforeDepth);
vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

return finalTexCoords; 
      
} 

void main(void) {
	//ambient
	vec4 ambient  = light.ambient * material.ambient;

	vec4 viewDir    = normalize(TangentViewPos - TangentFragPos);
	vec2 newiTexCoord0=ParallaxMapping(iTexCoord0,  viewDir);

	// diffuse 
	vec4 normal = vec4(texture(material.normalMap, newiTexCoord0).rgb,1);
    normal = normalize(normal * 2.0 - 1.0);
	//normal=normalize(Normal);
	vec4 lightDir   = normalize(TangentLightPos - TangentFragPos);
	float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = light.diffuse * (diff * texture(material.diffuseMap,newiTexCoord0));

	// specular
	viewDir    = normalize(viewPos - FragPos);
	lightDir   = normalize(light.position - FragPos);
	vec4 halfwayDir = normalize(lightDir + viewDir);
	normal=normalize(Normal);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	vec4 specular = light.specular * (spec * texture(material.specMap,newiTexCoord0)); 

	pixelColor=specular+ambient+diffuse;
}
=======
 
out vec4 pixelColor;
in vec4 iColor;
 
void main(void) {
    pixelColor=iColor;
}
>>>>>>> A-Kolizja
