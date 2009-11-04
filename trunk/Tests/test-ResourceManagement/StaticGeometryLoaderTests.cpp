#include "core-TestFramework\TestFramework.h"
#include "core-ResourceManagement\StaticGeometryLoader.h"
#include "core-ResourceManagement\GraphicalDataSource.h"
#include "core-ResourceManagement\StaticGeometry.h"
#include "core\ResourceStorage.h"
#include "core\Filesystem.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\ManagedTexture.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class RendererStub : public Renderer
   {
   protected:
      void resetViewport(unsigned int width, unsigned int height) {}

      void renderingBegin() {}

      void renderingEnd() {}

      bool isGraphicsSystemReady() const {return true;}

      void attemptToRecoverGraphicsSystem() {}

      void cleanAllTargets(unsigned int count) {}
   };

   // -------------------------------------------------------------------------

   template<>
   class TTextureImplDestructor<int>
   {
   public:
      void destroy(int* tex) {}
   };

   // -------------------------------------------------------------------------

   class GraphicalEntitiesFactoryMock : public GraphicalEntitiesFactory
   {
   private:
      static RendererStub s_renderer;
      static Filesystem s_filesystem;

   public:
      GraphicalEntitiesFactoryMock() : GraphicalEntitiesFactory(s_filesystem, s_renderer) {}

      Light* createLight(const std::string& name) {return NULL;}

      ParticleSystem* createParticleSystem(const std::string& name, 
                                           Material& material,
                                           unsigned int particlesCount)
      { return NULL; }

      RendererImpl* createFixedRendererImpl() { return NULL; }

      GraphicalEffect* createEffect(const std::string& name) { return NULL; }

      SkyBoxSide* createSkyBoxSide(SkyBoxSideId side, Texture* tex)  { return NULL; }

      RenderingTargetsCleaner* createTargetsCleaner() { return NULL; }

      Font* createFont(const D3DXFONT_DESC& fontDesc) {return NULL;}

   protected:
      GraphicalEntity* createGraphicalEntity(const std::string& name,
                                             const std::vector<LitVertex>& vertices,
                                             const std::vector<Face<USHORT> >& faces,
                                             const std::vector<Material*>& materials)
      { return NULL; }

      SkinnedGraphicalEntity* createSkinnedGraphicalEntity(const std::string& name,
                                                           const std::vector<LitVertex>& vertices,
                                                           const std::vector<Face<USHORT> >& faces,
                                                           const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                           const std::vector<SkinBoneDefinition>& skinBones,
                                                           const std::vector<Material*>& materials)
      { return NULL; }

      StaticGeometryRenderable* createStaticGeometry(Material& material,
                                                     const std::vector<LitVertex>& vertices,
                                                     const std::vector<Face<USHORT> >& faces)
      { return NULL; }

      Texture* loadTexture(const std::string& fileName) { return NULL; }

      Texture* createEmptyTexture() { return new ManagedTexture("", new TTextureImpl<int>()); }

      RenderingTarget* createDefaultRenderingTarget() {return NULL;}

      TextureRenderingTarget* createTextureRenderingTarget(const std::string& name) {return NULL;}

      PostProcessEffectRenderable* createPostProcessEffectRenderable(void) { return NULL; }
   };

   RendererStub GraphicalEntitiesFactoryMock::s_renderer;
   Filesystem GraphicalEntitiesFactoryMock::s_filesystem;

   // -------------------------------------------------------------------------

   class StaticGeometryMock : public StaticGeometry
   {
   private:
      std::vector<std::vector<LitVertex> > m_vertices;
      std::vector<std::vector<Face<USHORT> > > m_faces;
      std::vector<unsigned int> m_materials;

   public:

      int getRenderablesCount()
      {
         return m_materials.size();
      }

      const std::vector<LitVertex>& getVertices(unsigned int renderableIdx) const
      {
         return m_vertices.at(renderableIdx);
      }

      const std::vector<Face<USHORT> >& getFaces(unsigned int renderableIdx) const
      {
         return m_faces.at(renderableIdx);
      }

      void addMesh(const std::vector<LitVertex>& vertices,
                   const std::vector<Face<USHORT> >& faces,
                   Material& material)
      {
         m_materials.push_back(material.getIndex());
         m_vertices.push_back(vertices);
         m_faces.push_back(faces);
      }
   };
   
} // anonymous namespace

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryLoader, loadingSimpleMesh)
{
   class GraphicalDataSourceMock : public GraphicalDataSource
   {
      void parseMesh(MeshDefinition& mesh, 
                     AnimationDefinition& animation, 
                     std::vector<MaterialDefinition>& materials,
                     const std::string& name)
      {
         mesh.materials.push_back("material");
         materials.push_back(MaterialDefinition("material"));

         mesh.vertices.push_back(LitVertex::unskinnedNoTex(-1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 0, 1, 0, 0, 0, 1));

         mesh.faces.push_back(Face<USHORT> (0, 1, 2, 0));
      }
   };


   ResourceStorage<Material> materialsStorage;
   GraphicalEntitiesFactoryMock factory;

   StaticGeometryLoader loader(factory);

   GraphicalDataSourceMock source;
   StaticGeometryMock output;
   loader.load(materialsStorage, source, output);

   CPPUNIT_ASSERT_EQUAL(1, output.getRenderablesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, output.getFaces(0).size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryLoader, loadingSimpleMeshWithTwoMaterials)
{
   class GraphicalDataSourceMock : public GraphicalDataSource
   {
      void parseMesh(MeshDefinition& mesh, 
                     AnimationDefinition& animation, 
                     std::vector<MaterialDefinition>& materials,
                     const std::string& name)
      {
         mesh.materials.push_back("material1");
         mesh.materials.push_back("material2");
         materials.push_back(MaterialDefinition("material1"));
         materials.push_back(MaterialDefinition("material2"));

         mesh.vertices.push_back(LitVertex::unskinnedNoTex(-1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 0, 1, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex(-1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 0, -1, 0, 0, 0, 1));

         mesh.faces.push_back(Face<USHORT> (0, 1, 2, 0));
         mesh.faces.push_back(Face<USHORT> (3, 4, 5, 1));
      }
   };


   ResourceStorage<Material> materialsStorage;
   GraphicalEntitiesFactoryMock factory;

   StaticGeometryLoader loader(factory);

   GraphicalDataSourceMock source;
   StaticGeometryMock output;
   loader.load(materialsStorage, source, output);

   CPPUNIT_ASSERT_EQUAL(2, output.getRenderablesCount());

   // check the first renderable
   const std::vector<LitVertex>& vert1 = output.getVertices(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, vert1.size());
   COMPARE_VEC(D3DXVECTOR3(-1, 0, 0), vert1[0].m_coords);
   COMPARE_VEC(D3DXVECTOR3( 1, 0, 0), vert1[1].m_coords);
   COMPARE_VEC(D3DXVECTOR3( 0, 1, 0), vert1[2].m_coords);

   const std::vector<Face<USHORT> >& faces1 = output.getFaces(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, faces1.size());
   CPPUNIT_ASSERT_EQUAL((USHORT)0, faces1[0].idx[0]);
   CPPUNIT_ASSERT_EQUAL((USHORT)1, faces1[0].idx[1]);
   CPPUNIT_ASSERT_EQUAL((USHORT)2, faces1[0].idx[2]);
   CPPUNIT_ASSERT_EQUAL((DWORD)0, faces1[0].subsetID);

   // check the second renderable
   const std::vector<LitVertex>& vert2 = output.getVertices(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, vert2.size());
   COMPARE_VEC(D3DXVECTOR3(-1, 0, 0), vert2[0].m_coords);
   COMPARE_VEC(D3DXVECTOR3( 1, 0, 0), vert2[1].m_coords);
   COMPARE_VEC(D3DXVECTOR3( 0, -1, 0), vert2[2].m_coords);

   const std::vector<Face<USHORT> >& faces2 = output.getFaces(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, faces2.size());
   CPPUNIT_ASSERT_EQUAL((USHORT)0, faces2[0].idx[0]);
   CPPUNIT_ASSERT_EQUAL((USHORT)1, faces2[0].idx[1]);
   CPPUNIT_ASSERT_EQUAL((USHORT)2, faces2[0].idx[2]);
   CPPUNIT_ASSERT_EQUAL((DWORD)0, faces2[0].subsetID);
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryLoader, hierarchicalMeshWithTwoMaterials)
{
   class GraphicalDataSourceMock : public GraphicalDataSource
   {
      void parseMesh(MeshDefinition& mesh, 
                     AnimationDefinition& animation, 
                     std::vector<MaterialDefinition>& materials,
                     const std::string& name)
      {
         // root
         mesh.materials.push_back("material1");
         mesh.materials.push_back("material2");
         materials.push_back(MaterialDefinition("material1"));
         materials.push_back(MaterialDefinition("material2"));

         mesh.vertices.push_back(LitVertex::unskinnedNoTex(-1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 0, 1, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex(-1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 0, -1, 0, 0, 0, 1));

         mesh.faces.push_back(Face<USHORT> (0, 1, 2, 0));
         mesh.faces.push_back(Face<USHORT> (3, 4, 5, 1));

         D3DXMatrixTranslation(&(mesh.localMtx), 10, 20, 30);

         // child
         MeshDefinition* child = new MeshDefinition();
         child->materials.push_back("material2"); // we're using material that's mapped to subset 1 in 
                                                 // the parent mesh (here it is mapped to subset 2 - let's
                                                 // see if it gets sorted out)

         child->vertices.push_back(LitVertex::unskinnedNoTex(-1, 0, 0, 0, 0, 1));
         child->vertices.push_back(LitVertex::unskinnedNoTex( 1, 0, 0, 0, 0, 1));
         child->vertices.push_back(LitVertex::unskinnedNoTex( 0, 1, 0, 0, 0, 1));

         child->faces.push_back(Face<USHORT> (0, 1, 2, 0));
         D3DXMatrixTranslation(&(child->localMtx), 5, 0, 0);

         mesh.children.push_back(child);
      }
   };


   ResourceStorage<Material> materialsStorage;
   GraphicalEntitiesFactoryMock factory;

   StaticGeometryLoader loader(factory);

   GraphicalDataSourceMock source;
   StaticGeometryMock output;
   loader.load(materialsStorage, source, output);

   CPPUNIT_ASSERT_EQUAL(2, output.getRenderablesCount());

   // check the first renderable
   const std::vector<LitVertex>& vert1 = output.getVertices(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, vert1.size());
   COMPARE_VEC(D3DXVECTOR3( 9, 20, 30), vert1[0].m_coords);
   COMPARE_VEC(D3DXVECTOR3(11, 20, 30), vert1[1].m_coords);
   COMPARE_VEC(D3DXVECTOR3(10, 21, 30), vert1[2].m_coords);

   const std::vector<Face<USHORT> >& faces1 = output.getFaces(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, faces1.size());
   CPPUNIT_ASSERT_EQUAL((USHORT)0, faces1[0].idx[0]);
   CPPUNIT_ASSERT_EQUAL((USHORT)1, faces1[0].idx[1]);
   CPPUNIT_ASSERT_EQUAL((USHORT)2, faces1[0].idx[2]);
   CPPUNIT_ASSERT_EQUAL((DWORD)0, faces1[0].subsetID);

   // check the second renderable
   const std::vector<LitVertex>& vert2 = output.getVertices(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)6, vert2.size());
   COMPARE_VEC(D3DXVECTOR3( 9, 20, 30), vert2[0].m_coords);
   COMPARE_VEC(D3DXVECTOR3(11, 20, 30), vert2[1].m_coords);
   COMPARE_VEC(D3DXVECTOR3(10, 19, 30), vert2[2].m_coords);
   COMPARE_VEC(D3DXVECTOR3(14, 20, 30), vert2[3].m_coords);
   COMPARE_VEC(D3DXVECTOR3(16, 20, 30), vert2[4].m_coords);
   COMPARE_VEC(D3DXVECTOR3(15, 21, 30), vert2[5].m_coords);

   const std::vector<Face<USHORT> >& faces2 = output.getFaces(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, faces2.size());
   CPPUNIT_ASSERT_EQUAL((USHORT)0, faces2[0].idx[0]);
   CPPUNIT_ASSERT_EQUAL((USHORT)1, faces2[0].idx[1]);
   CPPUNIT_ASSERT_EQUAL((USHORT)2, faces2[0].idx[2]);
   CPPUNIT_ASSERT_EQUAL((DWORD)0,  faces2[0].subsetID);
   CPPUNIT_ASSERT_EQUAL((USHORT)3, faces2[1].idx[0]);
   CPPUNIT_ASSERT_EQUAL((USHORT)4, faces2[1].idx[1]);
   CPPUNIT_ASSERT_EQUAL((USHORT)5, faces2[1].idx[2]);
   CPPUNIT_ASSERT_EQUAL((DWORD)0,  faces2[1].subsetID);
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryLoader, sameVertexUsedForTwoFacesDoesntGetDuplicated)
{
   class GraphicalDataSourceMock : public GraphicalDataSource
   {
      void parseMesh(MeshDefinition& mesh, 
                     AnimationDefinition& animation, 
                     std::vector<MaterialDefinition>& materials,
                     const std::string& name)
      {
         mesh.materials.push_back("material");
         materials.push_back(MaterialDefinition("material"));

         mesh.vertices.push_back(LitVertex::unskinnedNoTex(-1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 1, 0, 0, 0, 0, 1));
         mesh.vertices.push_back(LitVertex::unskinnedNoTex( 0, 1, 0, 0, 0, 1));

         mesh.faces.push_back(Face<USHORT> (0, 1, 2, 0));
         mesh.faces.push_back(Face<USHORT> (2, 1, 0, 0));
      }
   };


   ResourceStorage<Material> materialsStorage;
   GraphicalEntitiesFactoryMock factory;

   StaticGeometryLoader loader(factory);

   GraphicalDataSourceMock source;
   StaticGeometryMock output;
   loader.load(materialsStorage, source, output);

   CPPUNIT_ASSERT_EQUAL(1, output.getRenderablesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, output.getVertices(0).size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, output.getFaces(0).size());
}

///////////////////////////////////////////////////////////////////////////////
