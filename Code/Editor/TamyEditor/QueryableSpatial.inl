#ifndef _QUERYABLE_SPATIAL_H
#error "This file can only be included from QueryableSpatial.h"
#else


///////////////////////////////////////////////////////////////////////////////

template <typename T>
QueryableSpatial<T>::QueryableSpatial(T& entity)
: m_entity(entity)
, m_localVol(m_entity.getGeometry().calculateBoundingVolume())
, m_globalVol(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
QueryableSpatial<T>::~QueryableSpatial()
{
   delete m_globalVol; m_globalVol = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void QueryableSpatial<T>::initialize(SceneQueries& parent)
{
   parent.storage().insert(*this);
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void QueryableSpatial<T>::deinitialize(SceneQueries& parent)
{
   parent.storage().remove(*this);
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const BoundingVolume& QueryableSpatial<T>::getBoundingVolume()
{
   delete m_globalVol;
   m_globalVol = m_localVol * m_entity.getGlobalMtx();
   return *m_globalVol;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const Array<Triangle*>& QueryableSpatial<T>::getBoundingGeometry()
{
   return m_noTriangles;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const D3DXMATRIX& QueryableSpatial<T>::getGlobalMtx()
{
   return m_entity.getGlobalMtx();
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
Entity& QueryableSpatial<T>::getEntity()
{
   return m_entity;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _QUERYABLE_SPATIAL_H
