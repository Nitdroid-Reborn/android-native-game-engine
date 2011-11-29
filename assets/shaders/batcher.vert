uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
attribute vec4 vPosition;
attribute vec4 vTexCoords;
attribute vec4 vColor;
attribute vec3 vNormal;

varying vec4 texCoords;
varying vec4 color;

void main() {
	gl_Position = modelViewProjectionMatrix * (vPosition);
	texCoords = vTexCoords;
	color = vColor;
}
