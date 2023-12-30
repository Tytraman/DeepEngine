**RefCounted** est une classe que d'autres classes peuvent hériter pour compter le nombre de références qu'il y a et détruire l'objet lorsque le compteur tombe à `0`.

# Attributs
- `SafeRefCount` refcount
- `SafeRefCount` refcount_init

# Méthodes

## init_ref
Initialise le compteur interne.
## reference
Augmente le compteur de références de `1`.

## unreference
Diminue le compteur de références de `1`.
## get_reference_count
Retourne le nombre de *références* qu'il a vers l'objet.

