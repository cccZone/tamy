#pragma once

#include <string>
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

class LightReflectingProperties;
class MaterialStage;
class MaterialOperation;
class MaterialOperationImplementation;
class TransparencyEnabler;
class RenderingTargetsPolicy;
class RendererImpl;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents the material with which 
 * the GraphicalNodes will be rendered. A material
 * is a composite of material components.
 */
class Material : public UniqueObject
{
private:
   static unsigned char s_stagesArrSize;

   std::string m_name;

   LightReflectingProperties* m_lightReflectingProperties;

   typedef MaterialStage* MaterialStageP;
   MaterialStageP* m_stages;
   unsigned char m_stagesCount;

   bool m_transparent;

public:
   Material(const std::string& name);
   Material(const Material& rhs);
   ~Material();

   /**
    * Material bears a name. It gives it a 'recognizable label'.
    *
    * However - the name of the material is not it's unique ID - it's index is.
    * Two completely different materials can bear the same name, however
    * it is guaranteed that THEY WILL HAVE DIFFERENT IDs.
    */
   const std::string& getName() const {return m_name;}

   unsigned int getStagesCount() const {return m_stagesCount;}

   void addStage(MaterialStage* stage);
   void removeStage(unsigned int stageIdx);
   MaterialStage& getStage(unsigned int stageIdx);

   LightReflectingProperties& getLightReflectingProperties() {return *m_lightReflectingProperties;}

   /**
    * This method allows to change the light reflecting properties of a material
    */
   void setLightReflectingProperties(LightReflectingProperties* lrp);

   bool isTransparent() const;

   void setForRendering(RendererImpl& impl) const;

private:
   void checkTransparency();
};

///////////////////////////////////////////////////////////////////////////////
