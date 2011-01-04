/// @file   ml-Blender/BlenderScene.h
/// @brief  a Blender scene importer

#ifndef _BLENDER_SCENE_H
#define _BLENDER_SCENE_H

#include <string>
#include <d3dx9.h>
#include <vector>
#include <map>


///////////////////////////////////////////////////////////////////////////////

class Model;
class Filesystem;
class ResourcesManager;
class TiXmlDocument;
class TiXmlNode;
class TiXmlElement;
class IProgressObserver;
class SliceDefinition;
class IColladaSlice;
class ISceneSlice;
class Entity;

///////////////////////////////////////////////////////////////////////////////

class BlenderScene
{
private:
   TiXmlDocument*                                  m_document;
   ResourcesManager&                               m_rm;
   IProgressObserver&                              m_observer;

   typedef std::vector< SliceDefinition* >         SlicesDefinitions;
   SlicesDefinitions                               m_definitions;

   typedef std::map< std::string, IColladaSlice* > SlicesMap; 
   SlicesMap                                       m_slicesMap;

   typedef std::vector< ISceneSlice* >             SceneSlicesMap;
   SceneSlicesMap                                  m_sceneSlicesMap;

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
   void load( Model& scene );

   /**
    * Returns the used instance of progress observer.
    */
   inline IProgressObserver& getObserver() { return m_observer; }

   /**
    * Returns the used instance of resources manager.
    */
   inline ResourcesManager& getResourcesManager() { return m_rm; }

   // -------------------------------------------------------------------------
   // Slices management
   // -------------------------------------------------------------------------

   /**
    * Adds a new scene slice in the process of slice parsing.
    *
    * @param id       slice id
    * @param slice
    */
   void addSceneSlice( ISceneSlice* slice );

   /**
    * Adds a new slice in the process of slice parsing.
    *
    * @param id       slice id
    * @param slice
    */
   void addSlice( const std::string& id, IColladaSlice* slice );

   /**
    * Returns a reference to the required resource instance, if one exists.
    *
    * @param id       resource id
    */
   template < typename T >
   T& getResource( const std::string& id ) const;

   /**
    * Returns a new instance of the required entity, if one exists.
    *
    * @param id       entity id
    */
   Entity* getEntity( const std::string& id ) const;
};

///////////////////////////////////////////////////////////////////////////////

class SliceDefinition
{
public:
   virtual ~SliceDefinition() {}

   virtual void parse( TiXmlNode* sceneSlice, BlenderScene& host ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

template < typename T >
class TSliceDefinition : public SliceDefinition
{
private:
   std::string    m_sliceTag;
   std::string    m_sectionTag;

public:
   TSliceDefinition( const std::string& sliceTag, const std::string& sectionTag );

   // -------------------------------------------------------------------------
   // SliceDefinition implementation
   // -------------------------------------------------------------------------
   void parse( TiXmlNode* sceneSlice, BlenderScene& host );
};

///////////////////////////////////////////////////////////////////////////////

#include "ml-Blender/BlenderScene.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _BLENDER_SCENE_H