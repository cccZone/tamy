#include "core-Renderer\LightsSorter.h"
#include "core-Renderer\Light.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

LightsSorter::LightsSorter()
: m_influencedPos(0, 0, 0)
{
}

///////////////////////////////////////////////////////////////////////////////

void LightsSorter::setInfluencedPos(const D3DXVECTOR3& pos)
{
   m_influencedPos = pos;
}

///////////////////////////////////////////////////////////////////////////////

bool LightsSorter::operator()(Light* lhs, Light* rhs) const
{
   return getLightContribution(*lhs) > getLightContribution(*rhs);
}

///////////////////////////////////////////////////////////////////////////////

float LightsSorter::getLightContribution(Light& light) const
{
   const D3DXMATRIX& lightGlobalMtx = light.getGlobalMtx();
   D3DXVECTOR3 lightDir(lightGlobalMtx._31, lightGlobalMtx._32, lightGlobalMtx._33);
   D3DXVECTOR3 lightPos(lightGlobalMtx._41, lightGlobalMtx._42, lightGlobalMtx._43);

   D3DXVECTOR3 direction = lightPos - m_influencedPos;
   float distance = D3DXVec3Length(&direction);

   if ((light.getType() != Light::LT_DIRECTIONAL) && 
       (distance > light.getRange()))
   {
      return 0;
   }

   D3DXVec3Normalize(&direction, &direction);

   // calculate light's attenuation factor.
   float atten = 1.0f;
   if (light.getType() != Light::LT_DIRECTIONAL)
   {
      atten = (light.getConstantAtt() + 
               light.getLinearAtt() * distance + 
               light.getQuadraticAtt() * (distance * distance));
      if (atten > 0) atten = 1 / atten; // avoid divide by zero case

   }

   // calculate light's spot factor
   float spot = 1.0f;
   if (light.getType() ==Light::LT_SPOT)
   {
      float rho = D3DXVec3Dot(&(-lightDir), &direction);

      if (rho > cosf(light.getTheta() / 2.0f)) 
      {
         spot = 1.0f;
      }
      else if (rho <= cosf(light.getPhi() / 2.0f)) 
      {
         spot = 0.0f;
      }
      else
      {
         spot = powf(((rho - cosf(light.getPhi() / 2.0f)) / 
                      (cosf(light.getTheta() / 2.0f) - 
                       cosf(light.getPhi()/ 2.0f))), 
                     light.getFalloff());

      }
   }

   float attSpotFactor = atten * spot;

   // calculate diffuse contribution for this point
   Color lightDiffuse = light.getDiffuseColor();
   Color diffuse;
   diffuse.r = lightDiffuse.r * attSpotFactor;
   diffuse.g = lightDiffuse.g * attSpotFactor;
   diffuse.b = lightDiffuse.b * attSpotFactor;

   // calculate ambient contribution for this point
   Color lightAmbient = light.getAmbientColor();
   Color ambient;
   ambient.r = lightAmbient.r * attSpotFactor;
   ambient.g = lightAmbient.g * attSpotFactor;
   ambient.b = lightAmbient.b * attSpotFactor;

   // calculate light contribution (fabsf() because even dark-lights contribute)
   Color color = ambient + diffuse;
   float contribution = fabsf(color.r);
   if (fabsf(color.g) > contribution) {contribution = fabsf(color.g);}
   if (fabsf(color.b) > contribution) {contribution = fabsf(color.b);}

   return contribution;
}

///////////////////////////////////////////////////////////////////////////////
