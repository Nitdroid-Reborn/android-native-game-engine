uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

attribute vec4 vPosition;
attribute vec2 vTexCoords;
attribute vec4 vColor;
attribute vec3 vNormal;

varying vec4 pPosition;
varying vec3 pNormal;
varying vec2 pTexCoords;
varying vec4 pColor;


void main() {
	gl_Position = modelViewProjectionMatrix * (vPosition);
	pTexCoords = vTexCoords;
	pColor = vColor;
	pPosition = vPosition;
	pNormal = vec3(modelViewMatrix * vec4(vNormal, 0.0));
}
