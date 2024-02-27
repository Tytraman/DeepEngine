Comme en C++ il n'existe pas le concept de *Garbage Collector*, il faut libérer soi-même la mémoire. Cette tâche est plutôt simple, mais quand le code devient de plus en plus conséquent, cela devient très vite compliqué de pouvoir libérer toute la mémoire.
Pour plusieurs raisons :
	- **L'oubli** : il est très facile d'oublier de libérer une zone mémoire.
	- **La façon dont le code est écrit** : je trouve par exemple difficile de gérer la mémoire lorsque l'on applique le design *Dependency Injection*.

Une des manières de gérer automatiquement ces zones mémoires est par l'utilisation de compteurs de références.

Le principe repose sur le fait que l'on compte combien il y a de références vers une zone mémoire et lorsque le compteur tombe à `0`, la zone mémoire est libérée.
# RefCounted
**RefCounted** est une classe que d'autres classes peuvent hériter pour compter le nombre de références qu'il y a et détruire l'objet lorsque le compteur tombe à `0`.

## Attributs
- `SafeRefCount` refcount
- `SafeRefCount` refcount_init

## Méthodes

### init_ref
Initialise le compteur interne.
Retourne `true` si l'initialisation a réussi.
### reference
Augmente le compteur de références de `1`.
Retourne `true` si l'incrémentation a réussi.
### unreference
Diminue le compteur de références de `1`.
Retourne `true` si l'objet doit être libéré de la mémoire.
### get_reference_count
Retourne le nombre de *références* qu'il y a vers l'objet.


A elle toute seule, cette classe ne sert pas à grand chose, elle fonctionne en complément de la classe suivante :

# Ref
C'est une classe `template` qui encapsule une classe à compter. Elle doit être utilisée afin de pouvoir se servir de [[Compteur de références#RefCounted]]

## Exemple

```c++
class MyReference: public RefCounted {
	GDCLASS(MyReference, RefCounted);
};

Ref<MyReference> myref(memnew(MyReference));
```

## Constructeurs

### Pointeur
```c++
Ref(T *p_reference);
```

```c++
Ref(T *p_reference) {
	if (p_reference) {
		ref_pointer(p_reference);
	}
}
```

Ici, `p_reference` ne doit pas être à `nullptr` car la fonction `ref_pointer` déréférence le pointeur.
Voici à quoi ressemble la fonction `ref_pointer` :

```c++
void ref_pointer(T *p_ref) {
	ERR_FAIL_NULL(p_ref);

	if (p_ref->init_ref()) {
		reference = p_ref;
	}
}
```

La chose importante à noter ici, c'est que le nombre de références vers l'objet est stocké dans l'objet lui-même à travers la classe `RefCounted`.

`init_ref` est une fonction qui fait partie de `RefCounted` donc de l'objet à compter lui-même.

