
#version 120

uniform sampler2DRect position;
uniform sampler2DRect velocity;

uniform float t;
uniform float dt;

vec4 getPosition() {
	return texture2DRect(position, gl_TexCoord[0].xy);
}

vec4 getVelocity() {
	return texture2DRect(velocity, gl_TexCoord[0].xy);
}

//this function is a force and returns velocity
void main() {
	vec3 p = getPosition().xyz;
	vec3 v = getVelocity().xyz;
	
//	centerline = p;
//	centerline.xz *= 0.0;
//	
//	inplane = p;
//	inplane.y *= 0.0;

//	centerline = normalize(centerline);
//	inplane = normalize(centerline);
//	tangent = cross(centerline, inplane);
	gl_FragColor = vec4(1,0,0,1);
	//v.xyz += tangent;

}