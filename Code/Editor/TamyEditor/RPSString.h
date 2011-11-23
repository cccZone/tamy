/// @file   TamyEditor/RPSString.h
/// @brief  rendering pipeline's string socket representation
#pragma once

#include "RenderingPipelineSocketRepresentation.h"
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RPSString : public RenderingPipelineSocketRepresentation
{
   DECLARE_CLASS( RPSString )

public:
   /**
    * Default constructor.
    */
   RPSString() : RenderingPipelineSocketRepresentation() {}

   /**
    * Constructor.
    *
    * @param socket        socket the node represents.
    */
   RPSString( GBNodeSocket& socket ) : RenderingPipelineSocketRepresentation( socket ) {}

protected:
   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   QColor getBgColor() const { return QColor( 255, 209, 132 ); }
};

///////////////////////////////////////////////////////////////////////////////
