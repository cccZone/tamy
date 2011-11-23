/// @file   TamyEditor/RPSVoid.h
/// @brief  rendering pipeline's void socket representation
#pragma once

#include "RenderingPipelineSocketRepresentation.h"
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RPSVoid : public RenderingPipelineSocketRepresentation
{
   DECLARE_CLASS( RPSVoid )

public:
   /**
    * Default constructor.
    */
   RPSVoid() : RenderingPipelineSocketRepresentation() {}

   /**
    * Constructor.
    *
    * @param socket        socket the node represents.
    */
   RPSVoid( GBNodeSocket& socket ) : RenderingPipelineSocketRepresentation( socket ) {}

protected:
   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   QColor getBgColor() const { return QColor( 224, 224, 166 ); }
};

///////////////////////////////////////////////////////////////////////////////
