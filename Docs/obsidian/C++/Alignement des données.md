Il est possible d'aligner certaines données grâce à des mots-clés qui diffèrent selon le compilateur utilisé.

# C++11
Directement défini dans le standard du C++11 donc disponible peu importe le compilateur.
- `alignas(__x)`
	- `__x` est la valeur d'alignement en octets. Elle doit être un multiple de `2`.

# Microsoft C/C++ (MSVC)
Défini par le compilateur de **Microsoft**.
- `__declspec(align(__x))`
	- `__x` est la valeur d'alignement en octets. Elle doit être un multiple de `2` qui peut aller de `1` à `8192`.

# GNU Compiler Collection (GCC)
Défini par la collection de compilateurs GNU.
- `__attribute__ ((aligned (__x)))`
	- `__x` est la valeur d'alignement en octets. Elle doit être un multiple de `2`.

# Clang
Clang est compatible avec **GCC**, ce qui implique que les attributs le soient.
Se référer à [[Alignement des données#GNU Compiler Collection (GCC)|GCC]].

