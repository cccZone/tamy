/// @file   core-Renderer/MaterialNode.h
/// @brief  material graph node
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilderNode.h"
#include "core-Renderer/MaterialSockets.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialNode : public ResourceObject, public GraphBuilderNode< MaterialNode >
{
   DECLARE_CLASS( MaterialNode)

public:
   virtual ~MaterialNode();

   /**
    * Creates an input with the specified name of the specified type.
    *
    * @param dataType      what type of data should the input marshal
    * @param name          input name
    */
   GBNodeInput< MaterialNode >* createInput( const Class& dataType, const std::string& name ) const;

   // -----------------------------------------------------------------
   // Object implementation
   // -----------------------------------------------------------------
   void onObjectLoaded();
   void onPropertyChanged( Property& property );
};

///////////////////////////////////////////////////////////////////////////////
