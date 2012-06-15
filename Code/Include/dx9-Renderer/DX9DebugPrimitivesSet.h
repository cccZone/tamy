/// @file   dx9-Renderer/DX9DebugPrimitivesSet.h
/// @brief  a helper class that renders a specific type of debug primitives 
#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "core/types.h"
#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A debug vertex definition.
 */
struct DebugVertex
{
   D3DXVECTOR3       m_vtx;
   DWORD             m_color;
};

/////////////////////////////////////////////////////////////////////////////


class DX9DebugPrimitivesSet
{
public:
   virtual ~DX9DebugPrimitivesSet() {}

   /**
    * Call when rendering process starts.
    */
   virtual void begin() = 0;

   /**
    * Call when rendering process ends.
    */
   virtual void end() = 0;

   /**
    * Call in order to draw the collected entities.
    */
   virtual void draw( IDirect3DDevice9* d3Device ) = 0;
};

/////////////////////////////////////////////////////////////////////////////

class DX9RegularDebugPrimitivesSet : public DX9DebugPrimitivesSet
{
public:
   IDirect3DVertexBuffer9*                         m_buffer;
   DebugVertex*                                    m_pVertices;
   uint                                            m_primitivesCount;

private:
   uint                                            m_maxVertices;
   D3DPRIMITIVETYPE                                m_primitiveType;
   bool                                            m_useZBuffer;

public:

   /**
    * Constructor.
    *
    * @param renderer
    * @param primitiveType    type of the drawn primitive
    * @param useZBuffer
    * @param maxPrimitives    maximum number of primitives this set can contain
    */
   DX9RegularDebugPrimitivesSet( DX9Renderer& renderer, D3DPRIMITIVETYPE primitiveType, bool useZBuffer, uint maxPrimitives );
   ~DX9RegularDebugPrimitivesSet();
   
   // -------------------------------------------------------------------------
   // DX9DebugPrimitivesSet implementation
   // -------------------------------------------------------------------------
   void begin();
   void end();
   void draw( IDirect3DDevice9* d3Device );
};

/////////////////////////////////////////////////////////////////////////////

class DX9IndexedDebugPrimitivesSet : public DX9DebugPrimitivesSet
{
public:
   IDirect3DIndexBuffer9*                          m_idxBuffer;
   IDirect3DVertexBuffer9*                         m_vtxBuffer;
   DebugVertex*                                    m_pVertices;
   word*                                           m_pIndices;
   uint                                            m_verticesCount;
   uint                                            m_indicesCount;

   struct Mesh
   {
      word                                         m_firstVertexIdx;
      word                                         m_firstIndexIdx;
      word                                         m_indicesCount;
   };
   const word                                      MAX_MESHES;
   Array< Mesh* >                                  m_meshes;
   word                                            m_meshesCount;

private:
   uint                                            m_maxVertices;
   uint                                            m_maxIndices;
   D3DPRIMITIVETYPE                                m_primitiveType;
   bool                                            m_useZBuffer;

public:

   /**
    * Constructor.
    *
    * @param renderer
    * @param primitiveType    type of the drawn primitive
    * @param useZBuffer
    * @param maxPrimitives    maximum number of primitives this set can contain
    */
   DX9IndexedDebugPrimitivesSet( DX9Renderer& renderer, D3DPRIMITIVETYPE primitiveType, bool useZBuffer, uint maxPrimitives );
   ~DX9IndexedDebugPrimitivesSet();
   
   /**
    * Call this method when you want to add a new mesh.
    *
    * @param indicesCount
    */
   void addMesh( uint indicesCount );

   // -------------------------------------------------------------------------
   // DX9DebugPrimitivesSet implementation
   // -------------------------------------------------------------------------
   void begin();
   void end();
   void draw( IDirect3DDevice9* d3Device );
};

/////////////////////////////////////////////////////////////////////////////
