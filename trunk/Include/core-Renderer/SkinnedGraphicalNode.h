#pragma once

#include "core-Renderer\AbstractGraphicalNode.h"
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
   BonesDef m_bones;
   Array<D3DXMATRIX> m_renderingMatrices;

public:
   SkinnedGraphicalNode(const std::string& name,
                        LeafGraphicalEntity& entity,
                        DWORD subset,
                        const BonesDef& bones);

   const Array<D3DXMATRIX>& getRenderingMatrices();

   inline const BonesDef& getBonesDefinition() const {return m_bones;}
};

///////////////////////////////////////////////////////////////////////////////
