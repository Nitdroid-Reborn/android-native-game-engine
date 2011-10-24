precision mediump float;
uniform sampler2D textureSampler;
varying vec4 texCoords;
varying vec4 color;

void main() {
	gl_FragColor = texture2D(textureSampler, texCoords.xy)*color;
}
