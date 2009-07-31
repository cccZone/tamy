#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

class LightReflectingProperties;
class MaterialStage;
class MaterialOperation;
class MaterialOperationImplementation;
class TransparencyEnabler;
class RenderingTargetsPolicy;
class MaterialImpl;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents the material with which 
 * the GraphicalNodes will be rendered. A material
 * is a composite of material components
 */
class Material
{
private:
   static unsigned char s_stagesArrSize;
   static unsigned int m_nextIndex;

   std::string m_name;
   unsigned int m_index;

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

   /** 
    * Since techinques are used in different rendering strategies,
    * we want to have a way of sorting through them.
    * Each material can get an id assigned telling its order
    * That way we can use them in the sorting algorithms
    */
   unsigned int getIndex() const {return m_index;}
   bool operator<(const Material& rhs) const {return m_index < rhs.m_index;}

   /** 
    * Operator returns true if both materials have the same id
    */
   bool operator==(const Material& rhs) const {return m_index == rhs.m_index;}
   bool operator!=(const Material& rhs) const {return m_index != rhs.m_index;}

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

   void setForRendering(MaterialImpl& impl);

private:
   void checkTransparency();
};

///////////////////////////////////////////////////////////////////////////////
