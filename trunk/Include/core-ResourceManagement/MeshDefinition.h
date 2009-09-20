#pragma once

#include <list>
#include <vector>
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\Face.h"
#include <string>
#include "core-Renderer\Color.h"
#include <d3dx9.h>
#include "core-Renderer\SkinBoneDefinition.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"


///////////////////////////////////////////////////////////////////////////////

struct MaterialDefinition
{
   std::string matName;

   Color ambient;
   Color diffuse;
   Color specular;
   Color emissive;
   float power;

   std::string texName;
   MatOpCode colorOp;
   SourceCode colorArg1;
   SourceCode colorArg2;
   MatOpCode alphaOp;
   SourceCode alphaArg1;
   SourceCode alphaArg2;
   CoordsOpCode coordsOp;

   MaterialDefinition(const std::string& _matName,
                      const Color& _ambient,
                      const Color& _diffuse,
                      const Color& _specular,
                      const Color& _emissive,
                      float _power,
                      const std::string& _texName,
                      MatOpCode _colorOp,
                      SourceCode _colorArg1,
                      SourceCode _colorArg2,
                      MatOpCode _alphaOp,
                      SourceCode _alphaArg1,
                      SourceCode _alphaArg2,
                      CoordsOpCode _coordsOp)
         : matName(_matName),
         ambient(_ambient),
         diffuse(_diffuse),
         specular(_specular),
         emissive(_emissive),
         power(_power),
         texName(_texName),
         colorOp(_colorOp), colorArg1(_colorArg1), colorArg2(_colorArg2),
         alphaOp(_alphaOp), alphaArg1(_alphaArg1), alphaArg2(_alphaArg2),
         coordsOp(_coordsOp)
   {}

   MaterialDefinition(const std::string& name)
      : matName(name), power(1), 
      colorOp(MOP_DISABLE), colorArg1(SC_NONE), colorArg2(SC_NONE),
      alphaOp(MOP_DISABLE), alphaArg1(SC_NONE), alphaArg2(SC_NONE),
      coordsOp(CC_WRAP)
   {}
};

///////////////////////////////////////////////////////////////////////////////

struct MeshDefinition
{
   // -------------------------------------------------------------------------

   std::string name;
   bool isSkin;

   std::vector<std::string> materials;
   std::vector<LitVertex> vertices;
   std::vector<Face<USHORT> > faces;

   // these two attributes have meaning only if the mesh is a skin
   std::vector<BonesInfluenceDefinition> bonesInfluencingAttribute;
   std::vector<SkinBoneDefinition> skinBones;

   D3DXMATRIX localMtx;

   MeshDefinition* parent;
   std::list<MeshDefinition*> children;

   // -------------------------------------------------------------------------

   MeshDefinition() : isSkin(false), parent(NULL) 
   {
      D3DXMatrixIdentity(&localMtx);
   }

   MeshDefinition(const MeshDefinition& rhs)
      : name(rhs.name),
      isSkin(rhs.isSkin),
      materials(rhs.materials),
      vertices(rhs.vertices),
      faces(rhs.faces),
      bonesInfluencingAttribute(rhs.bonesInfluencingAttribute),
      skinBones(rhs.skinBones),
      localMtx(rhs.localMtx),
      parent(rhs.parent)
   {
      for (std::list<MeshDefinition*>::const_iterator it = rhs.children.begin();
         it != rhs.children.end(); ++it)
      {
         children.push_back(new MeshDefinition(**it));
      }
   }
   ~MeshDefinition()
   {
      for (std::list<MeshDefinition*>::iterator it = children.begin();
         it != children.end(); ++it)
      {
         delete *it;
      }
      children.clear();
   }
   MeshDefinition& operator=(const MeshDefinition& rhs)
   {
      name = rhs.name;
      isSkin = rhs.isSkin;
      materials = rhs.materials;
      vertices = rhs.vertices;
      faces = rhs.faces;
      bonesInfluencingAttribute = rhs.bonesInfluencingAttribute;
      skinBones = rhs.skinBones;
      localMtx = rhs.localMtx;
      parent = rhs.parent;

      for (std::list<MeshDefinition*>::iterator it = children.begin();
         it != children.end(); ++it)
      {
         delete *it;
      }
      children.clear();

      for (std::list<MeshDefinition*>::const_iterator it = rhs.children.begin();
         it != rhs.children.end(); ++it)
      {
         children.push_back(new MeshDefinition(**it));
      }

      return *this;
   }

   const std::list<MeshDefinition*>& getChildren() const {return children;}
};

///////////////////////////////////////////////////////////////////////////////
