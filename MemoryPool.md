# Memory Pool #

A memory pool is a continous, preallocated area in memory that can  be used for faster memory allocations for objects.

This particular implementation is very similar to the concept used in the [round buffer](RoundBuffer.md), but with an exception, that we don't deallocate the memory when an object from the pool gets deleted.
This saves us the necessity to manage the defragmented memory chunks etc. and allows to quickly allocate memory for new objects.

The user gets one method - `reset` - which resets the pool's allocation pointer and allows to start allocating from the beginning of the pool.
**CAUTION**: Call it once you deleted all the objects allocated in the pool.

This approach makes the pool a perfect candidate for situations, when we need to allocate a huge amount of objects and delete them afterwards.
You can see this example in the [rendering view's render tree builder](RenderingView#RenderTreeBuilder.md) implementation, where the entire render tree is built every frame, and we allocate a lot of tree objects during that time, just to delete them a milisec later.


## Usage ##

It's as simple as in case of `RoundBuffer`.

```
#include "core/MemoryPool.h"

// ...

MemoryPool pool( 512 );

// add a few objects
MyPoolObject* obj1 = new ( pool ) MyPoolObject( 1 );
MyPoolObject* obj2 = new ( pool ) MyPoolObject( 2 );
MyPoolObject* obj3 = new ( pool ) MyPoolObject( 3 );

// do some stuff with'em

// delete them
MEMPOOL_DELETE( obj1 );
MEMPOOL_DELETE( obj2 );
MEMPOOL_DELETE( obj2 );


// and reset the pool's pointer, so that we can start the allocations anew
pool.reset();

```

First you create a `MemoryPool` instance, specifying its capacity in bytes.

Next, you allocate a few objects in it using the `placement new` operator, and then once you're done with them, you can delete them using a handy `MEMPOOL_DELETE` macro.

**Remember to reset the pool** once you're done with the iteration, so that the next iteration can start fresh.

## Pool objects ##

Defining a pool object is also easy - simply implement the `MemoryPoolObject interface like so:

```
class MyPoolObject : public MemoryPoolObject
{
};
```

It doesn't contain any methods that need implementing or reimplementing, so you're instantaneously good to go.class MyPoolObject : public MemoryPoolObject
{
};
}}}```