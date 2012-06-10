/// @file      TamyEditor/GraphBlock.h
/// @brief     a graphical representation of a graph node
#pragma once

#include <QGraphicsItem>
#include <QColor>
#include <QRectF>
#include <QPen>
#include <QFont>
#include <QGraphicsLineItem>
#include "core/ReflectionObject.h"
#include <set>


///////////////////////////////////////////////////////////////////////////////

class QPainter;
class QGraphicsScene;
class GraphBlockSocket;
class GraphBlockConnection;

///////////////////////////////////////////////////////////////////////////////

enum GraphBlockSocketPosition
{
   GBSP_INPUT,
   GBSP_OUTPUT,
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A graphical representation of a graph node.
 */
class GraphBlock : public ReflectionObject, public QGraphicsItem
{
   DECLARE_CLASS() 

public:
   enum Shape
   {
      GBS_RECTANGLE,
      GBS_CIRCLE,
      GBS_ROUNDED,
   };

protected:
   // sockets
   std::vector< GraphBlockSocket* >    m_sockets;
   unsigned int                        m_leftSocketsCount;
   unsigned int                        m_rightSocketsCount;

private:
   std::string                         m_caption;

   QPointF                             m_position;
   QRectF                              m_bounds;
   QRectF                              m_captionBounds;
   QRectF                              m_totalBounds;

   static QPen                         s_textPen;
   static QPen                         s_borderPen;
   static QPen                         s_selectionPen;
   QFont                               m_font;

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
    * Checks if the block overlaps the specified position.
    * 
    * @param pos
    */
   bool doesOverlap( const QPointF& pos ) const;

   /**
    * Returns the represented node instance.
    */
   virtual ReflectionObject& getNode() { return *( reinterpret_cast< ReflectionObject* >( NULL ) ); }

   /**
    * Returns a list of connections the block's involved in.
    *
    * @param outConnections
    */
   void getConnections( std::vector< GraphBlockConnection* >& outConnections ) const;

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
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_RECTANGLE; }

   /**
    * Returns the color of the block background.
    */
   virtual QColor getBgColor() const { return QColor( 255, 255, 255 ); }

   /**
    * Returns the pen used to draw the border.
    */
   virtual QPen getBorderPen() const { return isSelected() ? s_selectionPen : s_borderPen; }

   /**
    * Adds a new socket to the block.
    *
    * @param socket
    */
   void addSocket( GraphBlockSocket* socket );

   /**
    * Removes a socket on the specified position with the specified name, deleting its instance.
    *
    * @param position
    * @param socketNames
    */
   void removeSockets( GraphBlockSocketPosition position, const std::set< std::string >& socketNames );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A single socket placed on a graph block.
 *
 * Each socket maintains a list of all the connections it participates
 * in so that when it changes it's position, it can inform the connection
 * that it needs to recalculate its bounds.
 * 
 * The socket is responsible for notifying a block that a new connection
 * has been established - providing it's the source socket.
 * The responsibility of a destination socket is just to inform the connection
 * about the changes in its whereabouts.
 */
class GraphBlockSocket : public ReflectionObject, public QGraphicsItem
{
   DECLARE_CLASS()


   friend class GraphBlockConnection;

private:
   GraphBlock*                               m_parent;

   GraphBlockSocketPosition                  m_blockSide;
   std::string                               m_name;

   std::vector< GraphBlockConnection* >      m_connections;

   static QPen                               s_borderPen;
   QFont                                     m_font;
   QRectF                                    m_bounds;
   QRectF                                    m_nameBounds;
   QRectF                                    m_totalBounds;
   QPointF                                   m_position;

public:
   /**
    * Constructor.
    */
   GraphBlockSocket();

   /**
    * Initializes the socket
    *
    * @param parent  parent block
    * @param pos     socket's position ( left or right )
    * @param name    socket's name
    */
   void initialize( GraphBlock* parent, GraphBlockSocketPosition pos, const char* name );

   /**
    * Returns the parent graph block.
    */
   inline GraphBlock& getParentBlock() const { return *m_parent; }

   /**
    * Returns the socket's name.
    */
   inline const std::string& getName() const { return m_name; }

   /**
    * Returns the current socket position.
    */
   inline GraphBlockSocketPosition getPosition() const { return m_blockSide; }

   /**
    * Returns the width of the socket's name.
    */
   float getNameWidth() const;

   /**
    * Returns the represented socket instance.
    */
   virtual ReflectionObject& getSocket() { return *( reinterpret_cast< ReflectionObject* >( NULL ) ); }

   /**
    * Calculate the bounds of the socket.
    */
   void calculateBounds();

   /**
    * Recalculates the bounds of the connections this socket is involved in.
    */
   void calculateConnectionBounds();

   // -------------------------------------------------------------------------
   // Interface for connections management
   // -------------------------------------------------------------------------
   /**
    * Adds a new connection between this socket and another one.
    *
    * @param connection
    *
    * @return        the result of adding a connection
    */
   bool addConnection( GraphBlockConnection& connection );

   /**
    * Removes the specified connection from the socket.
    *
    * @param connection
    */
   void removeConnection( GraphBlockConnection& connection );

   /**
    * Checks the connection between this socket and the specified one.
    *
    * @param socket     socket a connection to which we want to confirm.
    */
   bool isConnectedTo( GraphBlockSocket& socket ) const;

   /**
    * Returns a list of all connections the socket has.
    */
   inline const std::vector< GraphBlockConnection* >& getConnections() const { return m_connections; }

   // -------------------------------------------------------------------------
   // QGraphicsItem implementation
   // -------------------------------------------------------------------------
   QRectF boundingRect() const { return m_totalBounds; }
   void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

protected:
   /**
    * Notification called when a new connection is added.
    *
    * @param connection.
    */
   virtual bool onConnectionAdded( GraphBlockConnection& connection ) { return false; }

   /**
    * Notification called when an existing connection is removed.
    *
    * @param connection.
    */
   virtual void onConnectionRemoved( GraphBlockConnection& connection ) {}

   /**
    * Returns the background color the socket should be drawn with.
    */
   virtual QColor getBgColor() const { return QColor( 255, 255, 255 ); }

   /**
    * Returns the p[en the socket's frame should be drawn with
    */
   virtual QPen getBorderPen() const { return s_borderPen; }

   // -------------------------------------------------------------------------
   // QGraphicsItem implementation
   // -------------------------------------------------------------------------
   void mousePressEvent( QGraphicsSceneMouseEvent* event );
   void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectPreSave();
   void onObjectLoaded();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Connection between two graph block sockets.
 *
 * The connection manages the information kept in the participating socket
 * (it's sort of a mediator in this context).
 */
class GraphBlockConnection : public ReflectionObject, public QGraphicsLineItem
{
   DECLARE_CLASS()

private:
   GraphBlockSocket*       m_source;
   GraphBlockSocket*       m_destination;


public:
   /**
    * Constructor.
    *
    * @param source           source socket
    * @param destination      destination socket
    */
   GraphBlockConnection( GraphBlockSocket* source = NULL, GraphBlockSocket* destination = NULL );

   /**
    * Creates a new connection between two sockets, if that's possible.
    *
    * @param source
    * @param destination
    */
   static GraphBlockConnection* createConnection( GraphBlockSocket* source, GraphBlockSocket* destination );

   /**
    * Calculates the bounds of the connection.
    */
   void calculateBounds();

   /**
    * Called when the connection gets removed.
    */
   void onRemoved();

   /**
    * Returns the source socket.
    */
   inline GraphBlockSocket& getSource() { return *m_source; }

   /**
    * Returns the destination socket.
    */
   inline GraphBlockSocket& getDestination() { return *m_destination; }

protected:
   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
};

///////////////////////////////////////////////////////////////////////////////
