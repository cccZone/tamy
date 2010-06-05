#include "dx9-Renderer\DX9TriangleMesh.h"
#include "dx9-Renderer\DX9Renderer.h"
#include <stdexcept>

#include "core-Renderer\LitVertex.h"

///////////////////////////////////////////////////////////////////////////////

DX9TriangleMesh::DX9TriangleMesh(TriangleMesh& mesh)
: m_mesh(mesh)
, m_dxMesh(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

DX9TriangleMesh::~DX9TriangleMesh()
{
   if (m_dxMesh != NULL)
   {
      m_dxMesh->Release();
      m_dxMesh = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9TriangleMesh::render()
{
   if (m_dxMesh != NULL)
   {
      m_dxMesh->DrawSubset(0);
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9TriangleMesh::initialize(Renderer& renderer)
{
   DX9Renderer* d3dRenderer = dynamic_cast<DX9Renderer*> (&renderer);
   if (d3dRenderer == NULL)
   {
      throw std::runtime_error("This class works only with DX9Renderer instance");
   }

   if (m_dxMesh != NULL)
   {
      m_dxMesh->Release();
      m_dxMesh = NULL;
   }

   VertexArray* vertices = m_mesh.getGenericVertexArray();
   const std::vector<Face>& faces = m_mesh.getFaces();
   unsigned int verticesCount = vertices->size();
   unsigned int trianglesCount = faces.size();

   if ((verticesCount == 0) || (trianglesCount == 0))
   {
      delete vertices;
      return;
   }

   DWORD FVF = vertices->getFVF();
   HRESULT res = D3DXCreateMeshFVF(trianglesCount, verticesCount, 
                                   D3DXMESH_MANAGED, FVF, 
                                   &(d3dRenderer->getD3Device()), &m_dxMesh);
   if (FAILED(res)) 
   { 
      throw std::logic_error("Can't create a mesh"); 
   }

   // fill the vertex buffer, analyze the bounding sphere radius on the way
   void* pVertex = NULL;
   res = m_dxMesh->LockVertexBuffer(0, (void**)&pVertex);
   if (FAILED(res)) 
   { 
      throw std::logic_error("Can't lock the mesh's vertex buffer");
   }
   vertices->copyTo(pVertex);
   m_dxMesh->UnlockVertexBuffer();

   // fill the index buffer & the attributes table
   USHORT* pIndex = NULL;
   DWORD* pAttrib = NULL;
   res = m_dxMesh->LockIndexBuffer(0, (void**)&pIndex);
   if (FAILED(res)) 
   { 
      throw std::logic_error("Can't lock the mesh's index buffer"); 
   }
   res = m_dxMesh->LockAttributeBuffer(0, &pAttrib);
   if (FAILED(res)) 
   { 
      throw std::logic_error("Can't lock the mesh's attributes buffer");
   }

   for (unsigned int i = 0; i < trianglesCount; ++i)
   {
      const Face& face = faces[i];
      *pIndex++  = face.idx[0]; 
      *pIndex++  = face.idx[1]; 
      *pIndex++  = face.idx[2];
      *pAttrib++ = 0;
   }
   m_dxMesh->UnlockIndexBuffer();
   m_dxMesh->UnlockAttributeBuffer();

   if (trianglesCount > 2)
   {
      DWORD *adjacency = new DWORD[m_dxMesh->GetNumFaces() * 3];
      m_dxMesh->GenerateAdjacency(0.001f, adjacency);
      m_dxMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE, adjacency, NULL, NULL, NULL);
      delete [] adjacency;
   }

   delete vertices;
}

///////////////////////////////////////////////////////////////////////////////
