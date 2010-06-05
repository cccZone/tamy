#pragma once

/// @file   TamyEditor\InputController.h
/// @brief  interpreter that translates the user's input into commands

#include "core-AppFlow.h"
#include <map>


///////////////////////////////////////////////////////////////////////////////

class CTimer;
class UserInputController;
class InputCommand;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Interpreter that translates the user's input into commands.
 */
class InputInterpreter: public KeyStatusHandler
{
private:
   typedef std::map< unsigned char, InputCommand* > CommandsMap;

private:
   UserInputController&             m_uiController;
   Renderer&                        m_renderer;

   // commands
   CommandsMap                      m_commands;

public:
   /**
    * Constructor.
    *
    * @param uiController        user input receiver
    * @param renderer            active renderer
    */
   InputInterpreter( UserInputController& uiController,
                     Renderer& renderer );
   ~InputInterpreter();

   // -------------------------------------------------------------------------
   // Commands management
   // -------------------------------------------------------------------------
   /**
    * Adds a new command.
    *
    * @param keyCode    key which when pressed will toggle the command execution
    * @param command    command to execute
    */
   void addCommand( unsigned char keyCode, InputCommand* command );

   // -------------------------------------------------------------------------
   // KeyStatusHandler implementation
   // -------------------------------------------------------------------------
   void keySmashed( unsigned char keyCode );
   void keyHeld( unsigned char keyCode );
   void keyReleased( unsigned char keyCode );
};

///////////////////////////////////////////////////////////////////////////////
