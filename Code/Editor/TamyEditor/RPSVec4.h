/// @file   TamyEditor/RPSVec4.h
/// @brief  rendering pipeline's 4-element float vector socket representation
#pragma once

#include "RenderingPipelineSocketRepresentation.h"
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RPSVec4 : public RenderingPipelineSocketRepresentation
{
   DECLARE_CLASS( RPSVec4 )

public:
   /**
    * Default constructor.
    */
   RPSVec4() : RenderingPipelineSocketRepresentation() {}

   /**
    * Constructor.
    *
    * @param socket        socket the node represents.
    */
   RPSVec4( RPNodeSocket& socket ) : RenderingPipelineSocketRepresentation( socket ) {}

protected:
   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   QColor getBgColor() const { return QColor( 219, 255, 175 ); }
};

///////////////////////////////////////////////////////////////////////////////
