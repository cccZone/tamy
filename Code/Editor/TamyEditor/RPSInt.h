/// @file   TamyEditor/RPSInt.h
/// @brief  rendering pipeline's int socket representation
#pragma once

#include "RenderingPipelineSocketRepresentation.h"
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RPSInt : public RenderingPipelineSocketRepresentation
{
   DECLARE_CLASS( RPSInt )

public:
   /**
    * Default constructor.
    */
   RPSInt() : RenderingPipelineSocketRepresentation() {}

   /**
    * Constructor.
    *
    * @param socket        socket the node represents.
    */
   RPSInt( GBNodeSocket& socket ) : RenderingPipelineSocketRepresentation( socket ) {}

protected:
   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   QColor getBgColor() const { return QColor( 255, 198, 237 ); }
};

///////////////////////////////////////////////////////////////////////////////
