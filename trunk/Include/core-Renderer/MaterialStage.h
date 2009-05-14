#pragma once


///////////////////////////////////////////////////////////////////////////////

class Texture;
class MaterialOperation;

///////////////////////////////////////////////////////////////////////////////

class MaterialStage
{
private:
   int m_index;
   Texture& m_texture;
   MaterialOperation* m_colorOperation;
   MaterialOperation* m_alphaOperation;

   bool m_transparent;

public:
   MaterialStage(Texture& emptyTexture, 
                 MaterialOperation* defaultColorOp, 
                 MaterialOperation* defaultAlphaOp);
   ~MaterialStage();

   void setIndex(int val) {m_index = val;}
   int getIndex() const {return m_index;}

   bool operator==(const MaterialStage& rhs) const;
   bool operator!=(const MaterialStage& rhs) const;

   const MaterialOperation& getColorOperation() const {return *m_colorOperation;}

   const MaterialOperation& getAlphaOperation() const {return *m_alphaOperation;}

   const Texture& getTexture() const {return m_texture;}

   bool isTransparent() const {return m_transparent;}

   void setForRendering();
};

///////////////////////////////////////////////////////////////////////////////
