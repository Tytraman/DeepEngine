
# Assertion à la compilation

Grâce à `static_assert`, il est possible de vérifier une expression constante au moment de la compilation.

## Utilisation

```c++
static_assert( constant-expression, string-literal );

static_assert( constant-expression );
```

Si `constant-expression` vaut `false`, une assertion se produit et affiche `string-literal` dans la console.

