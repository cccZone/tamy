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
void TNodesSpatialStorage<NodeType, ConcreteSpatialStorage>::detailedQuery(
                                                  const BoundingVolume& volume, 
                                                  Array<NodeType*>& output) const
{
   Array<NodeType*> possiblyVisibleNodes;
   query(volume, possiblyVisibleNodes);

   unsigned int nodesCount = possiblyVisibleNodes.size();
   if (nodesCount == 0) {return;}

   // perform a narrow phase query
   unsigned int trisCount;
   const Array<Triangle*>* geometry;

   NodeType* node = NULL;
   bool visible = false;
   D3DXMATRIX volumeTransformMtx;
   for (unsigned int i = 0; i < nodesCount; ++i)
   {
      visible = false;
      node = possiblyVisibleNodes[i];

      geometry = &(node->getBoundingGeometry());
      trisCount = geometry->size();
      if (trisCount == 0)
      {
         visible = true;
      }
      else
      {
         D3DXMatrixInverse(&volumeTransformMtx, NULL, &(node->getGlobalMtx()));
         for (unsigned int j = 0; j < trisCount; ++j)
         {
            BoundingVolume* transformedVolume = volume * volumeTransformMtx;
            Triangle* tri = (*geometry)[j];
            if (tri->testCollision(*transformedVolume) == true) 
            {
               visible = true;
               delete transformedVolume;
               break;
            }
            delete transformedVolume;
         }
      }

      if (visible)
      {
         output.push_back(node);
      }
   }
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
