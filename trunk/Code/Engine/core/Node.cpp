#include "core\Node.h"
#include <stdexcept>
#include <algorithm>
#include "core\MatrixWriter.h"
#include "core\dostream.h"
#include "core\NodeObserver.h"
#include "core\BoundingVolume.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

Node::Node(const std::string& name)
: m_name(name)
, m_parent(NULL)
, m_volume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 0))
, m_globalVolume(NULL)
{
   D3DXMatrixIdentity(&m_localMtx);
   D3DXMatrixIdentity(&m_globalMtx);

   // initialize our cache with some stupid data to force the initial update
   // of the global matrix for all the objects
   D3DXMatrixIdentity(&m_localMtxCache);
   m_localMtxCache._11 = -1;
   m_localMtxCache._22 = -1;
   m_localMtxCache._33 = -1;
   m_localMtxCache._44 = -1;

   D3DXMatrixIdentity(&m_parentGlobalMtxCache);
   m_parentGlobalMtxCache._11 = -1;
   m_parentGlobalMtxCache._22 = -1;
   m_parentGlobalMtxCache._33 = -1;
   m_parentGlobalMtxCache._44 = -1;
}

///////////////////////////////////////////////////////////////////////////////

Node::~Node()
{
   delete m_volume;
   m_volume = NULL;

   delete m_globalVolume;
   m_globalVolume = NULL;

   m_parent = NULL;

   for (std::list<Node*>::iterator it = m_childrenNodes.begin();
      it != m_childrenNodes.end(); ++it)
   {
      delete *it;
   }
   m_childrenNodes.clear();
}

///////////////////////////////////////////////////////////////////////////////

void Node::setLocalMtx(const D3DXMATRIX& localMtx) 
{
   m_localMtx = localMtx;
}

///////////////////////////////////////////////////////////////////////////////

void Node::setRightVec(const D3DXVECTOR3& vec)
{
   m_localMtx._11 = vec.x;
   m_localMtx._12 = vec.y;
   m_localMtx._13 = vec.z;
}

///////////////////////////////////////////////////////////////////////////////

void Node::setUpVec(const D3DXVECTOR3& vec)
{
   m_localMtx._21 = vec.x;
   m_localMtx._22 = vec.y;
   m_localMtx._23 = vec.z;
}

///////////////////////////////////////////////////////////////////////////////

void Node::setLookVec(const D3DXVECTOR3& vec)
{
   m_localMtx._31 = vec.x;
   m_localMtx._32 = vec.y;
   m_localMtx._33 = vec.z;
}

///////////////////////////////////////////////////////////////////////////////

void Node::setPosition(const D3DXVECTOR3& vec)
{
   m_localMtx._41 = vec.x;
   m_localMtx._42 = vec.y;
   m_localMtx._43 = vec.z;
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 Node::getRightVec() const
{
   return D3DXVECTOR3(m_localMtx._11, m_localMtx._12, m_localMtx._13);
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 Node::getUpVec() const
{
   return D3DXVECTOR3(m_localMtx._21, m_localMtx._22, m_localMtx._23);
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 Node::getLookVec() const
{
   return D3DXVECTOR3(m_localMtx._31, m_localMtx._32, m_localMtx._33);
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 Node::getPosition() const
{
   return D3DXVECTOR3(m_localMtx._41, m_localMtx._42, m_localMtx._43);
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& Node::getGlobalMtx() const
{
   if (hasParentNode() == false) {return m_localMtx;}

   // this node's global matrix can be influenced by the change
   // in this node's local matrix, or any of it's parents' global matrices.
   // we need to check for both events.
   bool localMatrixChanged = false;

   // The case with the parent's matrices will be dealt with using recurency,
   // so we basically need to check only the immediate parent to spot the difference
   // in the global matrix

   if (m_localMtxCache != m_localMtx)
   {
      m_localMtxCache = m_localMtx;
      m_globalMtx = m_localMtx;
      localMatrixChanged = true;
   }

   const D3DXMATRIX& parentGlobalMtx = m_parent->getGlobalMtx();
   if (localMatrixChanged || (m_parentGlobalMtxCache != parentGlobalMtx))
   {
      m_parentGlobalMtxCache = parentGlobalMtx;
      D3DXMatrixMultiply(&m_globalMtx, &m_localMtx, &m_parentGlobalMtxCache);
   }

   return m_globalMtx;
}

///////////////////////////////////////////////////////////////////////////////

void Node::getGlobalVectors( D3DXVECTOR3& right, D3DXVECTOR3& up, D3DXVECTOR3& look, D3DXVECTOR3& pos ) const
{
   D3DXMATRIX globalMtx = getGlobalMtx();
   right = D3DXVECTOR3(globalMtx._11, globalMtx._12, globalMtx._13);
   up = D3DXVECTOR3(globalMtx._21, globalMtx._22, globalMtx._23);
   look = D3DXVECTOR3(globalMtx._31, globalMtx._32, globalMtx._33);
   pos = D3DXVECTOR3(globalMtx._41, globalMtx._42, globalMtx._43);
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& Node::getBoundingVolume()
{
   delete m_globalVolume;
   m_globalVolume = *m_volume * getGlobalMtx();
   return *m_globalVolume;
}

///////////////////////////////////////////////////////////////////////////////

void Node::setBoundingVolume(BoundingVolume* volume)
{
   if (volume == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a BoundingVolume instance");
   }
   delete m_volume;
   m_volume = volume;
}

///////////////////////////////////////////////////////////////////////////////

Node& Node::getParentNode()
{
   if (m_parent == NULL) 
   {
      throw std::logic_error(std::string("This node doesn't have a parent"));
   }
   return *m_parent;
}

///////////////////////////////////////////////////////////////////////////////


void Node::addChild(Node* childNode)
{
   if (childNode->hasParentNode())
   {
      Node& prevParent = childNode->getParentNode();
      prevParent.removeChild(*childNode);
   }

   m_childrenNodes.push_back(childNode);
   childNode->setParent(*this);

   for (std::list<NodeObserver*>::iterator it = m_observers.begin();
        it != m_observers.end(); ++it)
   {
      (*it)->childAdded(*this, *childNode);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Node::removeChild(Node& childNode)
{
   for (std::list<NodeObserver*>::iterator it = m_observers.begin();
        it != m_observers.end(); ++it)
   {
      (*it)->childRemoved(*this, childNode);
   }

   std::list<Node*>::iterator it = std::find(m_childrenNodes.begin(), m_childrenNodes.end(), &childNode);
   if (it != m_childrenNodes.end())
   {
      childNode.resetParent();
      m_childrenNodes.erase(it);
   }
}

///////////////////////////////////////////////////////////////////////////////

Node* Node::findNode( const std::string& name )
{
   std::list< Node* >   nodesQueue;
   nodesQueue.push_back( this );

   while ( !nodesQueue.empty() )
   {
      Node* currNode = nodesQueue.front();
      nodesQueue.pop_front();
      if ( currNode->getName() == name )
      {
         return currNode;
      }

      for ( std::list<Node*>::iterator it = currNode->m_childrenNodes.begin(); it != currNode->m_childrenNodes.end(); ++it )
      {
         if ( *it != NULL )
         {
            nodesQueue.push_back( *it );
         }
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Node::accept(NodeVisitor& visitor)
{
   onAccept(visitor);

   for (std::list<Node*>::iterator it = m_childrenNodes.begin(); it != m_childrenNodes.end(); ++it)
   {
      (*it)->accept(visitor);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Node::attachObserver(NodeObserver& observer)
{
   m_observers.push_back(&observer);

   for (std::list<Node*>::iterator it = m_childrenNodes.begin();
      it != m_childrenNodes.end(); ++it)
   {
      (*it)->attachObserver(observer);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Node::detachObserver(NodeObserver& observer)
{
   for (std::list<NodeObserver*>::iterator it = m_observers.begin();
        it != m_observers.end(); ++it)
   {
      if (&observer == *it)
      {
         m_observers.erase(it);
         break;
      }
   }

   for (std::list<Node*>::iterator it = m_childrenNodes.begin();
      it != m_childrenNodes.end(); ++it)
   {
      (*it)->detachObserver(observer);
   }
}

///////////////////////////////////////////////////////////////////////////////
