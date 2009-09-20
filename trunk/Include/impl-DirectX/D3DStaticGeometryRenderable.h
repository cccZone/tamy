#pragma once

/// @file   impl-DirectX\D3DStaticGeometryRenderable.h
/// @brief  DirectX implementation of StaticGeometryRenderable

#include "core-Renderer\StaticGeometryRenderable.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\Face.h"
#include "impl-DirectX\D3DAbstractGraphicalEntity.h"
#include <d3d9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * A DirectX implementation of StaticGeometryRenderable class.
 */
class D3DStaticGeometryRenderable : public StaticGeometryRenderable,
                                    public D3DAbstractGraphicalEntity<LitVertex>
{
private:
   IDirect3DDevice9& m_d3Device;

public:
   D3DStaticGeometryRenderable(Material& material, 
                               const std::vector<LitVertex>& vertices,
                               const std::vector<Face<USHORT> >& faces,
                               IDirect3DDevice9& d3Device);

   void render();

protected:
   StaticGeometryRenderable* createSelf(Material& material,
                                        const std::vector<LitVertex>& vertices,
                                        const std::vector<Face<USHORT> >& faces);
};

///////////////////////////////////////////////////////////////////////////////
