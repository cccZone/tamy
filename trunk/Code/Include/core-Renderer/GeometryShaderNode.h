/// @file   core-Renderer/GeometryShaderNode.h
/// @brief  geometry shader graph node
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilderNode.h"
#include "core-Renderer/GeometrySockets.h"


///////////////////////////////////////////////////////////////////////////////

class GeometryEntity;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

class GeometryShaderNode : public ResourceObject, public GraphBuilderNode< GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   virtual ~GeometryShaderNode();

   /**
    * Creates an input with the specified name of the specified type.
    *
    * @param dataType      what type of data should the input marshal
    * @param name          input name
    */
   GBNodeInput< GeometryShaderNode >* createInput( const ReflectionType& dataType, const std::string& name ) const;

   /**
    * Called in order to initialize node's runtime data layout.
    *
    * @param host
    */
   void createLayout( const GeometryEntity& host ) const;

   /**
    * Called when the graph is fully restored. Allows to reprocess certain nodes - remove
    * unused sockets etc. and get the graph structure up to date with external
    * dependencies in general.
    */
   virtual void onGraphLoaded() {}

   /**
    * Called by the entity that uses the material in order to set it up.
    *
    * @param renderer
    * @param entity
    */
   virtual void preRender( Renderer& renderer, const GeometryEntity& entity ) const {}

   /**
    * Called by the entity that uses the material in order to clean up after using it.
    *
    * @param renderer
    * @param entity
    */
   virtual void postRender( Renderer& renderer, const GeometryEntity& entity ) const {}

   // -----------------------------------------------------------------
   // Object implementation
   // -----------------------------------------------------------------
   void onObjectLoaded();
   void onPropertyChanged( ReflectionProperty& property );

protected:
   /**
    * Called in order to initialize node implementation's specific runtime data layout.
    *
    * @param host
    */
   virtual void onCreateLayout( const GeometryEntity& host ) const {}
};

///////////////////////////////////////////////////////////////////////////////
