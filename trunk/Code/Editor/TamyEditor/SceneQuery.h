#pragma once

/// @file   TamyEditor/SceneQuery.h
/// @brief  an interface describing a single scene query

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * Asynchronous scene queries require an interface to be registered
 * with the SceneQueries manager - such interface represents a single
 * scene query, provides the manager with the query requirements
 * and is used to return the query results.
 * This is the interface :)
 */
class SceneQuery
{
public:
   virtual ~SceneQuery() {}

   /**
    * Returns the queried position
    */
   virtual const D3DXVECTOR2& getQueriedPosition() const = 0;

   /**
    * The manager will call this method to inform that the query's been
    * completed and will pass the results as the parameter.
    *
    * @param foundEntity   a pointer to the queried entity, or NULL 
    *                      if noting is found
    */
   virtual void setResult( Entity* foundEntity ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
