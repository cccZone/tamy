/// @file   TamyEditor/RPSFloat.h
/// @brief  rendering pipeline's float socket representation
#pragma once

#include "RenderingPipelineSocketRepresentation.h"
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RPSFloat : public RenderingPipelineSocketRepresentation
{
   DECLARE_CLASS( RPSFloat )

public:
   /**
    * Default constructor.
    */
   RPSFloat() : RenderingPipelineSocketRepresentation() {}

   /**
    * Constructor.
    *
    * @param socket        socket the node represents.
    */
   RPSFloat( GBNodeSocket& socket ) : RenderingPipelineSocketRepresentation( socket ) {}

protected:
   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   QColor getBgColor() const { return QColor( 163, 255, 247 ); }
};

///////////////////////////////////////////////////////////////////////////////
