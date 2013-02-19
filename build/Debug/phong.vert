varying vec3 normal, eyeVec;
#define MAX_LIGHTS 8
#define NUM_LIGHTS 3
varying vec3 lightDir;

uniform float temps;

void main()
{	
    normal = gl_NormalMatrix * gl_Normal;

    vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

    lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
    eyeVec = -vVertex;	
    gl_TexCoord[0]  = gl_MultiTexCoord0;
    gl_TexCoord[0].s += temps;
    gl_Position = ftransform();

}
