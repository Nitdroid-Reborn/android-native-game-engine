precision mediump float;
uniform sampler2D textureSampler;

varying vec4 pPosition;
varying vec3 pNormal;
varying vec2 pTexCoords;
varying vec4 pColor;

uniform vec3 lightPosition;
uniform vec3 materialDiffuse;
uniform vec3 materialAmbient;
uniform vec3 materialSpecular;
uniform float materialShininess;

void main() {
	vec3 P = pPosition.xyz;
	vec3 lp = lightPosition;
	vec3 L = normalize(lp - P);
	vec3 normal = normalize(pNormal);

	float diffuseLight = max(dot(L, normal), 0.0);
	vec3 diffuse = materialDiffuse*diffuseLight;

	vec3 V = normalize(-P);
  	vec3 H = normalize(L + V);
    float specularLight = pow(max(dot(H, normal), 0.0), materialShininess);
    if(diffuseLight <= 0) specularLight = 0.0;
    vec3 specular = materialSpecular * specularLight;

	
	gl_FragColor.xyz = (diffuse+specular)*texture2D(textureSampler, pTexCoords).rgb;
	gl_FragColor.w = 1.0;
}
