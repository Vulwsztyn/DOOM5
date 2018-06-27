#version 330

struct Material {
    vec4 ambient;
    sampler2D diffuseMap;
    sampler2D specMap;
	sampler2D normalMap;
	sampler2D heightMap;
    float shininess;
	float roughness;
	float gamma;
}; 
struct Light {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	vec4 lightColor;
};

uniform Light light[3];  
uniform vec4 viewPos; 
uniform Material material;
uniform int numberOfLights;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 FragPos; 
in vec4 Normal; 
in vec2 iTexCoord0; //wspolrzedne teksturowania
in vec4 TangentLightPos[3];
in vec4 TangentViewPos;
in vec4 TangentFragPos;

vec2 ParallaxMapping(vec2 texCoords, vec4 viewDir)
{ 
    // number of depth layers
    const float numLayers = 100;
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
//vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

// get depth after and before collision for linear interpolation
//float afterDepth  = currentDepthMapValue - currentLayerDepth;
//float beforeDepth = texture(material.heightMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
// interpolation of texture coordinates
//float weight = afterDepth / (afterDepth - beforeDepth);
//vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

return currentTexCoords; 
      
} 

vec4 calcLight(vec4 viewDir,vec2 newiTexCoord0,vec4 normal,Light light,vec4 TangentLightPos){
//ambient
	vec4 ambient  = light.ambient * material.ambient;

	// diffuse 
	vec4 lightDir   = normalize(TangentLightPos - TangentFragPos);
	float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = light.diffuse * (diff * texture(material.diffuseMap,newiTexCoord0));

	// specular
	viewDir    = normalize(viewPos - FragPos);
	lightDir   = normalize(light.position - FragPos);
	normal=normalize(Normal);
	vec4 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	vec4 specular = light.specular * (spec * texture(material.specMap,newiTexCoord0)); 
	return material.gamma*(specular+ambient+diffuse);
	}

void main(void) {
	vec4 viewDir    = normalize(TangentViewPos - TangentFragPos);
	vec2 newiTexCoord0=ParallaxMapping(iTexCoord0,  viewDir);
	vec4 normal = vec4(texture(material.normalMap, newiTexCoord0).rgb,1);
    normal = normalize(normal * 2.0 - 1.0);

	
	vec4 result=vec4(0,0,0,0);
	for(int i =0;i<numberOfLights;i++){
		if(distance(viewPos,light[i].position)<50)result+=(min(20/(1.8*distance(viewPos,light[i].position)),1))*calcLight(viewDir,newiTexCoord0,normal,light[i],TangentLightPos[i]);
	}
	pixelColor=result;
}
