#include "ml-Blender/ControllerCS.h"
#include "core-Renderer.h"
#include "core.h"
#include "tinyxml/tinyxml.h"
#include <stdexcept>

#include "ml-Blender/ColladaParser.h"
#include "ml-Blender/DSBindStructure.h"
#include "ml-Blender/DSIndexedBindStructure.h"
#include "ml-Blender/DSIndexed.h"
#include "ml-Blender/MeshCS.h"
#include "ml-Blender/BlenderScene.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class BindShapeMatrix : public DataSource
   {
      D3DXMATRIX     m_mtx;

   public:
      BindShapeMatrix( TiXmlElement& elem, const ColladaParser& sourcesDB )
      {
         std::string val = elem.GetText();
         std::stringstream valStr( val );
         valStr   >> m_mtx._11 >> m_mtx._12 >> m_mtx._13 >> m_mtx._14
                  >> m_mtx._21 >> m_mtx._22 >> m_mtx._23 >> m_mtx._24
                  >> m_mtx._31 >> m_mtx._32 >> m_mtx._33 >> m_mtx._34
                  >> m_mtx._41 >> m_mtx._42 >> m_mtx._43 >> m_mtx._44;
      }

      unsigned int size() const { return 1; }

      void readData( int dataOffset, void* data )
      {
         D3DXMATRIX& mtx = *( reinterpret_cast< D3DXMATRIX* >( data ) );
         mtx = m_mtx;
      }
   };

   // -------------------------------------------------------------------------

   class DSJoints : public DSBindStructure
   {
   public:
      DSJoints( TiXmlElement& elem, const ColladaParser& sourcesDB )
         : DSBindStructure( elem, sourcesDB )
      {
      }

   protected:
      void readData( int dataOffset, void* data )
      {
         Skeleton* skeleton = reinterpret_cast< Skeleton* >( data );

         std::string boneName;
         D3DXMATRIX invBindMtx;
         getSource( "JOINT" ).readData( dataOffset, &boneName );
         getSource( "INV_BIND_MATRIX" ).readData( dataOffset, &invBindMtx );
         D3DXMatrixTranspose( &invBindMtx, &invBindMtx );

         skeleton->setTransformation( boneName, invBindMtx );
      }
   };

   // -------------------------------------------------------------------------

   class DSVertexWeights : public DSIndexedBindStructure
   {
   public:
      DSVertexWeights( TiXmlElement& elem, const ColladaParser& sourcesDB )
         : DSIndexedBindStructure( elem, sourcesDB )
      {
      }

      void readData( int dataOffset, void* data )
      {
         Skeleton* skeleton = reinterpret_cast< Skeleton* >( data );
         
         unsigned int indicesCount = getIndicesCount( dataOffset );
         for ( unsigned int i = 0; i < indicesCount; ++i )
         {
            unsigned int boneIdx = getIndex( dataOffset, "JOINT", i );
            unsigned int weightIdx = getIndex( dataOffset, "WEIGHT", i );

            std::string boneName;
            float weight;
            getSource( "JOINT" ).readData( boneIdx, &boneName );
            getSource( "WEIGHT" ).readData( weightIdx, &weight );

            skeleton->addWeight( dataOffset, boneName, weight );
         }
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

ControllerCS::ControllerCS( TiXmlNode* controllerNode, ResourcesManager& rm )
{
   if ( !controllerNode )
   {
      throw std::invalid_argument( "NULL pointer instead a valid TiXmlNode mesh node specified" );
   }

   TiXmlElement* controllerElem = controllerNode->ToElement();
   ASSERT( controllerElem != NULL );

   std::string controllerName = controllerElem->Attribute( "id" );

   TiXmlElement* skinElem = controllerElem->FirstChildElement( "skin" );
   ASSERT( skinElem != NULL );

   m_skinSource = skinElem->Attribute( "source" );
   m_skinSource = m_skinSource.substr( 1 );

   ColladaParser parser( *skinElem );
   parser.associate< BindShapeMatrix >( "bind_shape_matrix" );
   parser.associate< DSJoints >( "joints" );
   parser.associate< DSVertexWeights >( "vertex_weights" );

   parser.initialize();   

   // create a skeleton resource
   m_skeleton = new Skeleton( controllerName );

   // initialize the skeleton resource
   DataSource* skeletonData = parser.getSource( "joints" );
   unsigned int count = skeletonData->size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      skeletonData->readData( i, m_skeleton );
   }

   DataSource* vertexWeightData = parser.getSource( "vertex_weights" );
   count = vertexWeightData->size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      vertexWeightData->readData( i, m_skeleton );
   }

   // store the vertex transformation data
   D3DXMATRIX bindShapeMtx;
   DataSource* bindShapeMtxData = parser.getSource( "bind_shape_matrix" );
   bindShapeMtxData->readData( 0, &bindShapeMtx );
   D3DXMatrixTranspose( &bindShapeMtx, &bindShapeMtx );
   m_skeleton->setShapeBindMatrix( bindShapeMtx );

   // add the skeleton resource to the resources manager
   rm.addResource( m_skeleton );
}

///////////////////////////////////////////////////////////////////////////////

Entity* ControllerCS::instantiate( const BlenderScene& host ) const
{
   TriangleMesh& skinMesh = host.getResource< TriangleMesh >( m_skinSource );
   return new SkinnedGeometry( skinMesh, *m_skeleton );
}

///////////////////////////////////////////////////////////////////////////////
