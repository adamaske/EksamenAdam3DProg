#version 410 core
out vec4 fragColor;         //The final color

in vec3 normalTransposed;   //Normal of fragment, already transposed
in vec3 fragmentPosition;
in vec2 UV;       //for textures

uniform sampler2D textureSampler;

uniform vec3 cameraPosition;

uniform float ambientStrength = 0.2;

uniform vec3 lightPosition;
uniform vec3 lightColor = vec3(1, 1, 1);
uniform float lightStrength = 1;
uniform float specularStrength = 0.1;
uniform int specularExponent = 32;

uniform vec3 objectColor = vec3(1.0, 1.0, 0.0); //white

void main() {

    //ambient
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    float angleFactor = max(dot(normalCorrected, lightDirection), 0.1);
    vec3 diffuse = angleFactor * objectColor * lightColor * lightStrength;

    //specular
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    float spec = 0.0;
    if (angleFactor > 0.0)     //calculations only needed if diffuse is above 0.0
    {
        vec3 reflectDirection = reflect(-lightDirection, normalCorrected);
        spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
    }
    vec3 specular = spec * lightColor * specularStrength;

    vec3 result = ambient + diffuse + specular * objectColor;

    fragColor = vec4(result, 1.0) * texture(textureSampler, UV);

}


