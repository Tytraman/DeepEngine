# Types de smart pointers
Il existe plusieurs types de pointeurs intelligents :
- `unique_ptr`
- `shared_ptr`
- `weak_ptr`

# unique_ptr
Ce type de pointeur permet d'avoir un pointeur "scoppé", c'est-à-dire qu'une fois sorti du scope, le destructeur de l'objet pointé sera appelé.
Comme son nom l'indique, le pointeur est **unique** et ne peut **pas** être copié.
```c++
std::unique_ptr<object> o1 = std::make_unique<object>();
std::unique_ptr<object> o2 = o1;  // Erreur de compilation.
```

## Questions
- Est-ce qu'un `shared_ptr` peut copier le pointeur d'un `unique_ptr` ?

# shared_ptr
Ce type de pointeur permet de **partager** la référence vers une zone mémoire tout en **comptabilisant** le nombre de référence.
Lorsque le nombre de référence atteint `0`, le destructeur de l'objet pointé est appelé.