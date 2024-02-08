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

# Taille des textures

La taille des textures dépend de l'implémentation d'OpenGL. Pour les textures `1D` et `2D` la taille max est spécifiée par `GL_MAX_TEXTURE_SIZE`.
Pour les textures 3D, aucune dimension ne peut être supérieure à `GL_MAX_3D_TEXTURE_SIZE`.
Pour les textures dans des tableaux, par exemple `GL_TEXTURE_2D_ARRAY`, la taille max du tableau est spécifiée par `GL_MAX_ARRAY_TEXTURE_LAYERS`.

Parmi ces limites, n'importe quelle valeur peut être utilisée, mais il est recommandé que la taille soit une puissance de **2**.

Dans un shader, il est possible de récupérer la taille de la texture grâce à la fonction :
```glsl
ivec textureSize(gsampler sampler​, int lod​);
```

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

# Format d'image

Décrit la façon dont une **texture** ou un **renderbuffer** stock ses données.

Il y a 3 types de formats d'image :
- **Couleur**.
- **Profondeur**.
- **Profondeur / stylo**.

## Couleur

Les couleurs sont stockées en **RGBA**.

La valeur **Alpha** n'a pas de définition intrinsèque car c'est le shader qui va déterminer l'utilisation du channel. Et en réalité, même les channels **RGB** n'ont pas de définition intrinsèque, tout dépend de l'utilisation de ces valeurs dans le shader.

Les formats de couleur peuvent être stockées de 3 façons différentes :
- **Entiers normalisés**.
- **Points flottants**.
- **Entiers**.
Que ce soit des **entiers normalisés** ou des **points flottants**, dans le shader elles seront considérées comme des vecteurs de valeurs flottantes, contrairement aux **entiers** qui seront considérés comme des vecteurs d'entiers.

Les **entiers normalisés** ont 2 types :
- **Normalisé non-signé** : [0, 1]
- **Normalisé signé** : [-1, 1]
Les **entiers** ont aussi 2 types :
- **Non-signé**.
- **Signé** : utilise le complément à 2.

Quand un shader échantillonne une texture, il la résout toujours en un vecteur **RGBA** à 4 valeurs.
Ce qui implique que les composants non stockés par le format d'image sont remplis automatiquement :
- Si un **R**, **G** ou **B** est manquant, il est rempli par un 0.
- Si un **A** est manquant, il est rempli par un 1.

Voici la syntax qu'OpenGL utilise pour écrire les formats de couleur : `GL_[components][size][type]`
Le champ `components` est la liste des composants que le format stock :
- **R**.
- **RG**.
- **RGB**.
- **RGBA**.
Aucune autre combinaison n'est autorisée.
Le champ `size` indique la profondeur de bits de chaque composant.
Le champ `type` indique de quelle façon sont stockées les valeurs, décrite plus haut :
- **""** : pas de suffix, **entiers normalisés non-signés**.
- **"\_SNORM"** : **entiers normalisés signés**.
- **"F"** : **points flottants**, par exemple `GL_RGBA32F` est un format de **points flottants** où chaque composant est une valeur flottante IEEE de 32 bits.
- **"I"** : **entiers signés**, par exemple `GL_RGBA8I` est un format d'**entiers signés** où chaque composant est un entier dans la plage [-128, 127].
- **"UI"** : **entiers non signés**, chaque composant est un entier dans la plage [0, `MAX_INT`].
 
Pour chaque type de format de couleur, il y a une limite sur la profondeur de bits disponible  par composant :

| **Type de format** | **Profondeur de bits par composant** |
| ---- | ---- |
| **Normalisé non-signé** | 2 *(1)*, 4 *(2)*, 5 *(3)*, 8, 10 *(3)*, 12 *(2)*, 16 |
| **Normalisé signé** | 8, 16 |
| **Entier non-signé** | 8, 16, 32 |
| **Entier signé** | 8, 16, 32 |
| **Point flottant** | 16, 32 |
*(1)* : Ces profondeurs de bits sont utilisables uniquement par `RGBA` seulement. Par exemple impossible d'utiliser `GL_RG2`.
*(2)* : Ces profondeurs de bits sont utilisables uniquement par `RGB` et `RGBA` seulement. Par exemple impossible d'utiliser `GL_R4`.
*(3)* : Ces profondeurs de bits ne sont utilisables uniquement par `RGB` uniquement. Par exemple impossible d'utiliser `GL_RGBA10`.

## Profondeur

Ce format d'image ne stock que les informations de **profondeur**.

Il y a 2 types de formats de profondeur :
- **Entier normalisé** : comme pour le format de couleur, possède une valeur flottante entre [0, 1].
- **Point flottant**.
La liste des formats de profondeur disponibles est la suivante :
- `GL_DEPTH_COMPONENT16`
- `GL_DEPTH_COMPONENT24`
- `GL_DEPTH_COMPONENT32`
- `GL_DEPTH_COMPONENT32F`

## Profondeur / stylo

Ce d'image cumule le **format de profondeur** ainsi que celui du **stylo**.

Il y a 2 types de formats de profondeur / stylo :
- **Entier normalisé** : comme pour le format de couleur, possède une valeur flottante entre [0, 1].
- **Point flottant**.
La liste des formats de profondeur / stylo disponibles est la suivante :
- `GL_DEPTH24_STENCIL8`
- `GL_DEPTH32F_STENCIL8`

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
Lorsqu'ils sont passés en paramètre de fonction, il faut ajouter le mot clé `in` :
```glsl
uniform sampler2D myTex;

void Function(in sampler2D tex);
```

Le type du **sampler** correspond au type de texture utilisable par celui-ci.

## Types de sampler

Il y a 3 catégories, basées sur le type de données du [[Textures#Format d'image]] :
- **Point flottant** : regroupe aussi les **entiers normalisés**.
- **Entier signé**.
- **Entier non-signé**.

Voici la liste des samplers disponibles :

| **GLSL Sampler** | **OpenGL texture enum** | **Texture type** |
| ---- | ---- | ---- |
| *g*sampler1D | `GL_TEXTURE_1D` | 1D texture |
| *g*sampler2D | `GL_TEXTURE_2D` | 2D texture |
| *g*sampler3D | `GL_TEXTURE_3D` | 3D texture |
| *g*samplerCube | `GL_TEXTURE_CUBE_MAP` | Cubemap Texture |
| *g*sampler2DRect | `GL_TEXTURE_RECTANGLE` | Rectangle Texture |
| *g*sampler1DArray | `GL_TEXTURE_1D_ARRAY` | 1D Array Texture |
| *g*sampler2DArray | `GL_TEXTURE_2D_ARRAY` | 2D Array Texture |
| *g*samplerCubeArray | `GL_TEXTURE_CUBE_MAP_ARRAY` | Cubemap Array Texture *(OpenGL 4.0)* |
| *g*samplerBuffer | `GL_TEXTURE_BUFFER` | Buffer Texture |
| *g*sampler2DMS | `GL_TEXTURE_2D_MULTISAMPLE` | Multisample Texture |
| *g*sampler2DMSArray | `GL_TEXTURE_2D_MULTISAMPLE_ARRAY` | Multisample Array Texture |

## Coordonnées de texture

Les coordonnées de la texture peuvent être normalisées ou dans l'espace *texel*.
Une coordonnée **normalisée** signifie que la taille de la texture correspond à l'intervalle [0, 1]. Ce qui permet aux coordonnées de la texture d'être indépendantes de n'importe quelle taille de la texture.
Une coordonnée dans l'espace *texel* signifie que les coordonnées sont dans l'intervalle [0, taille], où `taille` correspond à la taille de la texture dans cette dimension.

Les textures rectangles, de type `GL_TEXTURE_RECTANGLE` ne peuvent être que dans l'espace *texel*.
Autrement, à part si spécifié explicitement, tous les autres types seront par défaut normalisés.

## Accès à la texture

Pour échantillonner la texture avec des coordonnées normalisées, la fonction suivante doit être utilisée :
```glsl
gvec texture(gsampler sampler​, vec texCoord​[, float bias​]);
```

Cette fonction échantillonne la texture représentée par le `sampler`, à la position `texCoord`.
La taille du vecteur `texCoord` dépend du type de la dimension du `sampler`, par exemple, un sampler 2D nécessite l'utilisation d'un vecteur 2D.

