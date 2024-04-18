# Stoff2D
2D game engine in C.

### Some important things
- Checkout settings.h for control over some internals.

- Checkout README.ani to learn how to load animations with ease.

- The renderer uses a blank texture "white.png" for rendering coloured quads, so
  don't delete it.

- The font texture can be removed it's just an example.

- To add components to the ECS, go into defines.h and add them there under the 
  ECS section.
    - Create a new component struct 
    - Add an enum for it in "ComponentTypes" 
    - Add the struct to the union in "Component" 
  and you're done! You can now use s2d_ecs functions to use your new component.

- If you want the debug function s2d_ecs_print_components() to print component 
  names correctly, add the new component name to the array of names "componontStrings"
  at the top of ecs.c  
