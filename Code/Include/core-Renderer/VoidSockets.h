/// @file   core-Renderer/VoidSockets.h
/// @brief  rendering pipeline node sockets that don't pass any values.
#pragma once

#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RPVoidInput : public RPNodeInput
{
   DECLARE_CLASS( RPVoidInput )

public:
   /**
    * Constructor.
    */
   RPVoidInput( const std::string& name = "" );
   ~RPVoidInput();

protected:
   bool canConnect( RPNodeOutput& output ) const;
};

///////////////////////////////////////////////////////////////////////////////

class RPVoidOutput : public RPNodeOutput
{
   DECLARE_CLASS( RPVoidOutput )

public:
   /**
    * Constructor.
    */
   RPVoidOutput( const std::string& name = "" );
};

///////////////////////////////////////////////////////////////////////////////
