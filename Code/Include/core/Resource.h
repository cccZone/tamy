#pragma once

/// @file   core\Resource.h
/// @brief  interface marking all resources


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface marks all resources that can be loaded using 
 * the resources manager.
 */
class Resource
{
public:
   virtual ~Resource() {}

   /**
    * The method is called by the resources manager once
    * the resource has succesfully been registered with the manager.
    */
   virtual void onLoaded(ResourcesManager& mgr) = 0;
};

///////////////////////////////////////////////////////////////////////////////
