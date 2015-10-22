# Resources #

Resource is a special kind of class that binds a working engine object and a file in a filesystem that contains the definition of that object.
As such, the bulk of built-in functionality revolves around the issues of serialization and dependencies mapping.

Resource class is the cornerstone of the Resources system the engine is built upon. A single Resource is considered a constant data source, referenced by other engine classes that operate on that data, but not modifying it. Imagine an animation clip being such a resource - you can have one clip instance in the memory, and multiple animation players playing it on multiple characters moving on screen.

## Referencing a resource in your code ##

**IMPORTANT**
Resources come and go - you load them when you need them, and they can get unloaded if they're not used at the moment, and we need some spare memory.

That's why, as a rule of thumb - **NEVER USE POINTERS TO RESOURCES AS YOUR CLASS MEMBERS**.

Instead - use a `ResourceHandler` and use its API to get access to a resource instance.
That way you'll know that you will always get a hold of a resource ( provided it exists in your filesystem ).

## Resource definition ##

```

```

## Resources management ##
In order to facilitate resources management, a singleton ResourcesManager class was introduced.


You are not obliged to use the ResourcesManager - if you want to use it localy, you can just create it as a regular object - after all its a regular class.
But if you want to load it from the filesystem, we strongly suggest you going through the ResourcesManager API, which will take care of loading all external resources the resource may reference.

## Serialization ##

Resources rarely go just by themselves. They usually reference other resources etc.

In order to serialize the whole net of resource dependencies, a special tool's been prepared - `ReflectionSerializationUtil`.

Apart from handling resources serialization, it also allows you to serialize standalone objects and has a few other tools up its sleeve.

For details, reference the dedicated [Wiki page](ReflectionSerializationUtil.md)

A special tool's been prepared to facilitate the serialization of