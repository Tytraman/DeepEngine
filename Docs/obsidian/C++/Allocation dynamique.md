
# Appeler le constructeur

Il existe 2 manières d'appeler le constructeur d'un objet :
- Sur le stack.
- Avec le mot clé `new`.

## Avec le mot clé `new`

Le mot clé `new` permet d'allouer de la mémoire sur le heap afin d'y stocker les données de l'objet. En faisant cela, le constructeur de l'objet est appelé.

### Sur une zone mémoire déjà existante

Il existe un moyen d'appeler le constructeur d'un objet sur une zone mémoire déjà existante, sans avoir besoin de la supprimer et de la réallouer.
Pour cela, il faut inclure l'en-tête `<new>` et suivre cette syntax :
```c++
object *ptr = malloc(sizeof(object));
new(ptr) object(args);    // args équivaut aux paramètres du constructeur.
```

