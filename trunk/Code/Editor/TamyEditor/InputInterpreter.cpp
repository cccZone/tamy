#include "InputInterpreter.h"
#include "core.h"
#include "core-AppFlow.h"
#include "core-Renderer\Renderer.h"
#include "InputCommand.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

InputInterpreter::InputInterpreter(UserInputController& uiController,
                                    Renderer& renderer )
: m_uiController(uiController)
, m_renderer(renderer)
{
}

///////////////////////////////////////////////////////////////////////////////

InputInterpreter::~InputInterpreter()
{
   for ( CommandsMap::iterator it = m_commands.begin();
         it != m_commands.end(); ++it )
   {
      delete it->second;
   }
   m_commands.clear();
}

///////////////////////////////////////////////////////////////////////////////

void InputInterpreter::addCommand( unsigned char keyCode, InputCommand* command )
{
   if ( command == NULL  )
   {
      throw std::invalid_argument( "NULL pointer instead InputCommand instance" );
   }

   CommandsMap::iterator it = m_commands.find( keyCode );
   if ( it != m_commands.end() )
   {
      delete it->second;
      it->second = command;
   }
   else
   {
      m_commands.insert( std::make_pair( keyCode, command ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void InputInterpreter::keySmashed( unsigned char keyCode ) 
{
   CommandsMap::iterator it = m_commands.find( keyCode );
   if ( it == m_commands.end() )
   {
      return;
   }
   Point mousePos = m_uiController.getMousePos();
   D3DXVECTOR2 viewportPos;
   m_renderer.screenToViewport( mousePos, viewportPos );
   it->second->execute( IS_PRESSED, viewportPos );
}

///////////////////////////////////////////////////////////////////////////////

void InputInterpreter::keyHeld( unsigned char keyCode )
{
   CommandsMap::iterator it = m_commands.find( keyCode );
   if ( it == m_commands.end() )
   {
      return;
   }
   Point mousePos = m_uiController.getMousePos();
   D3DXVECTOR2 viewportPos;
   m_renderer.screenToViewport( mousePos, viewportPos );
   it->second->execute( IS_HELD, viewportPos );
}

///////////////////////////////////////////////////////////////////////////////

void InputInterpreter::keyReleased( unsigned char keyCode ) 
{
   CommandsMap::iterator it = m_commands.find( keyCode );
   if ( it == m_commands.end() )
   {
      return;
   }
   Point mousePos = m_uiController.getMousePos();
   D3DXVECTOR2 viewportPos;
   m_renderer.screenToViewport( mousePos, viewportPos );
   it->second->execute( IS_RELEASED, viewportPos );

}

///////////////////////////////////////////////////////////////////////////////
