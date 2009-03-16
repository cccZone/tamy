#pragma once

#include "core\Node.h"
#include "core-Renderer\Color.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special kind of node that represents a light lighting the scene
 */
class Light : public Node
{
public:
   enum LightType
   {
      LT_POINT,
      LT_SPOT,
      LT_DIRECTIONAL
   };

public:
   Light(const std::string& name);
   virtual ~Light() {}

   /**
    * The method turns the light ON or OFF
    */
   virtual void enable(bool val) = 0;

   virtual void setType(LightType type) = 0;
   virtual void setAmbientColor(const Color& ambient) = 0;
   virtual void setDiffuseColor(const Color& diffuse) = 0;
   virtual void setSpecularColor(const Color& specular) = 0;
   virtual void setRange(float range) = 0;
   virtual void setAttenuation(float constant, float linear, float quadratic) = 0;
   virtual void setSpotlightFeatures(float falloff, float theta, float phi) = 0;

protected:
   void onAccept(NodeVisitor& visitor);
};

///////////////////////////////////////////////////////////////////////////////
