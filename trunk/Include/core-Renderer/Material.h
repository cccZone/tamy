#pragma once

#include "core-Renderer\RenderingTechnique.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class LightReflectingProperties;
class MaterialStage;
class MaterialOperation;
class MaterialOperationImplementation;
class TransparencyEnabler;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents the material with which 
 * the GraphicalNodes will be rendered. A material
 * is a composite of material components
 */
class Material : public RenderingTechnique
{
private:
   static unsigned char s_stagesArrSize;

   LightReflectingProperties* m_lightReflectingProperties;
   MaterialOperation* m_disableAlpha;
   MaterialOperation* m_disableColor;
   TransparencyEnabler& m_transparencyEnabler;

   typedef MaterialStage* MaterialStageP;
   MaterialStageP* m_stages;

   unsigned char m_stagesCount;

   bool m_transparent;

public:
   Material(const std::string& name,
            RenderingTargetsPolicy& policy,
            LightReflectingProperties* lrp,
            MaterialOperationImplementation& alphaMatOp,
            MaterialOperationImplementation& colorMatOp,
            TransparencyEnabler& transparencyEnabler);
   Material(const Material& rhs);
   ~Material();


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

protected:
   unsigned int beginRendering();
   void endRendering();
   void beginPass(const unsigned int& passIdx);
   void endPass(const unsigned int& passIdx);

private:
   void checkTransparency();
};

///////////////////////////////////////////////////////////////////////////////
