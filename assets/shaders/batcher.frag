precision mediump float;
uniform sampler2D textureSampler;
varying vec4 texCoords;
varying vec4 color;
uniform int useTextures;


void main() {
	vec4 text = vec4(1,1,1,1);
	if(useTextures==1)
		text = texture2D(textureSampler, texCoords.xy);

	gl_FragColor = text*color;
}
