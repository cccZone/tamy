/// @file      TamyEditor/GraphBlock.h
/// @brief     a graphical representation of a graph node
#ifndef _GRAPH_BLOCK_H
#define _GRAPH_BLOCK_H

#include "core/ReflectionObject.h"
#include "core/GenericFactory.h"
#include "core/GraphBuilderSockets.h"
#include "core/GraphBuilderNode.h"
#include <QGraphicsProxyWidget>
#include <QGraphicsItem>
#include <QColor>
#include <QRectF>
#include <QPen>
#include <QFont>
#include <set>


///////////////////////////////////////////////////////////////////////////////

class QPainter;
class QGraphicsScene;
class GraphBlockSocket;
enum GraphBlockSocketPosition;

///////////////////////////////////////////////////////////////////////////////

/**
 * A graphical representation of a graph node.
 */
class GraphBlock : public ReflectionObject, public QGraphicsItem
{
   DECLARE_CLASS() 

protected:
   // sockets
   typedef GenericFactory< GBNodeSocket, GraphBlockSocket >   SocketsFactory;

   std::vector< GraphBlockSocket* >    m_sockets;
   SocketsFactory*                     m_socketsFactory;

   unsigned int                        m_leftSocketsCount;
   unsigned int                        m_rightSocketsCount;

private:
   std::string                         m_caption;

   QPointF                             m_position;
   QRectF                              m_bounds;
   QRectF                              m_captionBounds;
   QRectF                              m_totalBounds;

   QFont                               m_font;

   QGraphicsProxyWidget*               m_centralWidget;
   QWidget*                            m_embeddedWidget;

public:
   /**
    * Constructor.
    */
   GraphBlock();
   virtual ~GraphBlock();

   /**
    * Sets the caption of the block.h
    *
    * @param caption
    */
   void setCaption( const char* caption );

   /**
    * Sets an instance of sockets factory this block should use to create its sockets
    */
   void setSocketsFactory( SocketsFactory* socketsFactory );

   /**
    * Sets a widget in the center of the block.
    *
    * @param widget
    */
   void setCentralWidget( QWidget* widget );

   /**
    * Checks if the block overlaps the specified position.
    * 
    * @param pos
    */
   bool doesOverlap( const QPointF& pos ) const;

   /**
    * Returns the represented node instance.
    */
   virtual ReflectionObject* getNode() { return NULL; }

   /**
    * Looks for a socket with the specified name and position.
    *
    * @param position
    * @param name
    */
   GraphBlockSocket* getSocket( GraphBlockSocketPosition position, const std::string& name ) const;

   /**
    * Extracts a list of all socket names located on the specified position.
    *
    * @param position
    * @param outSocketNames
    */
   void getAllSockets( GraphBlockSocketPosition position, std::set< std::string >& outSocketNames ) const;

   /**
    * Gives access to the array of sockets
    */
   const std::vector< GraphBlockSocket* >& accessSockets() const { return m_sockets; }

   /**
    * Override this method in order to provide an implementation-specific context menu for the block.
    *
    * @param menu
    */
   virtual void onCreateContextMenu( QMenu* menu ) {}

   /**
    * Called in order to initialize the block.
    */
   virtual void initialize() = 0;

   // -------------------------------------------------------------------------
   // QGraphicsItem implementation
   // -------------------------------------------------------------------------
   inline QRectF boundingRect() const { return m_totalBounds; }
   void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );
   void mouseMoveEvent( QGraphicsSceneMouseEvent* event );

   // -------------------------------------------------------------------------
   // ReflectionObject implementation
   // -------------------------------------------------------------------------
   void onPropertyChanged( ReflectionProperty& property );

   // -------------------------------------------------------------------------
   // Block layout operation
   // -------------------------------------------------------------------------
   /**
    * Forces the block to recalculate its layout
    */
   void calculateBounds();

protected:
   // -------------------------------------------------------------------------
   // ReflectionObject implementation
   // -------------------------------------------------------------------------
   void onObjectPreSave();
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // Block layout settings
   // -------------------------------------------------------------------------
   /**
    * Adds a new socket to the block.
    *
    * @param socket
    */
   void addSocket( GraphBlockSocket* socket );

   /**
    * Removes a single socket on the specified position with the specified name, deleting its instance 
    * and getting rid of any connections that use them.
    *
    * @param position
    * @param socketName
    */
   void removeSocket( GraphBlockSocketPosition position, const std::string& socketName );

   /**
    * Removes sockets on the specified position with the specified names in bulk, deleting their instances
    * and getting rid of any connections that use them.
    *
    * @param position
    * @param socketNames
    */
   void removeSockets( GraphBlockSocketPosition position, const std::set< std::string >& socketNames );

private:
   void removeSingleSocket( GraphBlockSocketPosition position, const std::string& socketName );
};

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
class TGraphBlock : public GraphBlock, public Observer< typename TBaseNode, GraphBuilderNodeOperation >
{
protected:
   TNode*                                                m_nodePtr;
   TResourceHandle< typename TNode >*                    m_node;
   
public:
   /**
    * Default constructor.
    */
   TGraphBlock();

   /**
    * Constructor.
    *
    * @param node       represented node
    */
   TGraphBlock( TNode& node );
   virtual ~TGraphBlock();

   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   void initialize();
   ReflectionObject* getNode();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // Observer implementation
   // -------------------------------------------------------------------------
   void update( TBaseNode& subject );
   void update( TBaseNode& subject, const GraphBuilderNodeOperation& msg );

protected:
   /**
    * Refreshes the block's inputs.
    */
   void refreshInputs( TBaseNode& node );

   /**
    * Refreshes the block's outputs.
    */
   void refreshOutputs( TBaseNode& node );
   
private:
   void initSocketsFactory();
};

///////////////////////////////////////////////////////////////////////////////

#include "GraphBlock.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BLOCK_H
