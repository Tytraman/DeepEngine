**GLSL** est un langage de programmation haut niveau avec une syntax basée sur le **langage C** qui permet d'écrire du code exécuté par une carte graphique.

# Types de données

## Types basiques

| Nom | Description |
| ---- | ---- |
| `bool` | Type conditionnel, la valeur peut être `true` ou `false`. |
| `int` | Un entier 32-bit signé avec le complément à 2. |
| `uint` | Un entier 32-bit non signé. |
| `float` | Un nombre flottant **IEEE-754** avec une précision simple. |
| `double` | Un nombre flottant **IEEE-754** avec une double précision. |

## Vecteurs

| Nom | Description |
| ---- | ---- |
| `bvecn` | Un vecteur de `bool`. |
| `ivecn` | Un vecteur de `int`. |
| `uvecn` | Un vecteur de `uint`. |
| `vecn` | Un vecteur de `float`. |
| `dvecn` | Un vecteur de `double`. |

`n` est à remplacer par un chiffre entre `2` et `4`.

## Matrices

| Nom | Description |
| ---- | ---- |
| `matnxm` | Une matrice de `float` avec `n` colonnes et `m` lignes. |
| `matn` | Une matrice de `float` avec `n` colonnes et `n` lignes. |
| `dmatnxm` | **GL 4.0**, une matrice de `double` avec `n` colonnes et `m` lignes. |
| `dmatn` | **GL 4.0**, une matrice de `double` avec `n` colonnes et `n` lignes. |

## Types opaques

Les types opaques sont des références à des objets internes d'OpenGL, ils ne contiennent pas de "*valeurs*" dans le même sens que les types *normaux*.

| Nom | Description |
| ---- | ---- |
| Samplers | Représente une texture attachée à OpenGL. |
| Images | Fait référence à une image d'un type particulier. |
| Compteurs atomiques | Représente une localisation dans la mémoire d'un **Buffer Object** dans laquelle une opération atomique peut prendre place. |

## Tableaux

Il est possible d'utiliser des tableaux de la même manière qu'en **C / C++**, cependant il y a quelques limitations :
- Ils ne peuvent pas être multi-dimensionnels. Sauf à partir de **GL 4.3** ou avec l'utilisation de l'extension **ARB_arrays_of_arrays**.
- Elles doivent être initialisées avec une valeur **constante**.

La taille d'un tableau peut être récupéré grâce à la fonction `length`, par exemple :
```glsl
uniform float myValues[12];

...

myValues.length();    // Returns 12.
```
La fonction `length` retourne une valeur constante.

## Structure

Les structures sont exactement les mêmes qu'en **C / C++** :
```glsl
struct Light
{
  vec3 eyePosOrDir;
  bool isDirectional;
  vec3 intensity;
  float attenuation;
};
```

Quelques exceptions subsistent :
- Il n'est pas possible de déclarer une structure anonyme.
- Une structure doit posséder **au moins** un champ.
- Les structures ne peuvent pas être déclaré dans une autre structure.

Un constructeur est automatiquement créé et prend tous les champs en paramètres, par exemple, pour la structure `Light`, le constructeur serait :
```glsl
Light(const vec3 &eyePosOrDir, bool isDirectional, const vec3 &intensity, float attenuation);
```

# Données

Il est possible, comme en **C / C++**, d'utiliser des qualificateurs devant les types de variables pour modifier la façon dont celles-ci sont utilisables / utilisées.

## Inputs et outputs

Plusieurs qualificateurs existent pour les variables entrantes et sortantes, dont voici la liste :

| Nom | Description |
| ---- | ---- |
| `flat` | La valeur de la variable ne sera pas interpolée entre les 2 stages des shaders. |
| `noperspective` | La valeur de la variable sera interpolée de façon linéaire dans l'espace fenêtre. Ce n'est généralement pas utilisé. |
| `smooth` | La valeur de la variable sera interpolée entre les 2 stages des shaders, *activé par défaut si aucun qualificateur n'est présent*. |

