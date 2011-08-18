#include "SceneQuery.h"
#include "SceneQueries.h"


///////////////////////////////////////////////////////////////////////////////

SceneQuery::SceneQuery()
   : m_resultBuf( 0, 0, 0, 0 )
{

}

///////////////////////////////////////////////////////////////////////////////

void SceneQuery::notifyResult()
{
   Entity* entity = reinterpret_cast< Entity* >( SceneQueries::vecToPtr( (D3DXVECTOR4)m_resultBuf ) );
   m_resultBuf = Color( 0, 0, 0, 0 );
   setResult( entity );
}

///////////////////////////////////////////////////////////////////////////////
