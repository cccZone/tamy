/// @file   core-AppFlow\KeysStatusModel.h
/// @brief  model containing status of input keys (keyboard & mouse)
#pragma once


///////////////////////////////////////////////////////////////////////////////

/**
 * This model contains the status of input keys of mouse & keyboard.
 */
class KeysStatusModel
{
public:
   virtual ~KeysStatusModel() {}

   /**
    * Returns the status of a specified key.
    *
    * @param keyCode    code of the key we want to check
    * @return           'true' if the key is pressed, 'false' if it's released
    */
   virtual bool isKeyPressed( unsigned char keyCode ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
