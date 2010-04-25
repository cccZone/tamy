#pragma once

#include "core-Renderer\AbstractGraphicalNode.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a node that can be rendered on a graphical device.
 * This node instantiates a graphical entity
 */
class GraphicalNode : public AbstractGraphicalNode
{
private:
   Array<D3DXMATRIX> m_renderingMatrices;

public:
   GraphicalNode(const std::string& name, 
                 LeafGraphicalEntity& entity, 
                 DWORD subset);

   const Array<D3DXMATRIX>& getRenderingMatrices();
};

///////////////////////////////////////////////////////////////////////////////
