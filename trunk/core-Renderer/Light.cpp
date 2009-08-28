#include "core-Renderer\Light.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core\BoundingSphere.h"
#include "core\BoundingSpace.h"


///////////////////////////////////////////////////////////////////////////////

Light::Light(const std::string& name)
: Node(name)
, m_type(LT_POINT)
, m_ambient(0, 0, 0, 0)
, m_diffuse(0, 0, 0, 0)
, m_specular(0, 0, 0, 0)
, m_range(0)
, m_attConstant(0)
, m_attLinear(1)
, m_attQuadratic(0)
, m_spotFalloff(1)
, m_spotTheta(0)
, m_spotPhi(0)
, m_changeIdx(0)
{
}

///////////////////////////////////////////////////////////////////////////////

void Light::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(TNodesVisitor<Light>);
}

///////////////////////////////////////////////////////////////////////////////

void Light::setType(LightType type)
{
   ++m_changeIdx;
   m_type = type;
   adjustBoundingVolume();
}

///////////////////////////////////////////////////////////////////////////////

void Light::setRange(float range)
{
   ++m_changeIdx;
   m_range = range;
   adjustBoundingVolume();
}

///////////////////////////////////////////////////////////////////////////////

void Light::adjustBoundingVolume()
{
   switch(m_type)
   {
   case LT_POINT: 
      {
         setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), m_range)); 
         break;
      }
   case LT_SPOT: 
      {
         float halfRange = m_range * 0.5f;
         D3DXVECTOR3 lookVec = getLookVec();
         D3DXVECTOR3 pos = getPosition();
         D3DXVECTOR3 sphereCentre = pos + lookVec * halfRange;
         setBoundingVolume(new BoundingSphere(sphereCentre, halfRange)); 
         break;
      }
   case LT_DIRECTIONAL: 
      {
         setBoundingVolume(new BoundingSpace()); 
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
