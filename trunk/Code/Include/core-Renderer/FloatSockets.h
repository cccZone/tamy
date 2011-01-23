/// @file   core-Renderer/FloatSockets.h
/// @brief  rendering pipeline node sockets that pass float primitives
#pragma once

#include "core-Renderer/RenderingPipelineSockets.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RPFloatInput : public RPNodeInput
{
   DECLARE_CLASS( RPFloatInput )

public:
   /**
    * Constructor.
    */
   RPFloatInput( const std::string& name = "" );

   /**
    * Returns the float value exposed by the output the socket's connected to.
    *
    * @param data       runtime data buffer.
    */
   float getValue( RuntimeDataBuffer& data ) const;

protected:
   bool canConnect( RPNodeOutput& output ) const;
};

///////////////////////////////////////////////////////////////////////////////

class RPFloatOutput : public RPNodeOutput
{
   DECLARE_CLASS( RPFloatOutput )

private:
   TRuntimeVar< float >             m_value;

public:
   /**
    * Constructor.
    */
   RPFloatOutput( const std::string& name = "" );

   /**
    * Sets the new value in the socket.
    *
    * @param val
    */
   inline void setValue( RuntimeDataBuffer& data, float val ) const { data[ m_value ] = val; }

   /**
    * Returns the socket's value.
    */
   inline float getValue( RuntimeDataBuffer& data ) const { return data[ m_value ]; }

   // -------------------------------------------------------------------------
   // RPNodeOutput implementation
   // -------------------------------------------------------------------------
   void createLayout( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
