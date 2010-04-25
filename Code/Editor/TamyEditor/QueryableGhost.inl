#ifndef _QUERYABLE_GHOST_H
#error "This file can only be included from QueryableGhost.h"
#else


///////////////////////////////////////////////////////////////////////////////

template <typename T>
QueryableGhost<T>::QueryableGhost(T& entity)
: m_entity(entity)
{
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
QueryableGhost<T>::~QueryableGhost()
{
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void QueryableGhost<T>::initialize(SceneQueries& parent)
{
   parent.storage().insert(*this);
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void QueryableGhost<T>::deinitialize(SceneQueries& parent)
{
   parent.storage().remove(*this);
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const BoundingVolume& QueryableGhost<T>::getBoundingVolume()
{
   const D3DXMATRIX& situation = m_entity.getGlobalMtx();
   m_vol.origin.x = situation._41;
   m_vol.origin.y = situation._42;
   m_vol.origin.z = situation._43;
   return m_vol;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const Array<Triangle*>& QueryableGhost<T>::getBoundingGeometry()
{
   return m_noGeometry;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const D3DXMATRIX& QueryableGhost<T>::getGlobalMtx()
{
   return m_entity.getGlobalMtx();
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
Entity& QueryableGhost<T>::getEntity()
{
   return m_entity;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _QUERYABLE_GHOST_H
