/// @file   core-Renderer/FloatSockets.h
/// @brief  rendering pipeline node sockets that pass float primitives
#pragma once

#include "core-Renderer/RenderingPipelineSockets.h"


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
    */
   float getValue();

protected:
   bool canConnect( RPNodeOutput& output ) const;
};

///////////////////////////////////////////////////////////////////////////////

class RPFloatOutput : public RPNodeOutput
{
   DECLARE_CLASS( RPFloatOutput )

private:
   float             m_value;

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
   inline void setValue( float val ) { m_value = val; }

   /**
    * Returns the socket's value.
    */
   inline float getValue() const { return m_value; }
};

///////////////////////////////////////////////////////////////////////////////
