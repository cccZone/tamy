/// @file   core-Renderer/MatrixSockets.h
/// @brief  rendering pipeline node sockets that pass matrices
#pragma once

#include "core-Renderer/RenderingPipelineSockets.h"
#include "core/RuntimeData.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class RPMatrixInput : public RPNodeInput
{
   DECLARE_CLASS( RPMatrixInput )

private:
   static D3DXMATRIX     s_mtxIdentity;

public:
   /**
    * Constructor.
    */
   RPMatrixInput( const std::string& name = "" );

   /**
    * Returns the matrix value exposed by the output the socket's connected to.
    *
    * @param data       runtime data buffer.
    */
   const D3DXMATRIX& getValue( RuntimeDataBuffer& data ) const;

protected:
   bool canConnect( RPNodeOutput& output ) const;
};

///////////////////////////////////////////////////////////////////////////////

class RPMatrixOutput : public RPNodeOutput
{
   DECLARE_CLASS( RPMatrixOutput )

private:
   static D3DXMATRIX                      s_mtxIdentity;

   TRuntimeVar< D3DXMATRIX >              m_value;

public:
   /**
    * Constructor.
    */
   RPMatrixOutput( const std::string& name = "" );

   /**
    * Sets the new value in the socket.
    *
    * @param val
    */
   inline void setValue( RuntimeDataBuffer& data, const D3DXMATRIX& val ) const { data[ m_value ] = val; }

   /**
    * Returns the socket's value.
    */
   inline const D3DXMATRIX& getValue( RuntimeDataBuffer& data ) const { return data[ m_value ]; }

   // -------------------------------------------------------------------------
   // RPNodeOutput implementation
   // -------------------------------------------------------------------------
   void createLayout( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
