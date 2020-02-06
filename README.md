# Entity component system for creative coding apps
it's main focus is to **provide a simple api for composing objects and traversing data.** Traditional ecs libs are fine when your have some game like enviroment where most of the objects in the scene can be represented by components and their behaviour by systems, but in "creative coding" apps is common to have entities with a very specialized behaviour breaking the ECS mindset, this library tries to fix  that by not having very strict rules.

It also provides a couple of nice features that I think is usefull on a day-to-day creative coding eviroment like:

1. A transform system and a button system (  mouse click )
2. Draw targets ( usefull for drawing into an FBO's, sccissore'd  scenes,  and post processing ( see ECSRenderingTarget sample )
3. A somewhat naive serialzation mechanism (usufull for saving and loading scenes  )

### Disclameer
 I haven't done any performance tests but it's probably not the fastest ecs in the market, it's also not used in production extensively, so it is somewhat of a learning project, so use with care! 

 *some references that I used to make this:*
 
 1. EntityX
 2. Vitoreo Romero tutorials
 
I initially made the mistake of creating each component as an shared_ptr, that's a huge performance killer and sort of removes the purpuse of having the components packed into a sequencial array. Ideally we should use a hash based system like the one described over here:
[http://bitsquid.blogspot.com/2014/08/building-data-oriented-entity-system.html](http://bitsquid.blogspot.com/2014/08/building-data-oriented-entity-system.html)

 # Manager, Components & entities

## Managers

A manager is the ‘god like’ class that stores all the entities, components and systems.
We use it to create entities and systems.

You will need to call  ` Manager::setup() `, ` Manager::update() ` and ` Manager::draw() ` in the respective cinder functions

Entities are containers of components 
You can create an entity with the ecs::Manager.

```auto exampleEntity =  mManager.createEntity();```

## Components

```
struct ColorComponent : public ecs::Component{
  Color mColor;
};
```

And adding them is as simple as: 
`exampleEntity->addComponent<ColorComponent>();`

The component will be added to the manager, that way we ensure all ColorComponents live in a vector next to each other. The entity itself only holds  a raw ptr

You can modify a component in a entity by using the function “getComponent<T>()”

```
exampleEntity->getComponent<ColorComponent>()->mColor = Color::gray(0.5);

for( auto& c: mManager.getComponentArray<ColorComponent>() ){

shared_ptr<ColorComponent> cc = static_pointer_cast<ColorComponent>(c);
cc->mColor = Color(1.0f, 0.0f, 0.0f);
}
```
You can also access all entities with a component mask:

```for( auto& c: mManager.getEntitiesWithComponents<ColorComponent, RectComponent>() ){
e.getComponent<ColorComponent>->mColor = Color(1.0f, 0.0f, 0.0f); 
}
```

## Entity & Entity inheritance

Image that you have a scene with a lot of buttons, the buttons can be a default entity with components. For example:
```
auto btEntity = mManager.createEntity();
btEntity->addComponent<Button>(); // handles user input, callback etc..
btEntity->addComponent<Transform>();
btEntity->addComponent<Texture>(); // on click callback swaps this texture
```

And a scene entity it self would be an object that is used once and has lot's of specialized behaviours, it's more straight foward to extend the entity class.   

```
struct Scene : public ecs::Entity{

  setup() override {
    add components… transform, etc… 
  }

  animateIn(){
  }

  animateOut(){
  }
  
}

shared_ptr<Scene> scene = mManager.createEntity<Scene>();
```
## Systems

System are the only objects that get update and draw called every frame. They are an efficient way to update a bunch of components together

```
Struct ParticleSystem : ecs::System{

  void update() override{

  }
  void draw() override{

  }
}
```

and to create one: 
```auto particleSystem = mManager.createSystem<ParticleSystem>();```


### build tests:

1. `mkdir build && cd build`
2. `cmake ..`
3. `make && ./tests/tests`

### Todo's:
    1. add aditional tests
        * child classes of ecs::Entity
        * Transforms 
        * Drawables components
    
    3. threading? 
    4. make drawables and Iupdatables -a single object and components, not interfaces
    5. measure performance
    6. Merge ofxEcs and Ecs-Cinder into one block
