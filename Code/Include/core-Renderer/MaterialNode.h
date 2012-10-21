/// @file   core-Renderer/MaterialNode.h
/// @brief  material graph node
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilderNode.h"
#include "core-Renderer/MaterialSockets.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialInstance;
class MaterialEntity;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

class MaterialNode : public ResourceObject, public GraphBuilderNode< MaterialNode >
{
   DECLARE_ALLOCATOR( MaterialNode, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   virtual ~MaterialNode();

   /**
    * Creates an input with the specified name of the specified type.
    *
    * @param dataType      what type of data should the input marshal
    * @param name          input name
    */
   GBNodeInput< MaterialNode >* createInput( const ReflectionType& dataType, const std::string& name ) const;

   /**
    * Called in order to initialize node's runtime data layout.
    *
    * @param host
    */
   void createLayout( const MaterialInstance& host ) const;

   /**
    * Called when the graph is fully restored. Allows to reprocess certain nodes - remove
    * unused sockets etc. and get the graph structure up to date with external
    * dependencies in general.
    */
   virtual void onGraphLoaded() {}

   /**
    * Called by the instance that uses the material in order to set it up.
    *
    * @param renderer
    * @param instance      rendered material instance
    * @param entity        rendered entity
    */
   virtual void preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const {}

   /**
    * Called by the instance that uses the material in order to clean up after using it.
    *
    * @param renderer
    * @param instance      rendered material instance
    * @param entity        rendered entity
    */
   virtual void postRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const {}

   // -----------------------------------------------------------------
   // Object implementation
   // -----------------------------------------------------------------
   void onPropertyChanged( ReflectionProperty& property );

protected:
   /**
    * Called in order to initialize node implementation's specific runtime data layout.
    *
    * @param host
    */
   virtual void onCreateLayout( const MaterialInstance& host ) const {}
};

///////////////////////////////////////////////////////////////////////////////
