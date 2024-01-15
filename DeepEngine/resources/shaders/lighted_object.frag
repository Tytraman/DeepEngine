#version 330 core
#pragma debug(on)

in vec3 normal;
in vec4 color;              // Couleur de l'objet passé par le vertex shader.
in vec3 fragPos;            // Position du fragment relatif au monde.

out vec4 FragColor;

uniform vec4 deAmbient;     // Lumière d'ambiance.
uniform vec3 deLightPos;    // Position de la source lumineuse.
uniform vec4 deLightColor;  // Couleur de la source lumineuse.

void main()
{
    vec3 lightDir = normalize(deLightPos - fragPos);    // Récupère le vecteur unitaire (longueur = 1) de la direction entre la source et le fragment.
    
    float diff = max(0.0, dot(normal, lightDir));       // Calcule la luminance selon l'angle entre la direction de la lumière vers la normale et la direction de la normale.
    vec4 diffuse = diff * deLightColor;                 // Intensité de la couleur de diffusion.

    // Lorsqu'aucune diffusion n'est appliquée, la lumière d'ambiance est tout de même visible,
    // donc pour garder cette effet de luminance il faut ajouter à la lumière d'ambiance la lumière de diffusion.
    vec4 result = (deAmbient + diffuse) * color;

    FragColor = result;
}
