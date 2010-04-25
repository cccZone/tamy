#ifndef _LINEAR_STORAGE_H
#error "This file can only be included from LinearStorage.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename T>
LinearStorage<T>::LinearStorage() 
{
   m_container = new Array<T*>();
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
LinearStorage<T>::~LinearStorage()
{
   delete m_container;
   m_container = NULL;
}


///////////////////////////////////////////////////////////////////////////////

template<typename T>
void LinearStorage<T>::insert(T& elem)
{
   m_container->push_back(&elem);
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
void LinearStorage<T>::remove(T& elem)
{
   unsigned int index = m_container->find(&elem);
   m_container->remove(index);
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
void LinearStorage<T>::query(const BoundingVolume& boundingVol, Array<T*>& output) const
{
   unsigned int elemsCount = m_container->size();
   T* elem = NULL;
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      elem = (*m_container)[i];
      if (elem->getBoundingVolume().testCollision(boundingVol) == true)
      {
         output.push_back(elem);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
unsigned int LinearStorage<T>::size() const
{
   return m_container->size();
}

///////////////////////////////////////////////////////////////////////////////

#endif // _LINEAR_STORAGE_H
