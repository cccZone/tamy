/// @file   TamyEditor/DebugGeometry.h
/// @brief  a helper geometry class for rendering simple debug shapes
#pragma once

#include "core-Renderer\Geometry.h"


///////////////////////////////////////////////////////////////////////////////

class VertexShader;

///////////////////////////////////////////////////////////////////////////////

class DebugGeometry : public Geometry
{
   DECLARE_CLASS();

private:
   VertexShader*           m_vertexShader;
   void*                   m_representedObjPtr;

public:
   /**
    * Constructor.
    *
    * @param representedObjPtr         pointer to the represented object
    */
   DebugGeometry( void* representedObjPtr );
   ~DebugGeometry();

   /**
    * Returns a pointer to the represented object.
    */
   inline void* getRepresentedObjectPtr() { return m_representedObjPtr; }

   // -------------------------------------------------------------------------
   // Geometry implementation
   // -------------------------------------------------------------------------
   const BoundingVolume& getBoundingVolume() const;
   bool onPreRender( Renderer& renderer );
   void onPostRender( Renderer& renderer );

};

///////////////////////////////////////////////////////////////////////////////
