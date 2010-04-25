#ifndef _T_NODES_SPATIAL_STORAGE_H
#error "This file can only be included from TNodesSpatialStorage.h"
#else

#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template<typename NodeType, template<class> class ConcreteSpatialStorage>
TNodesSpatialStorage<NodeType, ConcreteSpatialStorage>::TNodesSpatialStorage(
                                    ConcreteSpatialStorage<NodeType>* storage)
: m_root(new Node("RootNode"))
, m_storage(storage)
, m_isAdding(false)
{
   m_root->attachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

template<typename NodeType, template<class> class ConcreteSpatialStorage>
TNodesSpatialStorage<NodeType, ConcreteSpatialStorage>::~TNodesSpatialStorage() 
{
   m_root->detachObserver(*this);

   delete m_root;
   m_root = NULL;

   delete m_storage;
   m_storage = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template<typename NodeType, template<class> class ConcreteSpatialStorage>
void TNodesSpatialStorage<NodeType, ConcreteSpatialStorage>::addNode(Node* node)
{
   m_root->addChild(node);
}

///////////////////////////////////////////////////////////////////////////////

template<typename NodeType, template<class> class ConcreteSpatialStorage>
void TNodesSpatialStorage<NodeType, ConcreteSpatialStorage>::removeNode(Node& node)
{
   m_root->removeChild(node);
}

///////////////////////////////////////////////////////////////////////////////

template<typename NodeType, template<class> class ConcreteSpatialStorage>
void TNodesSpatialStorage<NodeType, ConcreteSpatialStorage>::visit(NodeType& node)
{
   if (m_isAdding)
   {
      m_storage->insert(node);
   }
   else
   {
      m_storage->remove(node);
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename NodeType, template<class> class ConcreteSpatialStorage>
void TNodesSpatialStorage<NodeType, ConcreteSpatialStorage>::query(
                                                  const BoundingVolume& volume, 
                                                  Array<NodeType*>& output) const
{
   m_storage->query(volume, output);
}

///////////////////////////////////////////////////////////////////////////////

template<typename NodeType, template<class> class ConcreteSpatialStorage>
Node& TNodesSpatialStorage<NodeType, ConcreteSpatialStorage>::root()
{
   return *m_root;
}

///////////////////////////////////////////////////////////////////////////////

template<typename NodeType, template<class> class ConcreteSpatialStorage>
void TNodesSpatialStorage<NodeType, ConcreteSpatialStorage>::childAdded(
                                                                Node& parent, 
                                                                Node& child)
{
   m_isAdding = true;
   child.accept(*this);

   child.attachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

template<typename NodeType, template<class> class ConcreteSpatialStorage>
void TNodesSpatialStorage<NodeType, ConcreteSpatialStorage>::childRemoved(
                                                                  Node& parent, 
                                                                  Node& child)
{
   m_isAdding = false;
   child.accept(*this);

   child.detachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

#endif // _T_NODES_SPATIAL_STORAGE_H
