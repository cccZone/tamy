/// @file   core-Renderer/IntSockets.h
/// @brief  rendering pipeline node sockets that pass int primitives
#pragma once

#include "core-Renderer/RenderingPipelineSockets.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RPIntInput : public RPNodeInput
{
   DECLARE_CLASS( RPIntInput )

public:
   /**
    * Constructor.
    */
   RPIntInput( const std::string& name = "" );

   /**
    * Returns the int value exposed by the output the socket's connected to.
    *
    * @param data       runtime data buffer.
    */
   int getValue( RuntimeDataBuffer& data ) const;

protected:
   bool canConnect( RPNodeOutput& output ) const;
};

///////////////////////////////////////////////////////////////////////////////

class RPIntOutput : public RPNodeOutput
{
   DECLARE_CLASS( RPIntOutput )

private:
   TRuntimeVar< int >             m_value;

public:
   /**
    * Constructor.
    */
   RPIntOutput( const std::string& name = "" );

   /**
    * Sets the new value in the socket.
    *
    * @param val
    */
   inline void setValue( RuntimeDataBuffer& data, int val ) const { data[ m_value ] = val; }

   /**
    * Returns the socket's value.
    */
   inline int getValue( RuntimeDataBuffer& data ) const { return data[ m_value ]; }

   // -------------------------------------------------------------------------
   // RPNodeOutput implementation
   // -------------------------------------------------------------------------
   void createLayout( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
