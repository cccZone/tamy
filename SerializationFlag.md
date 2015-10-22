# Serialization flag #

## Rationale ##
Sometimes you need to be able to tell if a method was called during serialization or not.

The best example is an object constructor.
Say you have an object that instantiates another object in its constructor.
But that object is serialized. So as soon as the serialization code kicks in, the old instance will be wiped clean, and a memory leak will occur.

You need something to tell you that the code was invoked in the serialization context.

This is where a `SerializationFlag` comes in handy.

## Serialization context ##
This context takes place only when an object is being deserialized.
To be exact - it's activated only for the duration of a deserialized object instantiation, and is disabled directly afterwards.


## Usage ##

Simply call this line of code:
```
bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
```

If the code is being called from a serialization context, the method will return `true`.