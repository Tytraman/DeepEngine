# glGenBuffers
id:: 6574b058-8708-4eb1-989d-3a63ac022467
	- ```c
	  void glGenBuffers(GLsizei n,
	                    GLuint *buffers);
	  ```
	- Génère `n` buffers identifiés par des entiers stockés dans `buffers`.
	- ## Paramètres
		- `n` : indique le nombre de buffers à créer.
		- `buffers` : tableau dans lequel les identifiants des buffers générés seront stockés.
	- ## Erreurs
		- `GL_INVALID_VALUE` si `n` est négatif.
	- ## Versions OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
- # glDeleteBuffers
	- ```c
	  void glDeleteBuffers(GLsizei n,
	                       const GLuint *buffers);
	  ```
	- Supprime `n` buffers précédemment créés avec ((6574b058-8708-4eb1-989d-3a63ac022467)).
	- ## Paramètres
		- `n` : indique le nombre de buffers à supprimer.
		- `buffers` : tableau d'identifiants de buffers à supprimer.
	- ## Erreurs
		- `GL_INVALID_VALUE` si `n` est négatif.
	- ## Versions OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
- # glBindBuffer
  id:: 6574b2e0-a34a-4fb4-aec7-592660e21366
	- ```c
	  void glBindBuffer(GLenum target,
	                    GLuint buffer);
	  ```
	- Attache le `buffer` à la cible spécifiée.
	- ## Paramètres
		- `target` : cible à laquelle attacher le buffer. Peut être une des valeurs suivantes :
			- | Cible | Objectif |
			  | `GL_ARRAY_BUFFER` | Vertex attributes |
			  | `GL_ATOMIC_COUNTER_BUFFER` | Atomic counter storage |
			  | `GL_COPY_READ_BUFFER` | Buffer copy source |
			  | `GL_COPY_WRITE_BUFFER` |  	Buffer copy destination |
			  | `GL_DISPATCH_INDIRECT_BUFFER` | Indirect compute dispatch commands |
			  | `GL_DRAW_INDIRECT_BUFFER` | Indirect command arguments |
			  | `GL_ELEMENT_ARRAY_BUFFER` | Vertex array indices |
			  | `GL_PIXEL_PACK_BUFFER` | Pixel read target |
			  | `GL_PIXEL_UNPACK_BUFFER` | Texture data source |
			  | `GL_QUERY_BUFFER` | Query result buffer |
			  | `GL_SHADER_STORAGE_BUFFER` | Read-write storage for shaders |
			  | `GL_TEXTURE_BUFFER` | Texture data buffer |
			  | `GL_TRANSFORM_FEEDBACK_BUFFER` | Transform feedback buffer |
			  | `GL_UNIFORM_BUFFER` | Uniform block storage |
		- `buffer` : l'identifiant de buffer à attacher.
	- ## Erreurs
		- `GL_INVALID_ENUM` si `target` n'est pas une des valeurs de la liste.
		- `GL_INVALID_VALUE` si `buffer` n'est pas un identifiant généré précédemment par ((6574b058-8708-4eb1-989d-3a63ac022467)).
	- ## Versions OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
- # glBufferStorage
  id:: 6574b5f6-75ab-446f-a4d9-fb8c19f0e0fd
	- ```c
	  void glBufferStorage(GLenum target,
	                       GLsizeiptr size,
	                       const void *data,
	                       GLbitfield flags);
	  ```
	- Alloue et initialise un stockage de données immuables, c'est-à-dire que le stockage ne pourra pas être réalloué plus tard.
	- ## Paramètres
		- `target` : voir ((6574b2e0-a34a-4fb4-aec7-592660e21366)).
		- `size` : spécifie la taille en **octets** à allouer pour le buffer.
		- `data` : pointeur vers des données qui seront **copiées** vers le buffer, ou `nullptr` si aucune donnée ne doit être copié.
		- `flags` : Spécifie l'usage de ce stockage immuable, doit être une combinaison bitwise de ces valeurs :
			- `GL_DYNAMIC_STORAGE_BIT` : active la modification des données après la création du stockage grâce à l'appel `glBufferSubData`. Si le bit n'est pas mis, il ne sera pas possible de modifier le contenu du stockage depuis le client, mais il sera toujours possible de le modifier depuis le serveur grâce aux fonctions `glCopyBufferSubData` ou `glClearBufferSubData`.
				- TODO Mettre les références des fonctions.
			- `GL_MAP_READ_BIT` : permet au client de mapper le stockage et obtenir un pointeur vers le stockage pour y lire les données.
			- `GL_MAP_WRITE_BIT` : permet au client de mapper le stockage et obtenir un pointeur vers le stockage pour y écrire des données.
			- `GL_MAP_PERSISTENT_BIT`
				- TODO Comprendre ce que fait ce flag.
			- `GL_MAP_COHERENT_BIT`
				- TODO Comprendre ce que fait ce flag.
			- `GL_CLIENT_STORAGE_BIT`
				- TODO Comprendre ce que fait ce flag.
	- ## Erreurs
		- `GL_INVALID_ENUM` si `target` n'est pas une des valeurs de la liste.
		- `GL_INVALID_VALUE` si `size` est inférieur ou égale à 0.
		- `GL_INVALID_OPERATION` si le buffer attaché est le numéro 0.
		- `GL_OUT_OF_MEMORY` si OpenGL n'est pas capable de générer un stockage avec les `flags` spécifiés.
		- `GL_INVALID_VALUE` si `flags` possède un bit qui n'appartient pas à la liste.
		- `GL_INVALID_VALUE` si `flags` contient `GL_MAP_PERSISTENT_BIT` mais ne possède ni `GL_MAP_READ_BIT` ou `GL_MAP_WRITE_BIT`.
		- `GL_INVALID_VALUE` si `flags` contient `GL_MAP_COHERENT_BIT` mais ne contient pas aussi `GL_MAP_PERSISTENT_BIT`.
		- `GL_INVALID_OPERATION` si le flag `GL_BUFFER_IMMUTABLE_STORAGE` du buffer attaché à la cible vaut `GL_TRUE`.
	- ## Versions OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
- # glBufferData
  id:: 6574be7c-ddc2-477b-a656-4b1f4c288c72
	- ```c
	  void glBufferData(GLenum target,
	                    GLsizeiptr size,
	                    const void *data,
	                    GLenum usage);
	  ```
	- Alloue et initialise un stockage de données.
	- ## Paramètres
		- `target` : voir ((6574b2e0-a34a-4fb4-aec7-592660e21366)).
		- `size` : spécifie la taille en **octets** à allouer pour le buffer.
		- `data` : pointeur vers des données qui seront **copiées** vers le buffer, ou `nullptr` si aucune donnée ne doit être copié.
		- `usage` : indique comment sera utilisée le stockage avec une de ces valeurs :
			- |  | **Dessin** | **Lecture** | **Copie** |
			  | **Stream** | `GL_STREAM_DRAW` | `GL_STREAM_READ` | `GL_STREAM_COPY` |
			  | **Statique** | `GL_STATIC_DRAW` | `GL_STATIC_READ` | `GL_STATIC_COPY` |
			  | **Dynamique** | `GL_DYNAMIC_DRAW` | `GL_DYNAMIC_READ` | `GL_DYNAMIC_COPY` |
			- **Stream** : l'application modifiera les données après chaque utilisation, ou presque à chaque fois.
			- **Statique** : l'application définira les données qu'une seule fois.
			- **Dynamique** : l'application définira les données de temps en temps.
			- **Dessin** : l'application écrira dans le buffer mais ne le lira jamais.
			- **Lecture** : l'application n'écrira pas dans le buffer mais le lira.
			- **Copie** : l'application n'écrira pas et ne lira pas le buffer.
	- ## Erreurs
		- `GL_INVALID_ENUM` si `target` n'est pas une des valeurs de la liste.
		- `GL_INVALID_ENUM` si `usage` n'est pas une des valeurs de la liste.
		- `GL_INVALID_VALUE` si `size` est inférieur ou égale à 0.
		- `GL_INVALID_OPERATION` si le buffer attaché est le numéro 0.
		- `GL_INVALID_OPERATION` si le flag `GL_BUFFER_IMMUTABLE_STORAGE` du buffer attaché à la cible vaut `GL_TRUE`.
		- `GL_OUT_OF_MEMORY` si OpenGL n'est pas capable d'allouer un stockage avec la taille spécifiée.
	- ## Versions OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
- # glBufferSubData
  id:: 6575dc01-c81a-4a8a-9f50-2f2bd1897055
	- ```c
	  void glBufferSubData(GLenum target,
	                       GLintptr offset,
	                       GLsizeiptr size,
	                       const void *data);
	  ```
	- Modifie le contenu d'un buffer.
	- ## Paramètres
		- `target` : voir ((6574b2e0-a34a-4fb4-aec7-592660e21366)).
		- `offset` : indique le début des données à remplacer dans le buffer.
		- `size` : le nombre d'octets à remplacer dans le buffer à partir de `offset`.
		- `data` : les nouvelles données à envoyer dans la zone spécifiée du buffer.
	- ## Erreurs
		- `GL_INVALID_ENUM` si `target` n'est pas une des valeurs de la liste.
		- `GL_INVALID_OPERATION` si le buffer attaché est le numéro 0.
		- `GL_INVALID_VALUE` si `offset` ou `size` est inférieur ou égale à 0. Ou si `size + offset` dépasse `GL_BUFFER_SIZE` du buffer.
		- `GL_INVALID_OPERATION` si une partie de la zone spécifiée a été mappée avec `glMapBufferRange` ou `glMapBuffer`, à moins qu'elle soit mappée avec le flag `GL_MAP_PERSISTENT_BIT`.
		- `GL_INVALID_OPERATION` si la valeur de `GL_BUFFER_IMMUTABLE_STORAGE` est à `GL_TRUE` et que le flag `GL_DYNAMIC_STORAGE_BIT` n'est pas mis.
	- ## Versions OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
- # glGetBufferSubData
  id:: 6575ea44-2345-4bca-ae96-75059a7a9649
	- ```c
	  void glGetBufferSubData(GLenum target,
	                          GLintptr offset,
	                          GLsizeiptr size,
	                          void *data);
	  ```
	- Lit le contenu d'un buffer.
	- ## Paramètres
		- `target` : voir ((6574b2e0-a34a-4fb4-aec7-592660e21366)).
		- `offset` : indique le début des données à récupérer dans le buffer.
		- `size` : le nombre d'octets à récupérer dans le buffer à partir de `offset`.
		- `data` : pointeur vers lequel les données seront stockées.
	- ## Erreurs
		- `GL_INVALID_ENUM` si `target` n'est pas une des valeurs de la liste.
		- `GL_INVALID_OPERATION` si le buffer attaché est le numéro 0.
		- `GL_INVALID_VALUE` si `offset` ou `size` est inférieur ou égale à 0. Ou si `size + offset` dépasse `GL_BUFFER_SIZE` du buffer.
		- `GL_INVALID_OPERATION` si une partie de la zone spécifiée a été mappée avec `glMapBufferRange` ou `glMapBuffer`, à moins qu'elle soit mappée avec le flag `GL_MAP_PERSISTENT_BIT`.
	- ## Versions OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
- # glGetUniformLocation
  id:: 6576119e-c48e-40fe-b8d7-d362c1221cc2
	- ```c
	  GLint glGetUniformLocation(GLuint program,
	                             const GLchar *name);
	  ```
	- Récupère la localisation d'une variable uniforme.
	- ## Paramètres
		- `program` : le programme auquel récupérer la localisation de la variable uniforme.
		- `name` : le nom de la variable uniforme à chercher.
	- ## Retourne
		- La localisation de la variable uniforme si elle a été trouvée.
		- `-1` si aucune variable uniforme n'a été trouvée avec le nom spécifié.
	- ## Erreurs
		- `GL_INVALID_VALUE` si `program` n'est pas une valeur générée par OpenGL.
		- `GL_INVALID_OPERATION` si `program` n'est pas un objet programme.
		- `GL_INVALID_OPERATION` si `program` n'a pas été correctement linké.
	- ## Versions OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
- # glGetUniformBlockIndex
  id:: 6575f42d-565a-46b7-bd81-49222c7004e4
	- ```c
	  GLuint glGetUniformBlockIndex(GLuint program,
	                                const GLchar *uniformBlockName);
	  ```
	- Récupère l'index d'un bloc d'uniformes.
	- ## Paramètres
		- `program` : le programme auquel récupérer l'index du bloc d'uniformes.
		- `uniformBlockName` : le nom du bloc à chercher.
	- ## Retourne
		- L'index du bloc d'uniformes s'il a été trouvé.
		- `GL_INVALID_INDEX` si aucun bloc d'uniformes n'a été trouvé avec le nom spécifié.
	- ## Erreurs
		- `GL_INVALID_OPERATION` si `program` n'est pas le nom d'un programme précédemment linké avec `glLinkProgram`.
	- ## Versions OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
- # glGetActiveUniformBlockiv
	- ```c
	  void glGetActiveUniformBlockiv(GLuint program,
	                                 GLuint uniformBlockIndex,
	                                 GLenum pname,
	                                 GLint *params);
	  ```
	- Envoie une requête à propos d'un bloc d'uniformes pour récupérer des informations sur celui-ci.
	- ## Paramètres
		- `program` : le programme sur lequel le bloc d'uniformes est présent.
		- `uniformBlockIndex` : l'index du bloc d'uniformes, récupérable grâce à ((6575f42d-565a-46b7-bd81-49222c7004e4)).
		- `pname` : spécifie l'information à récupérer, peut être une des valeurs suivantes :
			- | **Constante** | **Description** |
			  | `GL_UNIFORM_BLOCK_BINDING` | Récupère le point de binding du buffer ou `0` s'il n'en a pas. |
			  | `GL_UNIFORM_BLOCK_DATA_SIZE` | Récupère la taille minimale totale du bloc d'uniformes. |
			  | `GL_UNIFORM_BLOCK_NAME_LENGTH` | Récupère la longueur du nom du bloc d'uniformes en incluant le caractère nul. |
			  | `GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS` | Récupère le nombre d'uniformes actifs dans le bloc. |
			  | `GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES` | Récupère les index des uniformes actifs, le nombre de valeurs écrites dans `params` est la valeur de `GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS`. |
			  | `GL_UNIFORM_BLOCK_REFERENCED_BY_*` | Récupère un booléen qui détermine si le bloc est utilisé par les différents shaders de la pipeline. |
		- `params` : pointeur vers la variable qui stockera le résultat de la requête.
	- ## Erreurs
		- `GL_INVALID_VALUE` si `uniformBlockIndex` est supérieur ou égale à la valeur de `GL_ACTIVE_UNIFORM_BLOCKS` ou n'est pas l'index d'un bloc d'uniformes dans `program`.
		- `GL_INVALID_ENUM` si `pname` n'est pas une des constantes de la liste.
		- `GL_INVALID_OPERATION` si `program` n'est pas le nom d'un programme précédemment linké avec `glLinkProgram`.
	- ## Versions OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
-