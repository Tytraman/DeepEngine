# Notions à comprendre avant

- [[Normal]]

# Basique
## Phong Reflection Model

![[phong_reflection_model.png]]
![[phong_reflection_model_example_2.png]]
### Ambient
C'est la lumière d'ambiance, cela peut être la lumière naturelle comme celle du soleil ou bien une lumière artificielle qui impacte toute la scène.
Lorsqu'une surface n'est éclairée par aucune source de lumière, elle ne sera affectée que par la lumière d'ambiance.

### Diffuse
Cette étape affecte la **luminance** appliquée à la surface selon l'angle de la source **émettrice** 

![[lighting_diffuse_1.png|350]]
*Quand la source émettrice est à 90° de la surface, la luminance est au maximum.*

![[lighting_diffuse_2.png|450]]
*On voit que quand l'angle diminue, la luminance aussi.*

![[lighting_diffuse_3.png|800]]
*Quand l'angle entre la source et la surface est négatif, alors aucune diffusion n'est appliquée. La surface n'est donc affectée que par la lumière d'ambiance.*

### Specular

Cette étape permet de refléter la lumière sur l'objet. Le niveau de reflection dépend d'à quel point la surface à un effet 'miroir', représenter par ce qu'on appelle `shininess`.
