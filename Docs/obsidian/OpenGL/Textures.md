Les textures peuvent être utilisées pour 2 raisons :
- Utiliser une ou des images dans un shader.
- Être utilisée comme cible de rendu.

Le terme *image* est définie comme étant un **tableau de pixels** d'une certaine dimension (1D, 2D ou 3D), avec une taille et un format spécifique.
Une texture est un conteneur d'**une** ou **plusieurs** images. Mais elle ne stockent pas d'images arbitraires : elles ont des contraintes sur les images qu'elles peuvent stocker :
- Le type de texture définie comment les images sont arrangées dans la texture.
- La taille de la texture définie la taille de l'image.
- Le format définie le format que **toutes** les images de la texture utilisent.

Ce qui fait que les textures sont quelque chose de concret, et non pas qu'un 'concept'.

Quand il est dit qu'une texture peut avoir **plusieurs** images, cela fait référence au *mipmapping*, c'est-à-dire le fait que l'image y soit plusieurs fois dans des tailles différentes.

Le terme souvent utilisé pour décrire un pixel d'une texture est *texel*.

# Types de textures

| Nom | Description |
| ---- | ---- |
| **GL_TEXTURE_1D** | Les images sont en 1D. Elles ont une largeur mais pas de hauteur ni de profondeur. |
| **GL_TEXTURE_2D** | Les images sont en 2D. Elles ont une largeur et une hauteur mais pas de profondeur. |
| **GL_TEXTURE_3D** | Les images sont en 3D. Elles ont une largeur, une hauteur et une profondeur. |
| **GL_TEXTURE_RECTANGLE** | L'image *(il ne peut pas y en avoir plusieurs)* est en 2D. Les coordonnées de la texture ne sont pas normalisées. |
| **GL_TEXTURE_BUFFER** | L'image *(il ne peut pas y en avoir plusieurs)* est en 1D. Le stockage pour ses données provient d'un **Buffer Object**. |
| **GL_TEXTURE_CUBE_MAP** | Il y a exactement 6 sets distincts d'images 2D. Chaque image faisant la même taille et devant former un carré. Ces images forment chaque face d'un cube. |
| **GL_TEXTURE_1D_ARRAY** | Les images sont en 1D. Peut contenir plusieurs sets d'images 1D dans la même texture. La taille du tableau fait partie de la taille de la texture. |
| **GL_TEXTURE_2D_ARRAY** | Les images sont en 2D. Peut contenir plusieurs sets d'images 2D dans la même texture. La taille du tableau fait partie de la taille de la texture. |
| **GL_TEXTURE_CUBE_MAP_ARRAY** | Les images sont toutes des cubemaps. Peut contenir plusieurs sets de cubemaps dans la même texture. La taille du tableau `*` 6 fait partie de la taille de la texture. |
| **GL_TEXTURE_2D_MULTISAMPLE** | L'image *(il ne peut pas y en avoir plusieurs)* est en 2D. Chaque pixel de l'image contient plusieurs échantillons au lieu d'une seule valeur. |
| **GL_TEXTURE_2D_MULTISAMPLE_ARRAY** | Combine `GL_TEXTURE_2D_ARRAY` et `GL_TEXTURE_2D_MULTISAMPLE`. *(Il ne peut pas y avoir plusieurs images).* |

# Taille des textures

La taille des textures dépend de l'implémentation d'OpenGL. Pour les textures `1D` et `2D` la taille max est spécifiée par `GL_MAX_TEXTURE_SIZE`.
Pour les textures 3D, aucune dimension ne peut être supérieure à `GL_MAX_3D_TEXTURE_SIZE`.
Pour les textures dans des tableaux, par exemple `GL_TEXTURE_2D_ARRAY`, la taille max du tableau est spécifiée par `GL_MAX_ARRAY_TEXTURE_LAYERS`.

Parmi ces limites, n'importe quelle valeur peut être utilisée, mais il est recommandé que la taille soit une puissance de **2**.

# Texture Object

![[OpenGL_Anatomy_of_a_Texture.png]]

Les textures dans OpenGL sont des [[Objects]], ce qui inclut qu'elles suivent les conventions standards de ceux-ci.
Elles ont donc les fonctions `glGenTextures`, `glBindTexture` et toutes les autres de ces conventions.

Il n'est pas légal d'attacher une texture *(avec `glBindTexture`)* et d'indiquer un type différent de celui défini lors de la création de la texture.

# Échantillonnage

L'échantillonnage est le processus qui récupère une valeur d'une texture à une position donnée.

La procédure d'échantillonnage se passe dans le shader au travers des **samplers**.
Il existe plusieurs types de samplers selon le type de texture utilisé.

Les samplers ne peuvent être définies qu'à travers les uniformes, il n'est pas possible de changer la valeur directement dans le shader.
Lorsqu'ils sont passés en paramètre de fonction, il faut ajouter le mot clé `in`.

