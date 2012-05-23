/// @file   core-Renderer\Light.h
/// @brief  representation of a light
#ifndef _LIGHT_H
#define _LIGHT_H

#include "core\Node.h"
#include "core\Color.h"
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special kind of node that represents a light lighting the scene
 */
class Light : public Node, public UniqueObject< Light >
{
public:
   enum LightType
   {
      LT_POINT,
      LT_SPOT,
      LT_DIRECTIONAL
   };

private:
   LightType m_type;

   Color m_ambient;
   Color m_diffuse;
   Color m_specular;

   float m_range;
   float m_attConstant;
   float m_attLinear;
   float m_attQuadratic;

   float m_spotFalloff;
   float m_spotTheta;
   float m_spotPhi;

   unsigned m_changeIdx;

public:
   Light(const std::string& name);
   virtual ~Light() {}

   /**
    * Lights can change. For optimization reasons, various classes
    * that actually render lights may keep their own cached light
    * representation.
    * This method returns an index that can be used as a reference 
    * to check whether any new changes were made to the light. Each new change
    * will alter the index - so telling wether the light has changed
    * or not is a simple matter of checking if the cached index value
    * is the same as the one this method currently is returned - if it's not,
    * the light has changed.
    *
    * This strategy has been used instead of an observer due to the fact that
    * lights may change quite frequently, and there can be many observers,
    * but only a small part of lights can be of interest to a particular
    * group of observers at one point in time - and that's something
    * an observer pattern doesn't cater for - it would send messages
    * out to all interested parties, wether they're interested or not.
    */
   inline unsigned int getChangeIdx() const;

   /**
    * The method sets the light type.
    *
    * @param type       one of the LightTypes
    */
   void setType(LightType type);

   /**
    * The method returns the light type
    *
    * @return           light type
    */
   inline const LightType& getType() const;

   /** 
    * Sets the light's ambient color.
    *
    * @param ambient     ambient color
    */
   inline void setAmbientColor(const Color& ambient);

   /**
    * Returns the light's ambient color.
    *
    * @return           ambient color value
    */
   inline const Color& getAmbientColor() const;

   /** 
    * Sets the light's diffuse color.
    *
    * @param diffuse     diffuse color
    */
   inline void setDiffuseColor(const Color& diffuse);

   /**
    * Returns the light's diffuse color.
    *
    * @return           diffuse color value
    */
   inline const Color& getDiffuseColor() const;

   /** 
    * Sets the light's specular color.
    *
    * @param specular     specular color
    */
   inline void setSpecularColor(const Color& specular);

   /**
    * Returns the light's specular color.
    *
    * @return           specular color value
    */
   inline const Color& getSpecularColor() const;

   /** 
    * Sets the distance to which extent light will be emmited.
    * Range doesn't matter as long as a directional light is concerned.
    *
    * @param range     light emmision range
    */
   void setRange(float range);

   /**
    * Returns the light's range
    *
    * @return           range
    */
   inline float getRange() const;

   /** 
    * Sets the coefficients describing how the light
    * fades with distance.
    *
    * @param constant     constant fading factor influences light 
    *                     despite the distance
    * @param linear       linear fading factor influences light in
    *                     a linear function of the distance
    * @param quadratic    quadratic fading factor influences light in
    *                     a quadratic function of the distance
    */
   inline void setAttenuation(float constant, float linear, float quadratic);

    /**
    * Returns the light's constant attenuation factor
    *
    * @return           constant attenuation factor
    */
   inline float getConstantAtt() const;

   /**
    * Returns the light's linear attenuation factor
    *
    * @return           linear attenuation factor
    */
   inline float getLinearAtt() const;

   /**
    * Returns the light's quadratic attenuation factor
    *
    * @return           quadratic attenuation factor
    */
   inline float getQuadraticAtt() const;

   /** 
    * These params influence only spotlights. They describe the shape
    * and features of the spotlight cone
    *
    * @param falloff    factor telling how will the light be attenuated
    *                   between the inner and the outer cone
    * @param theta      inner cone angle
    * @param phi        outer cone angle
    */
   inline void setSpotlightFeatures(float falloff, float theta, float phi);

   /**
    * Returns the spotlight's inner-to-outer cone attenuation factor
    *
    * @return           inner-to-outer cone attenuation factor
    */
   inline float getFalloff() const;

   /**
    * Returns the light's inner cone angle
    *
    * @return           inner cone angle
    */
   inline float getTheta() const;

   /**
    * Returns the light's outer cone angle
    *
    * @return           outer cone angle
    */
   inline float getPhi() const;

protected:
   void onAccept(NodeVisitor& visitor);

private:
   void adjustBoundingVolume();
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\Light.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _LIGHT_H
