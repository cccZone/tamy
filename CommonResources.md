# Common resources #

Sometimes, especially when we're importing a scene, we may need to create an entity of some sort.

The best example here are materials - if you're importing geometry ( and only geometry ) from an external file, you usually would like to set it up with some default materials so that you can immediately see what you imported.

`BasicRenderingEntitiesFactory` was created just for this purpose.

It contains a variety of methods that allow you to create all rendering entities ( such as `MaterialEntit` i.e. ), already set up with common rendering resources.

## Creating a material entity ##
Just all this method an you're good to go:

```
   /**
    * Creates a basic material entity.
    *
    * @param name
    */
   static MaterialEntity* createMaterial( const std::string& name = "" );
```