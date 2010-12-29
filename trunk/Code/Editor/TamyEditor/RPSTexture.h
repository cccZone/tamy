/// @file   TamyEditor/RPSTexture.h
/// @brief  rendering pipeline's texture socket representation
#pragma once

#include "RenderingPipelineSocketRepresentation.h"
#include "core-Renderer/TextureSockets.h"


///////////////////////////////////////////////////////////////////////////////

class QGraphicsSceneDragDropEvent;
class RPTextureOutput;
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

class RPSTextureOutput : public RenderingPipelineSocketRepresentation
{
   DECLARE_CLASS( RPSTextureOutput )

private:
   static QPen    s_dragPen;

   bool           m_renderTargetDraggedIn;

public:
   /**
    * Default constructor.
    */
   RPSTextureOutput();

   /**
    * Constructor.
    *
    * @param socket        socket the node represents.
    */
   RPSTextureOutput( RPTextureOutput& socket );

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
