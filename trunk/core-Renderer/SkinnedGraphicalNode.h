#pragma once

#include "AbstractGraphicalNode.h"
#include <d3dx9.h>
#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class SkinnedGraphicalEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * This node represents an instance of a skinned graphical entity's subset
 * in the scene
 */
class SkinnedGraphicalNode : public AbstractGraphicalNode
{
public:
   /**
    * first elem - bone node
    * second elem - bone offset matrix
    */
   typedef std::vector<std::pair<Node*, D3DXMATRIX> > BonesDef;

private:
   SkinnedGraphicalEntity& m_entity;
   BonesDef m_bones;

public:
   SkinnedGraphicalNode(const std::string& name,
                        SkinnedGraphicalEntity& entity,
                        DWORD subset,
                        const BonesDef& bones);

   void render();

   inline SkinnedGraphicalEntity& getEntity() const {return m_entity;}
   inline const BonesDef& getBonesDefinition() const {return m_bones;}
};

///////////////////////////////////////////////////////////////////////////////
