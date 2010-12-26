/// @file   TamyEditor/RPBPostProcess.h
/// @brief  rendering pipeline block representing a single post process stage
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPPostProcessNode;

///////////////////////////////////////////////////////////////////////////////

class RPBPostProcess : public RenderingPipelineBlock
{
   DECLARE_CLASS( RPBPostProcess )

private:
   RPPostProcessNode*                           m_nodePtr;
   TResourceHandle< RPPostProcessNode >*        m_node;

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBPostProcess() : m_nodePtr( NULL ), m_node( NULL ) {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBPostProcess( RPPostProcessNode& node );
   ~RPBPostProcess();

   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   Object& getNode();
   void initialize( RenderingPipeline& parentResource );

protected:
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_RECTANGLE; }
   virtual QColor getBgColor() const { return QColor( 224, 166, 175 ); }
};

///////////////////////////////////////////////////////////////////////////////
