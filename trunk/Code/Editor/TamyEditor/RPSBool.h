/// @file   TamyEditor/RPSBool.h
/// @brief  rendering pipeline's bool socket representation
#pragma once

#include "RenderingPipelineSocketRepresentation.h"
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RPSBool : public RenderingPipelineSocketRepresentation
{
   DECLARE_CLASS( RPSBool )

public:
   /**
    * Default constructor.
    */
   RPSBool() : RenderingPipelineSocketRepresentation() {}

   /**
    * Constructor.
    *
    * @param socket        socket the node represents.
    */
   RPSBool( RPNodeSocket& socket ) : RenderingPipelineSocketRepresentation( socket ) {}

protected:
   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   QColor getBgColor() const { return QColor( 186, 211, 255 ); }
};

///////////////////////////////////////////////////////////////////////////////
