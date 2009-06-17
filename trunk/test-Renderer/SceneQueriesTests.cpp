#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\Camera.h"
#include "core\Triangle.h"
#include "core\MatrixWriter.h"
#include "core\Ray.h"
#include "core\Array.h"
#include "GraphicalNodeMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "TextureStub.h"
#include "MaterialOperationImplementationMock.h"
#include "LightReflectingPropertiesStub.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialOperation.h"
#include "GraphicalEntityMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, broadPhaseQueries)
{
   Camera cameraNode("camera"); 

   VisualSceneManager sceneManager(1, 100);
   sceneManager.setActiveCamera(cameraNode);

   GraphicalNodeMock staticNodeInsideQuery(false);
   GraphicalNodeMock staticNodeOutsideNode(false);
   GraphicalNodeMock dynamicNodeInsideNode(true);
   GraphicalNodeMock dynamicNodeOutsideNode(true);

   staticNodeInsideQuery.setBoundingSphereRadius(1);
   staticNodeOutsideNode.setBoundingSphereRadius(1);
   dynamicNodeInsideNode.setBoundingSphereRadius(1);
   dynamicNodeOutsideNode.setBoundingSphereRadius(1);
   D3DXMatrixTranslation(&(staticNodeOutsideNode.accessLocalMtx()), 0, 0, -10);
   D3DXMatrixTranslation(&(dynamicNodeOutsideNode.accessLocalMtx()), 0, 0, -10);

   sceneManager.addNode(&staticNodeInsideQuery);
   sceneManager.addNode(&staticNodeOutsideNode);
   sceneManager.addNode(&dynamicNodeInsideNode);
   sceneManager.addNode(&dynamicNodeOutsideNode);

   Array<AbstractGraphicalNode*> nodes;
   sceneManager.query(Ray(D3DXVECTOR3(0, 0, 0.5f), D3DXVECTOR3(0, 0, 1)), nodes);

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, nodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&staticNodeInsideQuery, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&dynamicNodeInsideNode, nodes[1]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, narrowPhaseQueries)
{
   Camera cameraNode("camera"); 

   VisualSceneManager sceneManager(1, 100);
   sceneManager.setActiveCamera(cameraNode);

   // prepare the materials
   TextureStub texture("");
   MaterialOperationImplementationMock matOpImpl;
   std::list<std::string> results;
   LightReflectingPropertiesStub lrp(results, 0);

   Material material(lrp, matOpImpl, matOpImpl, 0);
   material.addStage(new MaterialStage(texture,
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));

   // create the node we'll use for rendering
   std::vector<Material*> materials; materials.push_back(&material);
   GraphicalEntityMock entity("entity", materials);
   entity.addTriangle(Triangle(D3DXVECTOR3(-3, 1, 0), D3DXVECTOR3(3, 1, 0), D3DXVECTOR3(-3, -1, 0)));

   GraphicalNode node("node", false, entity, 0);
   node.setBoundingSphereRadius(3);
   D3DXMatrixTranslation(&(node.accessLocalMtx()), 0, 0, 10);
   sceneManager.addNode(&node);

   // run the tests
   Array<AbstractGraphicalNode*> nodes;
   
   nodes.clear();
   sceneManager.detailedQuery(Ray(D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, nodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&node, nodes[0]);

   nodes.clear();
   sceneManager.detailedQuery(Ray(D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());

   // the object has been  lifted a bit so that its bounding volume will still
   // intersect the ray, but its silhouette won't - now check to see if this
   // transformation change gets included in the query results
   D3DXMatrixTranslation(&(node.accessLocalMtx()), 0, 1, 10);
   nodes.clear();
   sceneManager.detailedQuery(Ray(D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, erroneousCase)
{
   Camera cameraNode("camera"); 

   VisualSceneManager sceneManager(1, 100);
   sceneManager.setActiveCamera(cameraNode);

   // prepare the materials
   TextureStub texture("");
   MaterialOperationImplementationMock matOpImpl;
   std::list<std::string> results;
   LightReflectingPropertiesStub lrp(results, 0);
   Material material(lrp, matOpImpl, matOpImpl, 0);
   material.addStage(new MaterialStage(texture,
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));

   // create the node we'll use for rendering
   std::vector<Material*> materials; materials.push_back(&material);
   GraphicalEntityMock entity("entity", materials);
   entity.addTriangle(Triangle(D3DXVECTOR3(0, 4, 0), 
                               D3DXVECTOR3(4, 0, 0), 
                               D3DXVECTOR3(-4, 0, 0)));

   GraphicalNode node("node", false, entity, 0);
   node.setBoundingSphereRadius(20);
   D3DXMatrixTranslation(&(node.accessLocalMtx()), 0, 0, 1);
   sceneManager.addNode(&node);

   // run the tests
   Array<AbstractGraphicalNode*> nodes;
   D3DXVECTOR3 rayDir(0, -0.4f, 0.9f);
   sceneManager.detailedQuery(Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -0.4f, 0.9f)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());

   nodes.clear();
   sceneManager.detailedQuery(Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.3f, 0, 0.95f)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());
}

///////////////////////////////////////////////////////////////////////////////
