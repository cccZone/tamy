/// @file   ml-Blender/BlenderScene.h
/// @brief  a Blender scene importer

#ifndef _BLENDER_SCENE_H
#define _BLENDER_SCENE_H

#include <string>
#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class ResourcesManager;
class Model;
class Entity;
class TiXmlDocument;
class TiXmlNode;
class TiXmlElement;
class IProgressObserver;
class SpatialEntity;
class IResourceInstantiator;
class TriangleMesh;

///////////////////////////////////////////////////////////////////////////////

class BlenderScene
{
private:
   TiXmlDocument*                            m_document;
   ResourcesManager&                         m_rm;
   IProgressObserver&                        m_observer;

   std::vector< IResourceInstantiator* >     m_instantiators;

public:
   /**
    * Constructor.
    *
    * @param rm            resources manager that manages the scene resources
    * @param observer      loading progress observer
    */
   BlenderScene( const Filesystem& fs, const std::string& fileName, ResourcesManager& rm, IProgressObserver& observer );
   ~BlenderScene();

   /**
    * The method will load a scene from a Blender export file.
    *
    * @param scene         a model to which the scene should be uploaded
    */
   void load( Model& scener );

private:
   // -------------------------------------------------------------------------
   // Resources management
   // -------------------------------------------------------------------------
   /**
    * Parses the resources and creates a table of instantiators.
    */
   void parseResources( TiXmlNode* sceneSlice );

   /**
    * Releases the resource instantiators.
    */
   void releaseResourceInstantiators();

   // -------------------------------------------------------------------------
   // Objects management
   // -------------------------------------------------------------------------
   /**
    * Creates the scene objects and hooks them up with required resources.
    */
   void parseObjects( TiXmlNode* sceneSlice, Model& scene );

   /**
    * A method that creates a hierarchical object described in the specified
    * XML node.
    */
   SpatialEntity* createHierarchicalObject( TiXmlNode* rootNode );

   /**
    * Turns the data stored in an XML transform element into a transformation matrix.
    */
   void getObjectTransform( TiXmlElement* transformElem, D3DXMATRIX& outTransform ) const;
};

///////////////////////////////////////////////////////////////////////////////

#endif // _BLENDER_SCENE_H
