#pragma once

/// @file   TamyEngine\SceneQueriesModelRepresentation.h
/// @brief  interface for model representations required by component


///////////////////////////////////////////////////////////////////////////////

class SceneQueries;

///////////////////////////////////////////////////////////////////////////////

/**
 * Interface for model representations required by component.
 */
class SceneQueriesModelRepresentation
{
public:
   virtual ~SceneQueriesModelRepresentation() {}

   /**
    * This method allows to initialize a newly created representation.
    *
    * @param parent     scene query component that spawned this representation
    */
   virtual void initialize(SceneQueries& parent) = 0;

   /**
    * This method allows to deinitialize a representation that's
    * about to be removed from a view.
    *
    * @param parent     scene query component that spawned this representation
    */
   virtual void deinitialize(SceneQueries& parent) = 0;

};

///////////////////////////////////////////////////////////////////////////////
