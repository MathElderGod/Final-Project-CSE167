#version 330 core

in vec4 position; // raw position in the model coord
in vec3 normal;   // raw normal in the model coord
in vec2 coords;
 

uniform mat4 modelview; // from model coord to eye coord
uniform mat4 view;      // from world coord to eye coord
uniform mat4 lightproj;

// Material parameters
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emision;
uniform float shininess;
uniform sampler2D shadowMap;

// Light source parameters
const int maximal_allowed_lights = 10;
uniform bool enablelighting;
uniform int nlights;
uniform vec4 lightpositions[ maximal_allowed_lights ];
uniform vec4 lightcolors[ maximal_allowed_lights ];

// Output the frag color
out vec4 fragColor;


float ShadowCalculation(){
    // vec4 fragPosLightSpace = lightproj * modelview * position;
    vec4 fragPosLightSpace = lightproj * lightView * model * position;

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}  

void main (void){
    if (!enablelighting){
        // Default normal coloring (you don't need to modify anything here)
        vec3 N = normalize(normal);
        fragColor = vec4(0.5f*N + 0.5f , 1.0f);
    } else {

        // extract the matrix A, to convert the normal vector into the view BEWARE COLUMN MAJOR MAT[COL][ROW]
        vec3 column1 = vec3(modelview[0][0], modelview[0][1], modelview[0][2]);
        vec3 column2 = vec3(modelview[1][0], modelview[1][1], modelview[1][2]);
        vec3 column3 = vec3(modelview[2][0], modelview[2][1], modelview[2][2]);
        
        // the matrix A was extracted from the model view matrix
        mat3 matrixA = mat3(column1, column2, column3);

        // the matrix A is inverted and transposed
        mat3 matrixAInverseTransposed = inverse(transpose(matrixA));

        // the normal vector should now be in the view space
        vec3 newNormal = normalize(matrixAInverseTransposed * normalize(normal));

        // the normal vector is now in vec4, and is in the view space
        vec3 n = normalize(vec3(newNormal.x, newNormal.y, newNormal.z));

        // transfer the position to the view 
        vec4 p = modelview * position;

        // get the vector to the veiwer
        vec3 v = normalize( vec3(0.0f, 0.0f, 0.0f) - p.xyz);

        // keep track of the ongoing sum for all colors
        vec3 sumOfColors = vec3(0.0f, 0.0f, 0.0f);
        
        float shadow = ShadowCalculation();

        for(int j = 0; j < nlights; j++){
             
             // get the light positions in the view 
             vec4 q = view * lightpositions[j];

             // the unit direction d pointing from a point position p to a light position q
             vec3 l = normalize(p.w * q.xyz - q.w * p.xyz);

             // get the current light direction vector l
             // vec4 l = normalize(vec4(d.x, d.y, d.z, 1.0f));

             // get the half way vector 
             vec3 h = normalize(v.xyz + l.xyz);

             sumOfColors += ( ambient.xyz + (1.0f - shadow) * (diffuse.xyz * max( dot(n, l), 0.0f) + specular.xyz * pow( max( dot(n, h), 0.0f), shininess) )) * lightcolors[j].xyz;
        }
        // get the final reflected color R
        vec3 R = emision.xyz + sumOfColors;


        // output the final reflected color R
        fragColor = vec4(R.x, R.y, R.z, 1.0f);

        // float depthSampled = texture(shadowMap, coords);
        //fragColor = vec4(vec3(texture(shadowMap, coords)), 1.0f); 



        //fragColor = vec4(vec3(shadow), 1.0f);


    }
}