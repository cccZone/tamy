#pragma once

#include <list>
#include <vector>
#include "core-ResourceManagement\LitVertex.h"
#include "core-ResourceManagement\Face.h"
#include <string>
#include "core-Renderer\Color.h"
#include <d3dx9.h>
#include "core-Renderer\SkinBoneDefinition.h"


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

   MaterialDefinition(const std::string& _matName,
                      const Color& _ambient,
                      const Color& _diffuse,
                      const Color& _specular,
                      const Color& _emissive,
                      float _power,
                      const std::string& _texName)
         : matName(_matName),
         ambient(_ambient),
         diffuse(_diffuse),
         specular(_specular),
         emissive(_emissive),
         power(_power),
         texName(_texName)
   {}

   MaterialDefinition(const std::string& name)
      : matName(name), power(1)
   {}
};

///////////////////////////////////////////////////////////////////////////////

struct MeshDefinition
{
   // -------------------------------------------------------------------------

   std::string name;
   bool isSkin;

   std::vector<MaterialDefinition> materials;
   std::vector<LitVertex> vertices;
   std::list<Face<USHORT> > faces;

   // these two attributes have meaning only if the mesh is a skin
   std::vector<BonesInfluenceDefinition> bonesInfluencingAttribute;
   std::vector<SkinBoneDefinition> skinBones;

   D3DXMATRIX localMtx;

   MeshDefinition* parent;
   std::list<MeshDefinition*> children;

   // -------------------------------------------------------------------------

   MeshDefinition() : isSkin(false), parent(NULL) {}
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
