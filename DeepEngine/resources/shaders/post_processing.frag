#version 330 core
#pragma debug(on)

out vec4 FragColor;

in vec2 texCoord;

// Échantillonneur de texture 2D correspondant au rendu.
uniform sampler2D screenTexture;

// Ce shader permet de donner un ou plusieurs effets sur la globalité du rendu.
// Par exemple pour rendre le rendu en niveau de gris, le rendre en noir et blanc en seuillant les pixels.
void main()
{
    // Prend la valeur moyenne du pixel pour l'avoir en niveau de gris.
    FragColor = texture(screenTexture, texCoord);
//    float average = (FragColor.r + FragColor.g + FragColor.b) / 3;
//
//    // Fait un seuillage binaire.
//    if(average <= 0.35)
//        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//    else
//        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
//
//    // Inverse les couleurs.
//    FragColor.r = 1 - FragColor.r;
//    FragColor.g = 1 - FragColor.g;
//    FragColor.b = 1 - FragColor.b;
}