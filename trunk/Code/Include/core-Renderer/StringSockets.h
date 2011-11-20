/// @file   core-Renderer/StringSockets.h
/// @brief  rendering pipeline node sockets that pass strings
#pragma once

#include "core-Renderer/RenderingPipelineSockets.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RPStringInput : public RPNodeInput
{
   DECLARE_CLASS( RPStringInput )

public:
   /**
    * Constructor.
    */
   RPStringInput( const std::string& name = "" );

   /**
    * Returns the string value exposed by the output the socket's connected to.
    *
    * @param data       runtime data buffer.
    */
   const std::string& getValue( RuntimeDataBuffer& data ) const;

protected:
   bool canConnect( RPNodeOutput& output ) const;
};

///////////////////////////////////////////////////////////////////////////////

class RPStringOutput : public RPNodeOutput
{
   DECLARE_CLASS( RPStringOutput )

private:
   TRuntimeVar< std::string >             m_value;

public:
   /**
    * Constructor.
    */
   RPStringOutput( const std::string& name = "" );

   /**
    * Sets the new value in the socket.
    *
    * @param val
    */
   inline void setValue( RuntimeDataBuffer& data, const std::string& val ) const { data[ m_value ] = val; }

   /**
    * Returns the socket's value.
    */
   inline const std::string& getValue( RuntimeDataBuffer& data ) const { return data[ m_value ]; }

   // -------------------------------------------------------------------------
   // RPNodeOutput implementation
   // -------------------------------------------------------------------------
   void createLayout( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
