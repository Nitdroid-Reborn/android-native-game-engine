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

	float diffuseLight = max(dot(L, normal), 0.1);
	vec3 diffuse = materialDiffuse*diffuseLight;

		
	vec4 tex = texture2D(textureSampler, pTexCoords).rgba;

	gl_FragColor.xyz = (diffuse)*tex.rgb;

	gl_FragColor.w = tex.a;
}
