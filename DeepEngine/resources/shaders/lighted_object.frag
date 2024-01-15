#version 330 core
#pragma debug(on)

in vec3 normal;
in vec4 color;
in vec3 fragPos;

out vec4 FragColor;

uniform vec4 deAmbient;     // Lumière d'ambiance.
uniform vec3 deLightPos;    // Position de la source lumineuse.

void main()
{
    vec3 lightDir = normalize(deLightPos - fragPos);    // Récupère le vecteur unitaire (longueur = 1) de la direction entre la source et le fragment.
    vec4 scatteredLight = deAmbient;

    FragColor = min(color * scatteredLight, vec4(1.0));
}
