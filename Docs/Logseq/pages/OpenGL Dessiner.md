- Il existe plusieurs façons de dessiner avec OpenGL.
- # glDrawArrays
  id:: 65744865-0dfc-4e8f-b52e-18621d3340c6
	- Cette fonction permet de dessiner le **VAO** actuellement attaché.
	- ```c
	  void glDrawArrays(GLenum mode, GLint first, GLsizei count);
	  ```
		- Construit une séquence de primitives géométriques à partir du **VAO** attaché.
		- Commence à `first` et termine à `first + count`.
		- `mode` détermine quel type de primitives dessiner parmis la liste suivante :
			- `GL_POINTS`
			- `GL_LINES`
			- `GL_LINE_STRIP`
			- `GL_LINE_LOOP`
			- `GL_TRIANGLES`
			- `GL_TRIANGLE_STRIP`
			- `GL_TRIANGLE_FAN`
			- `GL_PATCHES`
	- Peut-être perçue comme un raccourci vers la fonction plus complexe ((65744c8a-d28d-4e45-95d6-9f4c4f625038)).
	- ## Version OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
- # glDrawArraysInstancedBaseInstance
  id:: 65744c8a-d28d-4e45-95d6-9f4c4f625038
	- TODO Décrire la fonction void glDrawArraysInstancedBaseInstance.
	- ```c
	  void glDrawArraysInstancedBaseInstance(GLenum mode,
	                                         GLint first,
	                                         GLsizei count,
	                                         GLsizei instancecount,
	                                         GLuint baseinstance);
	  ```
		- Commence à `first` et termine à `first + count`.
		- `mode` détermine quel type de primitives dessiner parmis la liste suivante :
			- `GL_POINTS`
			- `GL_LINE_STRIP`
			- `GL_LINE_LOOP`
			- `GL_LINES`
			- `GL_TRIANGLE_STRIP`
			- `GL_TRIANGLE_FAN`
			- `GL_TRIANGLES`
			- `GL_LINES_ADJACENCY`
			- `GL_LINE_STRIP_ADJACENCY`
			- `GL_TRIANGLES_ADJACENCY`
			- `GL_TRIANGLE_STRIP_ADJACENCY`
			- `GL_PATCHES`
		- `instancecount` spécifie le nombre d'**instances** des indices à dessiner.
		- TODO Comprendre le paramètre `baseinstance`.
	- Cette fonction fait exactement la même chose que ((65744865-0dfc-4e8f-b52e-18621d3340c6)) à l'exception qu'elle fait `instancecount` fois le rendu des indices spécifiés.
	- Le vertex shader peut accéder au numéro d'itération *(entier 32 bits)* grâce à `gl_InstanceID`.
	- Cette fonction a le même effet que :
		- ```c
		  if(mode or count is invalid)
		  	generate appropriate error
		  else
		  {
		  	for(int i = 0; i < instancecount; ++i)
		      {
		  		instanceID = i;
		  		glDrawArrays(mode, first, count);
		  	}
		  	instanceID = 0;
		  }
		  ```
	- ## Version OpenGL
	  | **2.0** | **2.1** | **3.0** | **3.1** | **3.2** | **3.3** | **4.0** | **4.1** | **4.2** | **4.3** | **4.4** | **4.5** |
	  | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ |
-