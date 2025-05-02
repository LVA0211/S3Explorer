#version 330 core

#define TWO_PI 2.f * 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068f
#define DELTA 0.0000000001f
#define DELTA_SQUARED DELTA*DELTA

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 model;
uniform vec4 scale;
uniform mat4 view;
uniform mat4 projection;

uniform bool is_back_hemisphere;
uniform float normal_falloff = 0.0f;
uniform float light_falloff;
uniform bool project_normals_to_sphere = true;

out vec2 TexCoord;
out float light;

const vec4 viewCam = vec4(0,0,0,1);

void main() {   
    
    vec4 ontoSpherePos = normalize(scale * aPos);

    vec4 localPos = aPos;
    vec4 localNormal = aNormal;
    vec4 viewPos = normalize(view * model * ontoSpherePos);

    float dist = acos(dot(viewCam,viewPos));
    if(is_back_hemisphere){
        dist = TWO_PI - dist;
    };

    vec4 ontoSphereNormal;

    if(dot(localNormal,localNormal) < DELTA_SQUARED)
    {
        light = 1.f;
    } else {

        if(project_normals_to_sphere){
        //initialise in case of scale.w == 0
        ontoSphereNormal = vec4(0,0,0,-1);
        //project the normal so that orthogonality is preserved. dot(newNormal,newPos)=0 and dot(newNormal, d(newPos)/dt)=0.
        //x,y,z scaling factors cancel out in the w-coord. The last coordinate is divided by scale.w
        if(abs(scale.w)>DELTA){
        ontoSphereNormal = normalize(vec4(  localNormal.x/scale.x,
                                            localNormal.y/scale.y,
                                            localNormal.z/scale.z,
                                            (-localNormal.x*localPos.x - localNormal.y*localPos.y - localNormal.z*localPos.z)/scale.w
                                    ));
        };
        } else {
            ontoSphereNormal =  normalize(aNormal);
        };

        vec4 viewNormal = view*model*ontoSphereNormal;
        vec4 lightDir = normalize(viewCam - dot(viewCam, viewPos) * viewPos);

        light = (1.f + normal_falloff*(abs(dot(lightDir, viewNormal))-1.f));
    }

    light /= (1.f + dist*light_falloff + dist*dist*light_falloff);

    gl_Position = projection * viewPos;
    TexCoord = aUV;
}