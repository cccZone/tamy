#pragma once


///////////////////////////////////////////////////////////////////////////////

class Texture;
class MaterialOperation;
class CoordinatesOperation;
class StageTextureRenderer;
class RendererImpl;

///////////////////////////////////////////////////////////////////////////////

class MaterialStage
{
private:
   int m_index;
   Texture& m_texture;
   MaterialOperation* m_colorOperation;
   MaterialOperation* m_alphaOperation;
   CoordinatesOperation* m_coordsOp;

public:
   MaterialStage(Texture& texture, 
                 MaterialOperation* defaultColorOp, 
                 MaterialOperation* defaultAlphaOp,
                 CoordinatesOperation* coordsOp);
   MaterialStage(const MaterialStage& rhs);
   ~MaterialStage();

   void setIndex(int val) {m_index = val;}
   int getIndex() const {return m_index;}

   bool operator==(const MaterialStage& rhs) const;
   bool operator!=(const MaterialStage& rhs) const;

   const CoordinatesOperation& getCoordOperation() const {return *m_coordsOp;}

   const MaterialOperation& getColorOperation() const {return *m_colorOperation;}

   const MaterialOperation& getAlphaOperation() const {return *m_alphaOperation;}

   const Texture& getTexture() const {return m_texture;}

   void setForRendering(RendererImpl& impl) const;
};

///////////////////////////////////////////////////////////////////////////////
