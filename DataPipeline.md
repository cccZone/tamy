# Data pipeline #

It allows you to define pipelines that propagate data from one point to another, shaping and changing it.

## Rationale ##

Here are use cases according to which the pipeline should operate:
  * it should operate on arbitrary nodes and edges ( which could be RTTI `Objects` )
  * it should be able to serialize them
  * we should be able to put it in a `Resource`
  * other `Resources` should be able to store pointers to its nodes and edges
  * we should be able to pass data between the nodes via the edges.

The pipeline should be compiled to a simple `Graph`, as its a structure we can easily bend to our will and process using the existing algorithms.

## Components ##

  * `GraphBuilder` - represents the pipeline. It contains a method `buildGraph` that allows you to instantiate a `Graph` instance.

  * `GraphBuilderNode` - represents a graph node. It can contain a number of named input and output sockets that define how many incoming and outgoing edges a node can have.

  * `GBNodeOutput` - a socket from which a graph edge starts. It can have some data associated with it. The real strength lies in the fact that the parent node **knows** about this socket and it can access it.

  * `GBNodeInput` - a socket where a graph edge ends. It **knows** what input socket it is connected to and can access it, and the parent node knows about this socket.

All classes are template classes, meaning that you can define what custom implementations they'll be representing, and not worry about casting.


## Data propagation ##

About the sockets - it's a powerfull mechanism that allows to exchange data between the nodes.
So the graph can now not only be used to represent some static data, but can actually **propagate** data.

How can you achieve that? Well - basic definitions don't provide that functionality, however if you chose to have it, you can create implementations that:
  * have a data setter and getter on the output node - and the parent node can set the data.
  * have a counterpart input node that can connect only with the proper output ( you can define what output can be connected to an input by overriding the `GBNodeInput::canConnect` method ).
  * have a getter method on the custom input the parent block can query, and which in turn queries the data from the input.

[Rendering pipeline](RendererCore#Rendering_pipeline.md) and a few other implementations use that data propagation mechanism - check it out.


## How to use it ##

As I mentioned, the base classes are templates, and are not `RTTI` oriented ( except for the sockets - but those are not registered and are objects just to provide future RTTI capabilites from the base class ).

**COMMENT**: For simplicity sake, I'll skip all additional functionalities and describe the skeleton you have to define in order to get your graph resources up and running.

First of all you need to define your own `GraphBuilder` class, like so:

```
// ------------------------- .h file ------------------------------
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilder.h"
#include <vector>


///////////////////////////////////////////////////////////////////////

class MyGraphNode;

///////////////////////////////////////////////////////////////////////

class MyGraphBuilder: public Resource, public GraphBuilder< MyGraphBuilder, MyGraphNode>
{
   DECLARE_RESOURCE( MyGraphBuilder);

public:
   /**
    * Constructor.
    *
    * @param resourceName   name of the resource file
    */
   MyGraphBuilder( const FilePath& resourceName = FilePath() );

protected:
   // -----------------------------------------------------------------
   // GraphBuilder implementation
   // -----------------------------------------------------------------
   void onNodeAdded( MyGraphNode* node );
   void onNodeRemoved( MyGraphNode& node );
};

// ------------------------- .cpp file ------------------------------

///////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( MyGraphBuilder, Resource, myres, AM_BINARY );
   PROPERTY( std::vector< MyGraphNode* >, m_nodes );
   // ... your own additional properties go here
END_RESOURCE();

///////////////////////////////////////////////////////////////////////

MyGraphBuilder::MyGraphBuilder( const FilePath& resourceName )
   : Resource( resourceName )
{
}

///////////////////////////////////////////////////////////////////////

void MyGraphBuilder::onNodeAdded( MyGraphNode* node )
{
   addObject( node );
}

///////////////////////////////////////////////////////////////////////

void MyGraphBuilder::onNodeRemoved( MyGraphNode& node )
{
   removeObject( node.getObjectId() );
}

///////////////////////////////////////////////////////////////////////

// ------------------- Type registration file ------------------------

REGISTER_RTTI( MyGraphBuilder );
```


Next - you have to define your node:

```
// ------------------------- .h file ------------------------------
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilderNode.h"
#include "MySockets.h"


///////////////////////////////////////////////////////////////////////

class MyGraphNode: public ResourceObject, public GraphBuilderNode< MyGraphNode >
{
   DECLARE_CLASS( MyGraphNode)

public:
   virtual ~MyGraphNode() {}

   // -----------------------------------------------------------------
   // Object implementation
   // -----------------------------------------------------------------
   void onObjectLoaded();
   void onPropertyChanged( Property& property );
};

///////////////////////////////////////////////////////////////////////

// ------------------------- .cpp file ------------------------------

///////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( MyGraphNode, ResourceObject );
   PROPERTY( InputsMap, m_inputs );
   PROPERTY( OutputsMap, m_outputs );
   // custom node properties go here
END_OBJECT();

///////////////////////////////////////////////////////////////////////

void MyGraphNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   onBulkSocketsInitialization();
}

///////////////////////////////////////////////////////////////////////

void MyGraphNode::onPropertyChanged( Property& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_inputs" )
   {
      notify( GBNO_INPUTS_CHANGED );
   }
   else if ( property.getName() == "m_outputs" )
   {
      notify( GBNO_OUTPUTS_CHANGED );
   }
   else
   {
      notify( GBNO_CHANGED );
   }
}

///////////////////////////////////////////////////////////////////////

// ------------------- Type registration file ------------------------

REGISTER_RTTI( MyGraphNode );

```

And finally the sockets ( I'm gonna show a sample socket that marshals floats ):

```
// ------------------------- .h file ------------------------------
#pragma once

#include "core/GraphBuilderSockets.h"
#include "core/Object.h"


///////////////////////////////////////////////////////////////////////

class MyGraphNode;

///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

class MyGraphNode;
typedef GBNodeOutput< MyGraphNode > MyOutput;

///////////////////////////////////////////////////////////////////////

class MyFloatInput : public TGBNodeInput< MyGraphNode, float >
{
   DECLARE_CLASS( MyFloatInput )

public:
   /**
    * Constructor.
    */
   MyFloatInput ( const std::string& name = "" ) : TGBNodeInput< MyGraphNode, float >( name ) {}
};

// -------------------------------------------------------------------

class MyFloatOutput : public TGBNodeOutput< MyGraphNode, float >
{
   DECLARE_CLASS( MyFloatOutput )

public:
   /**
    * Constructor.
    */
   MyFloatOutput ( const std::string& name = "" ) : TGBNodeOutput< MyGraphNode, float >( name ) {}
};

///////////////////////////////////////////////////////////////////////

// ------------------------- .cpp file ------------------------------

///////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MyFloatInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( MyOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MyFloatOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MyGraphNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////

// ------------------- Type registration file ------------------------

REGISTER_RTTI( MyFloatInput );
REGISTER_RTTI( MyFloatOutput);

```

## Creating a simple pipeline ##


First of all, we'll assume that we have our classes definitions created:

```
   class GBMockNode;
   class MockOutput : public GBNodeOutput< GBMockNode >
   {
   public:
      MockOutput() : GBNodeOutput< GBMockNode >( "Output" ) {}
   };

   // -----------------------------------------------------------------

   class MockInput : public GBNodeInput< GBMockNode>
   {
   public:
      MockInput() : GBNodeInput< GBMockNode>( "Input" ) {}
   };

   // -----------------------------------------------------------------

   class GBMockNode : public GraphBuilderNode< GBMockNode >
   {
   public:
      int         m_idx;

   public:
      GBMockNode( int idx ) : m_idx( idx ) {}
      virtual ~GBMockNode() {}
   };

   // -----------------------------------------------------------------

   class GBMStartNode : public GBMockNode
   {
   public:
      GBMStartNode( int idx ) : GBMockNode( idx ) 
      {
         defineOutput( new MockOutput() );
      }
   };

   // -----------------------------------------------------------------

   class GBMEndNode : public GBMockNode
   {
   public:
      GBMEndNode( int idx ) : GBMockNode( idx ) 
      {
         defineInput( new MockInput() );
      }
   };

   // -------------------------------------------------------------------------

   class GBMUtilNode : public GBMockNode
   {
   public:
      int         m_idx;

   public:
      GBMUtilNode( int idx ) : GBMockNode( idx )
      {
         defineInput( new MockInput() );
         defineOutput( new MockOutput() );
      }
   };

   // -----------------------------------------------------------------

   class GraphBuilderMock : public GraphBuilder< GraphBuilderMock, GBMockNode >
   {
   public:
      // --------------------------------------------------------------
      // GraphBuilder implementation
      // --------------------------------------------------------------
      void onNodeAdded( GBMockNode* node ) {}
      void onNodeRemoved( GBMockNode& node ) {}
   };
```

As you can see, here we have 3 node types:
  * Start ( with one output called "Output" )
  * Util ( with an input called "Input" and an output called "Output" )
  * End ( with on input called "Input" )

Let's connect them in a simple manner:

```
GraphBuilderMock builder;

GBMockNode* node[] = { new GBMStartNode( 0 ), new GBMUtilNode( 1 ), new GBMEndNode( 2 ) };

builder.addNode( node[0] );
builder.addNode( node[1] );
builder.addNode( node[2] );

node[0]->connect( "Output", node[1], "Input" );
node[1]->connect( "Output", node[2], "Input" );
```


Method `GraphBuilderNode::connect` facilitates creating connection between two nodes. Simply specify the output of the original node, the target node and its input we want to connect to, and the connection will be established.


Whenever you want to disconnect two nodes, you can use the `GraphBuilderNode::disconnect` method:
```
/**
 * Disconnects this node from the specified node's input.
 *
 * @param destNode
 * @param inputName
 */
void disconnect( Impl& destNode, const std::string& inputName );
```