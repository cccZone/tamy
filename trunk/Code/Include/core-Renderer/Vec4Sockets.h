/// @file   core-Renderer/Vec4Sockets.h
/// @brief  rendering pipeline node sockets that pass 4-element vectors
#pragma once

#include "core-Renderer/RenderingPipelineSockets.h"
#include "core/RuntimeData.h"
#include  <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class RPVec4Input : public RPNodeInput
{
   DECLARE_CLASS( RPVec4Input )

public:
   /**
    * Constructor.
    */
   RPVec4Input( const std::string& name = "" );

   /**
    * Returns the int value exposed by the output the socket's connected to.
    *
    * @param data       runtime data buffer.
    */
   const D3DXVECTOR4& getValue( RuntimeDataBuffer& data ) const;

protected:
   bool canConnect( RPNodeOutput& output ) const;
};

///////////////////////////////////////////////////////////////////////////////

class RPVec4Output : public RPNodeOutput
{
   DECLARE_CLASS( RPVec4Output )

private:
   TRuntimeVar< D3DXVECTOR4 >             m_value;

public:
   /**
    * Constructor.
    */
   RPVec4Output( const std::string& name = "" );

   /**
    * Sets the new value in the socket.
    *
    * @param val
    */
   inline void setValue( RuntimeDataBuffer& data, const D3DXVECTOR4& val ) const { data[ m_value ] = val; }

   /**
    * Returns the socket's value.
    */
   inline const D3DXVECTOR4& getValue( RuntimeDataBuffer& data ) const { return data[ m_value ]; }

   // -------------------------------------------------------------------------
   // RPNodeOutput implementation
   // -------------------------------------------------------------------------
   void createLayout( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
