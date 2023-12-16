# Format d'image
id:: 656f5954-5393-4868-9c24-2e1b4c005d8b
	- Décrit la façon dont une **texture** ou un **renderbuffer** stock ses données.
	- Il y a 3 types de formats d'image.
		- **Couleur**.
		- **Profondeur**.
		- **Profondeur / stylo**.
	- Ces 3 formats peuvent êtres utilisés avec les **textures** ou les **renderbuffers** et peuvent êtres **multisamplés**.
	- ## Couleur
		- Les couleurs sont stockées en **RGBA**.
		- La valeur **Alpha** n'a pas de définition intrinsèque car c'est le shader qui va déterminer l'utilisation du channel.
			- En réalité, même les channels **RGB** n'ont pas de définition intrinsèque, tout dépend de l'utilisation de ces valeurs.
		- Les formats de couleur peuvent être stockées de 3 façons différentes :
			- **Entiers normalisés**.
			- **Points flottants**.
			- **Entiers**.
		- Que ce soit des **entiers normalisés** ou des **points flottants**, dans le shader elles seront considérées comme des vecteurs de valeurs flottantes, contrairement aux **entiers** qui seront considérés comme des vecteurs d'entiers.
		- Les **entiers normalisés** ont 2 types :
			- **Normalisé non-signé** : [0, 1]
			- **Normalisé signé** : [-1, 1]
		- Les **entiers** ont aussi 2 types :
			- **Non-signé**.
			- **Signé** : utilise le complément à 2.
		- Quand un shader échantillonne une texture, il la résout toujours en un vecteur **RGBA** à 4 valeurs.
			- Les composants non stockés par le format d'image sont remplis automatiquement.
			- Si un **R**, **G** ou **B** est manquant, il est rempli par un 0.
			- Si un **A** est manquant, il est rempli par un 1.
		- Voici la syntax qu'OpenGL utilise pour écrire les formats de couleur.
			- `GL_[components][size][type]`
			- Le champ `components` est la liste des composants que le format stock :
				- **R**.
				- **RG**.
				- **RGB**.
				- **RGBA**.
				- Aucune autre combinaison n'est autorisée.
			- Le champ `size` indique la profondeur de bits de chaque composant.
			- Le champ `type` indique de quelle façon sont stockées les valeurs, décrite plus haut.
				- **""** : pas de suffix, **entiers normalisés non-signés**.
				- **"_SNORM"** : **entiers normalisés signés**.
				- **"F"** : **points flottants**, par exemple `GL_RGBA32F` est un format de **points flottants** où chaque composant est une valeur flottante IEEE de 32 bits.
				- **"I"** : **entiers signés**, par exemple `GL_RGBA8I` est un format d'**entiers signés** où chaque composant est un entier dans la plage [-128, 127].
				- **"UI"** : **entiers non signés**, chaque composant est un entier dans la plage [0, `MAX_INT`].
			- Pour chaque type de format de couleur, il y a une limite sur la profondeur de bits disponible  par composant :
				- | **Type de format** | **Profondeur de bits par composant** |
				  | Normalisé non-signé | 2 *(1)*, 4 *(2)*, 5 *(3)*, 8, 10 *(3)*, 12 *(2), 16 |
				  | Normalisé signé | 8, 16 |
				  | Entier non-signé | 8, 16, 32 |
				  | Entier signé | 8, 16, 32 |
				  | Point flottant | 16, 32 |
					- *(1)* : Ces profondeurs de bits sont utilisables uniquement par `RGBA` seulement. Par exemple impossible d'utiliser `GL_RG2`.
					- *(2)* : Ces profondeurs de bits sont utilisables uniquement par `RGB` et `RGBA` seulement. Par exemple impossible d'utiliser `GL_R4`.
					- *(3)* : Ces profondeurs de bits ne sont utilisables uniquement par `RGB` uniquement. Par exemple impossible d'utiliser `GL_RGBA10`.
			-
			-
			-
	- ## Profondeur
		- Ce format d'image ne stock que les informations de **profondeur**.
		- Il y a 2 types de formats de profondeur :
			- **Entier normalisé** : comme pour le format de couleur, possède une valeur flottante entre [0, 1].
			- **Point flottant**.
		- La liste des formats de profondeur disponibles est la suivante :
			- `GL_DEPTH_COMPONENT16`
			- `GL_DEPTH_COMPONENT24`
			- `GL_DEPTH_COMPONENT32`
			- `GL_DEPTH_COMPONENT32F`
	- ## Profondeur / stylo
		- Ce d'image cumule le **format de profondeur** ainsi que celui du **stylo**.
		- Il y a 2 types de formats de profondeur / stylo :
			- **Entier normalisé** : comme pour le format de couleur, possède une valeur flottante entre [0, 1].
			- **Point flottant**.
		- La liste des formats de profondeur / stylo disponibles est la suivante :
			- `GL_DEPTH24_STENCIL8`
			- `GL_DEPTH32F_STENCIL8`
- # GLSL
	- ## Sampler
		- Les **samplers** doivent êtres des *uniforms* ou des paramètres de fonctions.
			- Ils ne peuvent pas avoir de valeur assigné, donc doit être passé en paramètre de fonction avec le mot-clé `in`.
			- / 
			  ```glsl
			  uniform sampler2D myTex;
			  
			  void Function(in sampler2D tex);
			  ```
		- Chaque **sampler** représente une seule texture d'un type particulier.
		- Le type du **sampler** correspond au type de texture utilisable par celui-ci.
	- ## Types de sampler
		- Il y a 3 catégories, basées sur le type de données du [format d'image](((656f5954-5393-4868-9c24-2e1b4c005d8b))) :
			- **Point flottant** : regroupe aussi les **entiers normalisés**.
			- **Entier signé**.
			- **Entier non-signé**.
		- Voici la liste des samplers disponibles :
			- | **GLSL Sampler** | **OpenGL texture enum** | **Texture type** |
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
			-