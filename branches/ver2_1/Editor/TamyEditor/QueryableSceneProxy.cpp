#include "QueryableSceneProxy.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class NullQuerableScene : public QueryableScene
   {
   public:
      void query(const BoundingVolume& boundingVol, 
                 Array<SpatiallyQueryable*>& output) const
      {}

      void insert(SpatiallyQueryable& entity) {}

      void remove(SpatiallyQueryable& entity) {}
   };
} // anonymous

///////////////////////////////////////////////////////////////////////////////

QueryableSceneProxy::QueryableSceneProxy()
: m_defaultImpl(new NullQuerableScene())
, m_currentImpl(m_defaultImpl)
{
}

///////////////////////////////////////////////////////////////////////////////

QueryableSceneProxy::~QueryableSceneProxy()
{
   m_currentImpl = NULL;
   delete m_defaultImpl; m_defaultImpl = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void QueryableSceneProxy::plugin(QueryableScene* scene)
{
   if (m_currentImpl != m_defaultImpl)
   {
      delete m_currentImpl;
   }

   if (scene == NULL)
   {
      m_currentImpl = m_defaultImpl;
   }
   else
   {
      m_currentImpl = scene;
   }
}

///////////////////////////////////////////////////////////////////////////////

void QueryableSceneProxy::query(const BoundingVolume& boundingVol, 
              Array<SpatiallyQueryable*>& output) const
{
   m_currentImpl->query(boundingVol, output);
}

///////////////////////////////////////////////////////////////////////////////

void QueryableSceneProxy::insert(SpatiallyQueryable& entity)
{
   m_currentImpl->insert(entity);
}

///////////////////////////////////////////////////////////////////////////////

void QueryableSceneProxy::remove(SpatiallyQueryable& entity)
{
   m_currentImpl->remove(entity);
}

///////////////////////////////////////////////////////////////////////////////
