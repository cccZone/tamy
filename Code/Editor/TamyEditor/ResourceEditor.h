#pragma once

/// @file   TamyEditor\ResourceEditor.h
/// @brief  common interface for all abstract editors


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Common interface for all abstract editors.
 *
 * CAUTION: ResourceEditors are created by the ResourcesBrowser, however
 * it doesn't manage their lifetime - they need to do it themselves.
 */
class ResourceEditor
{
public:
   virtual ~ResourceEditor() {}

   /**
    * This method will be called once the editor is created to initialize
    * and start it up.
    *
    * @param mgr     main editor and the main  services database.
    */
   virtual void initialize( TamyEditor& mgr ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
