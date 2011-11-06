/// @file   TamyEditor/EditorsDocker.h
/// @brief  an interface that allows to place newly created editors somewhere
#pragma once


///////////////////////////////////////////////////////////////////////////////

class ResourceEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * An interface that allows to place somewhere the newly created editors 
 * the resources browser made.
 */
class EditorsDocker
{
public:
   virtual ~EditorsDocker() {}

   /**
    * Adds a resource editor to the docker.
    *
    * @param editor.
    */
   virtual void addEditor( ResourceEditor* editor ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
