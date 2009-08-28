#pragma once

/// @file      core-ResourceManagement\IWFScene.h
/// @brief     a storage that gets initialized by the IWFLoader

#include <string>
#include <vector>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

struct Color;
struct MeshDefinition;
struct MaterialDefinition;

///////////////////////////////////////////////////////////////////////////////

enum IWFLightType
{
   LT_POINT,
   LT_SPOT,
   LT_DIRECTIONAL
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a storage that will be initialized with the contents
 * of an IWF file loaded by the IWFLoader class.
 */
class IWFScene
{
public:
   virtual ~IWFScene() {}

   /**
    * Adds a new light to the scene.
    *
    * @param name       light name
    * @param ambient    light ambient color
    * @param diffuse    light diffuse color
    * @param specular   light specular color
    * @param lightType  type of the light (spot, point, directional)
    * @param range      how far does the light emit its rays
    * @param constAtt   constant attenuation factor
    * @param linearAtt  linear attenuation factor
    * @param fallOff    spotlight's inner-to-outer cone attenuation factor
    * @param theta      spotlight's inner cone angle
    * @param phi        spotlight's outer cone angle
    * @param situation  matrix describing the positon and orientation 
    *                   of the light
    */
   virtual void addLight(const std::string& name,
                         const Color& ambient, 
                         const Color& diffuse, 
                         const Color& specular,
                         IWFLightType lightType,
                         float range,
                         float constAtt, float linearAtt, float quadAtt,
                         float fallOff, float theta, float phi,
                         const D3DXMATRIX& situation) = 0;

   /** 
    * Adds a new sky box to the scene.
    * 
    * @param frontTex   front side texture
    * @param backTex    back side texture
    * @param leftTex    left side texture
    * @param rightTex   right side texture
    * @param topTex     top side texture
    * @param bottomTex  bottom side texture
    */
   virtual void addSkyBox(const std::string& frontTex,
                          const std::string& backTex,
                          const std::string& leftTex,
                          const std::string& rightTex,
                          const std::string& topTex,
                          const std::string& bottomTex) = 0;

   /**
    * Adds a new dynamic mesh to the scene.
    * 
    * @param meshFileName     name of the file that contains the mesh
    * @param situation        matrix describing the positon and orientation 
    *                         of the object
    */
   virtual void addDynamicMesh(const std::string& meshFileName, 
                               const D3DXMATRIX& situation) = 0;

   /**
    * Adds a new static mesh to the scene.
    * 
    * @param mesh             mesh data
    * @param materials        mesh materials
    * @param situation        matrix describing the positon and orientation 
    *                         of the mesh
    */
   virtual void addStaticGeometry(MeshDefinition& mesh, 
                                  std::vector<MaterialDefinition>& materials,
                                  const D3DXMATRIX& situation) = 0;
};

///////////////////////////////////////////////////////////////////////////////
