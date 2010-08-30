/// @file   ml-Blender/IColladaSlice.h
/// @brief  a slice parser interface

#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

class Entity;
class TiXmlNode;
class BlenderScene;
class Model;

///////////////////////////////////////////////////////////////////////////////

/**
 * A resource instantiation interface.
 */
class IColladaSlice
{
public:
   virtual ~IColladaSlice() {}
};

///////////////////////////////////////////////////////////////////////////////

class IResourceSlice
{
public:
   virtual ~IResourceSlice() {}

   template< typename T >
   T& getResource() const
   {
      T* res = reinterpret_cast< T* >( getResourcePtr() );
      return *res;
   }

protected:
   virtual void* getResourcePtr() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

template < typename T >
class TResourceSlice : public IResourceSlice
{
public:
   virtual ~TResourceSlice() {} 
};

///////////////////////////////////////////////////////////////////////////////

class IEntitySlice
{
public:
   virtual ~IEntitySlice() {}

   virtual Entity* instantiate( const BlenderScene& host ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

class ISceneSlice
{
protected:
   std::string    m_id;

public:
   virtual ~ISceneSlice() {}

   virtual void instantiate( const BlenderScene& host, Model& scene ) const = 0;

   inline void setID( const std::string& id ) { m_id = id; }
};

///////////////////////////////////////////////////////////////////////////////
