#ifndef _LEAF_ATTRIBUTE_SORTER_H
#error "This file can only be included from LeafAttributeSorter.h"
#else

#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

template <typename T>
LeafAttributeSorter<T>::~LeafAttributeSorter()
{
   unsigned int count = m_comparables.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_comparables[i];
   }
   m_comparables.clear();
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void LeafAttributeSorter<T>::add(Attributed& object)
{
   const Attributes& attribs = object.getAttributes();
   for ( Attributes::const_iterator it = attribs.begin(); it != attribs.end(); ++it)
   {
      T* requiredAttrib = dynamic_cast<T*> (*it);
      if (requiredAttrib != NULL)
      {
         m_comparables.push_back(new Comparable(*requiredAttrib, object));
      }
   }

   std::sort(m_comparables.begin(), m_comparables.end(), m_comparator);
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void LeafAttributeSorter<T>::remove(Attributed& object)
{
   for (ComparablesVec::iterator it = m_comparables.begin();
      it != m_comparables.end(); ++it)
   {
      if (&((*it)->object) == &object)
      {
         m_comparables.erase(it);
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void LeafAttributeSorter<T>::render()
{
   unsigned int count = m_comparables.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_comparables[i]->object.render();
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _LEAF_ATTRIBUTE_SORTER_H
