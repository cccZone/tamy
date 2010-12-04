/// @file   TamyEditor/RPBPostProcess.h
/// @brief  rendering pipeline block representing a single post process stage
#pragma once

#include "GraphBlock.h"


///////////////////////////////////////////////////////////////////////////////

class RPPostProcessNode;

///////////////////////////////////////////////////////////////////////////////

class RPBPostProcess : public GraphBlock
{
   DECLARE_CLASS( RPBPostProcess )

private:
   RPPostProcessNode*         m_node;

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBPostProcess() : m_node( NULL ) {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBPostProcess( RPPostProcessNode& node );

   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   Object* getNode() const;

protected:
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_RECTANGLE; }
   virtual QColor getBgColor() const { return QColor( 255, 100, 0 ); }
};

///////////////////////////////////////////////////////////////////////////////
