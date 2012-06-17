#pragma once

/// @file   TamyEditor/SceneQuery.h
/// @brief  an interface describing a single scene query

#include <d3dx9.h>
#include "core/Vector.h"
#include "core/Array.h"
#include "core/types.h"


///////////////////////////////////////////////////////////////////////////////

class Entity;
struct Vector;

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
private:
   uint                    m_resultsCount;
   const uint              m_maxResults;
   Array< Vector >         m_resultBuf;
   Array< uint >           m_resultOwnerId;

public:
   virtual ~SceneQuery() {}

   /**
    * Returns the queried position
    */
   virtual const Vector& getQueriedPosition() const = 0;

   /**
    * Notifies about the query completion.
    *
    * @param resultOwnerId
    */
   void notifyResult( uint resultOwnerId );

   /**
    * Returns the result buffer.
    *
    * @param resultOwnerId
    */
   Vector& getResultBuffer( uint resultOwnerId );

protected:
   SceneQuery();

   /**
    * The manager will call this method to inform that the query's been
    * completed and will pass the results as the parameter.
    *
    * @param foundEntities    pointers to the queried entity ( may contain NULL pointers )
    */
   virtual void setResult( const Array< Entity* >& foundEntities ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
