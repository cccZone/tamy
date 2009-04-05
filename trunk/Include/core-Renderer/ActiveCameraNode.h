#pragma once

#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

class ActiveCameraNode : public Node
{
private:
   Node m_defaultNode;
   Node* m_activeCamera;

public:
   ActiveCameraNode();
   ~ActiveCameraNode();

   void setCameraNode(Node& camera);
   void resetCameraNode();

   const D3DXMATRIX& getGlobalMtx();
   const D3DXMATRIX& getLocalMtx() const;
   void setLocalMtx(const D3DXMATRIX& localMtx);
   D3DXMATRIX& accessLocalMtx();

   void setRightVec(const D3DXVECTOR3& vec);
   void setUpVec(const D3DXVECTOR3& vec);
   void setLookVec(const D3DXVECTOR3& vec);
   void setPosition(const D3DXVECTOR3& vec);
   D3DXVECTOR3 getRightVec() const;
   D3DXVECTOR3 getUpVec() const;
   D3DXVECTOR3 getLookVec() const;
   D3DXVECTOR3 getPosition() const;
};

///////////////////////////////////////////////////////////////////////////////
