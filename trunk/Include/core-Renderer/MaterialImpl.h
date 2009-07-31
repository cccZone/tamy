#pragma once


///////////////////////////////////////////////////////////////////////////////

class LightReflectingProperties;
class TextureImpl;
class MaterialOperation;
class CoordinatesOperation;

///////////////////////////////////////////////////////////////////////////////

class MaterialImpl
{
public:
   virtual ~MaterialImpl() {}

   virtual void setTransparency(bool enable) = 0;
   
   virtual void setLRP(LightReflectingProperties& lrp) = 0;

   virtual void setTexture(unsigned char stageIndex, TextureImpl& textureImpl) = 0;

   virtual void setColorOperation(unsigned char stageIndex, MaterialOperation& colorOp) = 0;

   virtual void setAlphaOperation(unsigned char stageIndex, MaterialOperation& alphaOp) = 0;

   virtual void setCoordsOperation(unsigned char stageIndex, CoordinatesOperation& coordsOp) = 0;

};

///////////////////////////////////////////////////////////////////////////////
