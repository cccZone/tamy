#pragma once

#include <list>
#include <d3dx9.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class NodeVisitor;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a node representation. Nodes can form tree hierarchies.
 */
class Node
{
private:
   std::string m_name;

   // local coordinate system
   D3DXMATRIX m_localMtx;

   // global coordinate system
   D3DXMATRIX m_globalMtx;
   D3DXMATRIX m_parentGlobalMtxCache;
   D3DXMATRIX m_localMtxCache;

   Node* m_parent;
   std::list<Node*> m_children;

public:
   Node(const std::string& name = "");
   virtual ~Node();

   /**
    * Each node has a name(meaningful or otherwise) which serves
    * the node identification purpose. 
    */
   const std::string& getName() const {return m_name;}

   /**
    * This is the matrix that describes the node's absolute world position
    * (unlike the local matrix wich describes the position relative to node's
    * parent).
    */
   const D3DXMATRIX& getGlobalMtx();

   /**
    * This is the matrix that describes the node's position in relation
    * to the position of its parent.
    * It the node doesn't have a parent, this one will be equal
    * to the global matrix
    */
   const D3DXMATRIX& getLocalMtx() const {return m_localMtx;}

   void setLocalMtx(const D3DXMATRIX& localMtx);

   /**
    * The method allows to access the matrix of the node directly,
    * skipping the setters.
    * Manipulating the matrix in this way is sometimes necessary
    * as various libs manipulate pointers to matrices. 
    * Not to worry - the global matrix will always remain in synch
    */
   D3DXMATRIX& accessLocalMtx() {return m_localMtx;}

   /*
    * A group of accessors to the local coordinate system vectors
    */
   void setRightVec(const D3DXVECTOR3& vec);
   void setUpVec(const D3DXVECTOR3& vec);
   void setLookVec(const D3DXVECTOR3& vec);
   void setPosition(const D3DXVECTOR3& vec);
   D3DXVECTOR3 getRightVec() const;
   D3DXVECTOR3 getUpVec() const;
   D3DXVECTOR3 getLookVec() const;
   D3DXVECTOR3 getPosition() const;

   /**
    * A node can have a single parent node. This methid will return true
    * if this is the case.
    */
   bool hasParent() const {return m_parent != NULL;}

   /**
    * A node can have a single parent node. This method will return 
    * a reference to it if this is the case.
    *
    * @throws std::logic_error if the parent doesn't have a node set
    */
   Node& getParent();

   /**
    * A node can have any number of children. Their positions
    * will be kept relative to the parent's position.
    *
    * Since a node can't have more than one parent,
    * attaching a child node to two different parent nodes
    * will result in the node being reattached from the one
    * to which it was attached formerly.
    */
   void addChild(Node* childNode);

   unsigned int getChildrenCount() const {return m_children.size();}

   const std::list<Node*>& getChildren() const {return m_children;}
   
   /**
    * Removes a child from the list of children and resets its
    * parental information.
    * The instance of the child node we want to remove needs to be the one
    * the instance of the parent node has an information about - no name based
    * lookups are provided.
    */
   void removeChild(Node& childNode);

   /**
    * The hierarchy of nodes can be penetrated from outside using
    * the Visitor pattern. 
    *
    * This method calls onAccept on this node's instance to 
    * make the node react to the visitor if applicable,
    * then in relays the visitor to all of its children.
    */
   void accept(NodeVisitor& visitor);


protected:
   /**
    * The hierarchy of nodes can be penetrated from outside using
    * the Visitor pattern. Simply overload this method to accept a visitor.
    *
    * This particular type of node doesn't react to visitors - if it's
    * used as this class only (and it's not something deived from it),
    * then it's used as a Composite for other nodes.
    */
   virtual void onAccept(NodeVisitor& visitor) {}

   void setParent(Node& parent) {m_parent = &parent;}

   void resetParent() {m_parent = NULL;}
};

///////////////////////////////////////////////////////////////////////////////