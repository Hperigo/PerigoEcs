# Entity component system for creative coding apps

### build tests:

1. `mkdir build && cd build`
2. `cmake ..`
3. `make && ./tests/tests`

### Todo: 
    1. add aditional tests
        * child classes of ecs::Entity
        * Transforms 
        * Drawables
    
    2. custom deleter in EntityRef
    3. threading? 
    4. make drawables and Iupdatables a single object
    5. measure performance