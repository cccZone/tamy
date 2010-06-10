#pragma once

/// @file   TamyEditor/EntityManualEditor.h
/// @brief  a common interface for editors that will be operated 
///         from the EditEntityCommand context

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class EditEntityCommand;

///////////////////////////////////////////////////////////////////////////////

/**
 * A common interface for editors that will be operated from 
 * the EditEntityCommand context.
 */
class EntityManualEditor
{
public:
   virtual ~EntityManualEditor() {}

   /**
    * Initializes the editor.
    *
    * @param host
    */
   virtual void initialize( EditEntityCommand& host ) = 0;

   /**
    * Called when the edition mode changes.
    */
   virtual void notifyEditModeChange() = 0;

   /**
    * Called to initialize the edition process.
    *
    * @param valChange
    * @param camera
    */
   virtual void startEdition( const D3DXVECTOR2& initClickPos ) = 0;

   /**
    * Changes the parameter of an entity by the specified value difference,
    * optionally taking the camera into consideration (when it comes
    * to camera-relative transformations such as movement, rotation etc.)
    *
    * @param valChange
    * @param camera
    */
   virtual void edit( const D3DXVECTOR2& valChange ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
