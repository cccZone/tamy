/// @file   TamyEditor/RenderingPipelineLayout.h
/// @brief  layout for a rendering pipeline
#pragma once

#include "GraphLayout.h"
#include "core/Resource.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Layout for a rendering pipeline
 */
class RenderingPipelineLayout : public Resource, public GraphLayout
{
   DECLARE_RESOURCE( RenderingPipelineLayout )

private:
   Resource*                        m_model;

public:
   /**
    * Constructor.
    *
    * @param path
    */
   RenderingPipelineLayout( const std::string& path = "" );

protected:
   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceSave( ResourcesManager& mgr );
   void onResourceLoaded( ResourcesManager& mgr );
};

///////////////////////////////////////////////////////////////////////////////
