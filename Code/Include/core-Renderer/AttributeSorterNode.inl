#ifndef _ATTRIBUTE_SORTER_NODE_H
#error "This file can only be included from AttributeSorterNode.h"
#else

#include "core-Renderer\DefaultAttributeSorter.h"
#include "core-Renderer\Renderable.h"


///////////////////////////////////////////////////////////////////////////////

template <typename T>
TAttributeSorterNode<T>::TAttributeSorterNode()
: m_topPriorityChild(new DefaultAttributeSorter())
, m_secondaryPriorityChild(new DefaultAttributeSorter())
{
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
TAttributeSorterNode<T>::~TAttributeSorterNode()
{
   delete m_topPriorityChild; m_topPriorityChild = NULL;
   delete m_secondaryPriorityChild; m_secondaryPriorityChild = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void TAttributeSorterNode<T>::setPrimarySorter(AttributeSorter* sorter)
{
   delete m_topPriorityChild;
   if (sorter == NULL)
   {
      m_topPriorityChild = new DefaultAttributeSorter();
   }
   else
   {
      m_topPriorityChild = sorter;
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void TAttributeSorterNode<T>::setSecondarySorter(AttributeSorter* sorter)
{
   delete m_secondaryPriorityChild;
   if (sorter == NULL)
   {
      m_secondaryPriorityChild = new DefaultAttributeSorter();
   }
   else
   {
      m_secondaryPriorityChild = sorter;
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void TAttributeSorterNode<T>::add(Attributed& object)
{
   bool attribFound = false;

   const Renderable::Attributes& attribs = object.getAttributes();
   for (Renderable::Attributes::const_iterator it = attribs.begin();
      (it != attribs.end()) && (attribFound == false); ++it)
   {
      attribFound = (dynamic_cast<T*> (*it) != NULL);
   }

   if (attribFound)
   {
      m_secondaryPriorityChild->add(object);
   }
   else
   {
      m_topPriorityChild->add(object);
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void TAttributeSorterNode<T>::remove(Attributed& object)
{
   bool attribFound = false;

   const Renderable::Attributes& attribs = object.getAttributes();
   for (Renderable::Attributes::const_iterator it = attribs.begin();
      (it != attribs.end()) && (attribFound == false); ++it)
   {
      attribFound = (dynamic_cast<T*> (*it) != NULL);
   }

   if (attribFound)
   {
      m_secondaryPriorityChild->remove(object);
   }
   else
   {
      m_topPriorityChild->remove(object);
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void TAttributeSorterNode<T>::render()
{
   m_topPriorityChild->render();
   m_secondaryPriorityChild->render();
}

///////////////////////////////////////////////////////////////////////////////

#endif // _ATTRIBUTE_SORTER_NODE_H
