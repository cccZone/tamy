#include "SceneQuery.h"
#include "QueryRenderingPass.h"


///////////////////////////////////////////////////////////////////////////////

SceneQuery::SceneQuery()
   : m_resultsCount( 0 )
   , m_maxResults( 8 )
   , m_resultBuf( m_maxResults )
   , m_resultOwnerId( m_maxResults )
{
   for ( uint i = 0; i < m_maxResults; ++i )
   {
      m_resultOwnerId[i] = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////

Vector& SceneQuery::getResultBuffer( uint resultOwnerId ) 
{ 
   ASSERT_MSG( m_resultsCount < m_maxResults, "Up to 8 scenes may be queried with the same query" );
   Vector& result = m_resultBuf[ m_resultsCount ];
   m_resultOwnerId[m_resultsCount] = resultOwnerId;
   if ( m_resultsCount < m_maxResults )
   {
      m_resultsCount++;
   }
   return result;
}

///////////////////////////////////////////////////////////////////////////////

void SceneQuery::notifyResult( uint resultOwnerId )
{
   // first - locate the owner and reset its query flag. While you're at it, check
   // if there are any more result owner ids set, meaning that there are still some
   // unfinished queries we're waiting for
   bool noMoreQueriesToComplete = true;
   for ( uint i = 0; i < m_resultsCount; ++i )
   {
      if ( m_resultOwnerId[i] == resultOwnerId )
      {
         m_resultOwnerId[i] = 0;
      }
      else if ( m_resultOwnerId[i] > 0 )
      {
         noMoreQueriesToComplete = false;
      }
   }

   // if there are no more owners, it means we can finally share the results
   if ( noMoreQueriesToComplete )
   {
      Array< Entity* > entities( m_resultsCount );
      for ( uint i = 0; i < m_resultsCount; ++i )
      {
         Entity* entity = reinterpret_cast< Entity* >( QueryRenderingPass::vecToPtr( m_resultBuf[i] ) );
         entities.push_back( entity );
      }

      m_resultsCount = 0;
      setResult( entities );
   }
}

///////////////////////////////////////////////////////////////////////////////
