#pragma once


///////////////////////////////////////////////////////////////////////////////

class LightReflectingProperties;
class MaterialStage;
class MaterialOperation;
class MaterialOperationImplementation;

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

   unsigned int m_index;
   LightReflectingProperties& m_lightReflectingProperties;
   MaterialOperation* m_disableAlpha;
   MaterialOperation* m_disableColor;

   typedef MaterialStage* MaterialStageP;
   MaterialStageP* m_stages;

   unsigned char m_stagesCount;

   bool m_transparent;

public:
   Material(LightReflectingProperties& lrp, 
            MaterialOperationImplementation& alphaMatOp,
            MaterialOperationImplementation& colorMatOp,
            unsigned int index = 0);
   ~Material();

   unsigned int getStagesCount() const {return m_stagesCount;}

   void addStage(MaterialStage* stage);
   void removeStage(unsigned int stageIdx);
   MaterialStage& getStage(unsigned int stageIdx);

   LightReflectingProperties& getLightReflectingProperties() {return m_lightReflectingProperties;}

   /** 
    * Since materials are used in the rendering strategy as a batching pointer,
    * we want to have a way of sorting through them.
    * Each material can get an id assigned telling its order
    * That way we can use them in the sorting algorithms
    */
   unsigned int getIndex() const {return m_index;}
   bool operator<(const Material& rhs) const {return m_index < rhs.m_index;}

   bool operator==(const Material& rhs) const;
   bool operator!=(const Material& rhs) const;

   bool isTransparent() const;

   void setForRendering();

protected:
   /**
    * This method is called whenever we need to render a transparent material.
    * The implementation of this class should make whatever's necessary
    * in the graphical; library it uses to ensure that
    */
   virtual void enableTransparency(bool enable) {}

private:
   void checkTransparency();
};

///////////////////////////////////////////////////////////////////////////////
