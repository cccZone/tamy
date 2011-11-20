/// @file   TamyEditor/RPSMatrix.h
/// @brief  rendering pipeline's matrix socket representation
#pragma once

#include "RenderingPipelineSocketRepresentation.h"
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RPSMatrix : public RenderingPipelineSocketRepresentation
{
   DECLARE_CLASS( RPSMatrix )

public:
   /**
    * Default constructor.
    */
   RPSMatrix() : RenderingPipelineSocketRepresentation() {}

   /**
    * Constructor.
    *
    * @param socket        socket the node represents.
    */
   RPSMatrix( RPNodeSocket& socket ) : RenderingPipelineSocketRepresentation( socket ) {}

protected:
   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   QColor getBgColor() const { return QColor( 159, 211, 55 ); }
};

///////////////////////////////////////////////////////////////////////////////
