#pragma once

#include "core\SceneManager.h"
#include "core\Node.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core\NodeObserver.h"
#include "core\Assert.h"
#include "core\LinearNodesStorage.h"
#include "core\Octree.h"


///////////////////////////////////////////////////////////////////////////////

class AbstractSceneManager;

///////////////////////////////////////////////////////////////////////////////

template<typename NodeType>
class SceneAspectManager : public TNodesVisitor<NodeType>
{
private:
   AbstractSceneManager* m_controller;

   LinearNodesStorage<NodeType>* m_dynamicNodesContainer;
   Octree<NodeType>* m_staticNodesContainer;
   
public:
   virtual ~SceneAspectManager() 
   {
      delete m_staticNodesContainer;
      m_staticNodesContainer = NULL;

      delete m_dynamicNodesContainer;
      m_dynamicNodesContainer = NULL;
   }

   void visit(NodeType& node)
   {
      ASSERT(m_controller != NULL, "Class not initialized. Set the controller!!!");

      if (m_controller->isAdding())
      {
         if (node.isDynamic())
         {
            m_dynamicNodesContainer->insert(node);
         }
         else
         {
            m_staticNodesContainer->insert(node);
         }
         onAdd(node);
      }
      else
      {
         onRemove(node);
         if (node.isDynamic())
         {
            m_dynamicNodesContainer->remove(node);
         }
         else
         {
            m_staticNodesContainer->remove(node);
         }
      }
   }

   void setController(AbstractSceneManager& controller) 
   {
      m_controller = &controller;
   }

   /**
   * The method allows to query the scene for nodes that overlap
   * the volume passed in the @param volume.
   *
   * The results are returned in the array passed in the @param output
   */
   void query(const BoundingVolume& volume, Array<NodeType*>& output) const
   {
      m_staticNodesContainer->query(volume, output);
      m_dynamicNodesContainer->query(volume, output);
   }

   /**
   * The method allows to query the scene for nodes that overlap
   * the volume passed in the @param volume. The query is performed in broad and narrow phase,
   * meaning that not only the bounding volumes of the scene nodes, but also
   * the underlying geometry will be tested
   *
   * The results are returned in the array passed in the @param output
   */
   void detailedQuery(const BoundingVolume& volume, Array<NodeType*>& output) const
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

protected:
   SceneAspectManager(unsigned int maxElemsPerSector, float worldSize)
      : m_controller(NULL),
      m_staticNodesContainer(new Octree<NodeType>(maxElemsPerSector, worldSize)),
      m_dynamicNodesContainer(new LinearNodesStorage<NodeType>())
   {
   }

   /**
    * Overload this method if you want to do something when a node is added
    */
   virtual void onAdd(NodeType& node) = 0;

   /**
   * Overload this method if you want to do something when a node is removed
   */
   virtual void onRemove(NodeType& node) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
* You NEED TO invoke this macro in the scene manager class's constructor
* in order for it to work properly
*/
#define REGISTER_SCENE_ASPECT(NodeType) ((SceneAspectManager<NodeType>*)(this))->setController(*this);

///////////////////////////////////////////////////////////////////////////////

/**
* This is a leaf scene manager - it implements the management
* of a single aspect of a scene - audio, visual, AI etc..
*/
class AbstractSceneManager : public SceneManager,
                             public NodeVisitor
{
private:
   bool m_adding;

public:
   virtual ~AbstractSceneManager() {}

   void addNode(Node* node)
   {
      m_adding = true;
      node->accept(*this);
   }

   void removeNode(Node& node)
   {
      m_adding = false;
      node.accept(*this);
   }

   /**
   * The method allows to query the scene for nodes of a particular type that overlap
   * the volume passed in the @param volume.
   *
   * The results are returned in the array passed in the @param output
   */
   template<typename NodeType>
   void query(const BoundingVolume& volume, Array<NodeType*>& output) const
   {
      const SceneAspectManager<NodeType>* mgr = dynamic_cast<const SceneAspectManager<NodeType>*> (this);
      if (mgr == NULL) {return;}
      mgr->query(volume, output);
   }

   /**
   * The method allows to query the scene for nodes of a particular type that overlap
   * the volume passed in the @param volume. The query is performed in broad and narrow phase,
   * meaning that not only the bounding volumes of the scene nodes, but also
   * the underlying geometry will be tested
   *
   * The results are returned in the array passed in the @param output
   */
   template<typename NodeType>
   void detailedQuery(const BoundingVolume& volume, Array<NodeType*>& output) const
   {
      const SceneAspectManager<NodeType>* mgr = dynamic_cast<const SceneAspectManager<NodeType>*> (this);
      if (mgr == NULL) {return;}
      mgr->detailedQuery(volume, output);
   }

   inline bool isAdding() const {return m_adding;}

protected:
   AbstractSceneManager() : m_adding(true) {}
};

///////////////////////////////////////////////////////////////////////////////
