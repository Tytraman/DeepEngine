#version 330 core
#pragma debug(on)

struct material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform material deMaterial;
uniform light deLight;

in vec3 normal;
in vec4 color;              // Couleur de l'objet passé par le vertex shader.
in vec3 fragPos;            // Position du fragment relatif au monde.

out vec4 FragColor;

uniform vec3 deViewPos;     // Position de la caméra.

void main()
{
    vec3 lightDir = normalize(deLight.position - fragPos);    // Récupère le vecteur unitaire (longueur = 1) de la direction entre la source et le fragment.
    
    float diff = max(0.0, dot(normal, lightDir));       // Calcule la luminance selon l'angle entre la direction de la lumière vers la normale et la direction de la normale.
    
    vec3 viewDir = normalize(deViewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(0.0, dot(viewDir, reflectDir)), deMaterial.shininess);

    vec3 ambient = deLight.ambient * deMaterial.ambient;
    vec3 diffuse = deLight.diffuse * (diff * deMaterial.diffuse);      // Intensité de la couleur de diffusion.
    vec3 specular = deLight.specular * (spec * deMaterial.specular);

    // Lorsqu'aucune diffusion n'est appliquée, la lumière d'ambiance est tout de même visible,
    // donc pour garder cette effet de luminance il faut ajouter à la lumière d'ambiance la lumière de diffusion.
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, color.a);
}
