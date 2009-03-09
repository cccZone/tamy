#pragma once

#include "MatrixWriter.h"
#include "Node.h"
#include "HierarchyWriter.h"
#include "MeshDefinition.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class MeshDefinitionWriter : public HierarchyWriter<MeshDefinition>
{
private:
   bool m_writeBasics;
   bool m_writeVertices;
   bool m_writeFaces;
   bool m_writeBones;
public:
   MeshDefinitionWriter()
      : m_writeBasics(true), m_writeVertices(false), m_writeFaces(false), m_writeBones(false)
   {
   }

   void writeBasics()
   {
      m_writeBasics = true;
      m_writeVertices = false;
      m_writeFaces = false;
      m_writeBones = false;
   }

   void writeVertices()
   {
      m_writeBasics = false;
      m_writeVertices = true;
      m_writeFaces = false;
      m_writeBones = false;
   }

   void writeFaces()
   {
      m_writeBasics = false;
      m_writeVertices = false;
      m_writeFaces = true;
      m_writeBones = false;
   }

   void writeBones()
   {
      m_writeBasics = false;
      m_writeVertices = false;
      m_writeFaces = false;
      m_writeBones = true;
   }

protected:
   void writeSingle(std::ostream& stream, MeshDefinition& entity)
   {
      if (m_writeBasics)
      {
         stream << "name : " << entity.name << 
                   "; children count : " << entity.getChildren().size() <<
                   "; local mtx : " << entity.localMtx<< "; " << std::endl;
      }


      if (m_writeVertices)
      {
         stream << "vertices : {" << std::endl;

         for (std::list<LitVertex>::iterator it = entity.vertices.begin();
              it != entity.vertices.end(); ++it)
         {
            stream << *it << ", " << std::endl;
         }

         stream << "};" << std::endl;
      }

      if (m_writeFaces)
      {
         stream << "faces : {" << std::endl;
         for (std::list<Face<USHORT> >::iterator it = entity.faces.begin();
              it != entity.faces.end(); ++it)
         {
            stream << "[" << it->idx[0] << ", " << it->idx[1] << 
               ", " << it->idx[2] << " - " << it->subsetID << "], " << std::endl;
         }
         stream << "};" << std::endl;
      }

      if (m_writeBones)
      {
         stream << "bonesInfluencingAttribute : {" << std::endl;
         for (std::vector<BonesInfluenceDefinition>::iterator setIt = entity.bonesInfluencingAttribute.begin();
              setIt != entity.bonesInfluencingAttribute.end(); ++setIt)
         {
            for (BonesInfluenceDefinition::iterator boneIt = setIt->begin();
                 boneIt != setIt->end(); ++boneIt)
            {
               stream << *boneIt << ", " << std::endl;
            }
         }

         stream << "};" << std::endl << "skinBones : {" << std::endl;
         for (std::vector<SkinBoneDefinition>::iterator boneIt = entity.skinBones.begin();
              boneIt != entity.skinBones.end(); ++boneIt)
         {
            stream << "[" << boneIt->name << " - " << boneIt->boneOffset << "]," << std::endl;
         }

         stream << "}" << std::endl;
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
