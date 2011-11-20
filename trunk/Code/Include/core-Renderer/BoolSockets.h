/// @file   core-Renderer/BoolSockets.h
/// @brief  rendering pipeline node sockets that pass bool primitives
#pragma once

#include "core-Renderer/RenderingPipelineSockets.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RPBoolInput : public RPNodeInput
{
   DECLARE_CLASS( RPBoolInput )

public:
   /**
    * Constructor.
    */
   RPBoolInput( const std::string& name = "" );

   /**
    * Returns the bool value exposed by the output the socket's connected to.
    *
    * @param data       runtime data buffer.
    */
   bool getValue( RuntimeDataBuffer& data ) const;

protected:
   bool canConnect( RPNodeOutput& output ) const;
};

///////////////////////////////////////////////////////////////////////////////

class RPBoolOutput : public RPNodeOutput
{
   DECLARE_CLASS( RPBoolOutput )

private:
   TRuntimeVar< bool >             m_value;

public:
   /**
    * Constructor.
    */
   RPBoolOutput( const std::string& name = "" );

   /**
    * Sets the new value in the socket.
    *
    * @param val
    */
   inline void setValue( RuntimeDataBuffer& data, bool val ) const { data[ m_value ] = val; }

   /**
    * Returns the socket's value.
    */
   inline bool getValue( RuntimeDataBuffer& data ) const { return data[ m_value ]; }

   // -------------------------------------------------------------------------
   // RPNodeOutput implementation
   // -------------------------------------------------------------------------
   void createLayout( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
