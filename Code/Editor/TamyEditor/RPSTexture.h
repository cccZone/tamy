/// @file   TamyEditor/RPSTexture.h
/// @brief  rendering pipeline's texture socket representation
#pragma once

#include "RenderingPipelineSocketRepresentation.h"
#include "core-Renderer/TextureSockets.h"


///////////////////////////////////////////////////////////////////////////////

class QGraphicsSceneDragDropEvent;
class RPRenderTargetOutput;
class RPImageOutput;
class RPTextureInput;

///////////////////////////////////////////////////////////////////////////////

class RPSTextureInput : public RenderingPipelineSocketRepresentation
{
   DECLARE_CLASS( RPSTextureInput )

public:
   /**
    * Default constructor.
    */
   RPSTextureInput() : RenderingPipelineSocketRepresentation() {}

   /**
    * Constructor.
    *
    * @param socket        socket the node represents.
    */
   RPSTextureInput( RPTextureInput& socket ) : RenderingPipelineSocketRepresentation( socket ) {}

protected:
   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   QColor getBgColor() const { return QColor( 186, 166, 221 ); }
};

///////////////////////////////////////////////////////////////////////////////

class RPSRenderTargetOutput : public RenderingPipelineSocketRepresentation
{
   DECLARE_CLASS( RPSRenderTargetOutput )

private:
   static QPen    s_dragPen;

   bool           m_renderTargetDraggedIn;

public:
   /**
    * Default constructor.
    */
   RPSRenderTargetOutput();

   /**
    * Constructor.
    *
    * @param socket        socket the node represents.
    */
   RPSRenderTargetOutput( RPRenderTargetOutput& socket );

protected:
   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   QColor getBgColor() const { return QColor( 186, 166, 221 ); }
   QPen getBorderPen() const;
   void dragEnterEvent( QGraphicsSceneDragDropEvent *event );
   void dragMoveEvent( QGraphicsSceneDragDropEvent *event );
   void dragLeaveEvent( QGraphicsSceneDragDropEvent *event );
   void dropEvent( QGraphicsSceneDragDropEvent *event );
};

///////////////////////////////////////////////////////////////////////////////

class RPSImageOutput : public RenderingPipelineSocketRepresentation
{
   DECLARE_CLASS( RPSImageOutput )

public:
   /**
    * Default constructor.
    */
   RPSImageOutput() : RenderingPipelineSocketRepresentation() {}

   /**
    * Constructor.
    *
    * @param socket        socket the node represents.
    */
   RPSImageOutput( RPImageOutput& socket ) : RenderingPipelineSocketRepresentation( socket ) {}

protected:
   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   QColor getBgColor() const { return QColor( 186, 166, 221 ); }
};

///////////////////////////////////////////////////////////////////////////////
