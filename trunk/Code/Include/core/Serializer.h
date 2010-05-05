#ifndef _SERIALIZER_H
#define _SERIALIZER_H

/// @file   core\Serializer.h
/// @brief  transactional serializer

#include "core\UniqueObject.h"
#include <string>
#include <vector>
#include <list>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Serializable;
class SerializerImpl;
class Resource;
class ResourcesManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class gives the serializables abilities to serialize
 * their state to an external device.
 *
 * Serialization is a transactional process, and a single serializer
 * life cycle can be thought of as a whole transaction - the transaction
 * is opened when it's created, and closed when it's deleted.
 */
class Serializer
{
protected:
   enum PointerSaveState
   {
      PSS_SAVE,
      PSS_LOAD,
      PSS_UPDATE,
   };

   enum DependencyType
   {
      DPT_INTERNAL = 1,
      DPT_EXTERNAL = 2
   };

public:
   virtual ~Serializer() {}

   /**
    * The method serializes a solid type value.
    *
    * @param val     value to serialize
    */
   template<typename T>
   void operator<<(T& val)
   {
      serializeBuf((byte*)&val, sizeof(val));
   }

   /**
    * The method serializes a string.
    *
    * @param str     string to serialize
    */
   virtual void operator<<(std::string& str) = 0;

   /**
    * The method serializes a matrix.
    *
    * @param mtx     matrix to serialize
    */
   virtual void operator<<(D3DXMATRIX& mtx) = 0;

   /**
    * The method serializes a vector.
    *
    * @param mtx     vector to serialize
    */
   virtual void operator<<(D3DXVECTOR3& vec) = 0;

   /**
    * The method serializes a pointer to another serializable
    */
   virtual void operator<<(Serializable** ptr) = 0;

   /**
    * Method serializing the contents of a vector, if the vector contains non-pointers
    */
   template<typename T>
   void operator<<(std::vector<T>& vec);

   /**
    * Method serializing the contents of a vector, if the vector contains pointers.
    */
   template<typename T>
   void operator<<(std::vector<T*>& vec);

protected:
   /**
    * Serializes a buffer of bytes.
    *
    * @param buf
    * @param size    number of bytes in the buffer
    */
   virtual void serializeBuf(byte* buf, size_t size) = 0;

   virtual PointerSaveState getState() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::vector<Resource*>      ExternalDependenciesSet;
typedef std::vector<Serializable*>  InternalDependenciesSet;

///////////////////////////////////////////////////////////////////////////////

/**
 * An internal class used in the process of serialization to find 
 * all the dependencies between the objects (pointers) and save/restore
 * their status.
 */
class DependenciesMapper : public Serializer
{
private:
   InternalDependenciesSet&      m_internalDependencies;
   PointerSaveState              m_state;
  
public:
   DependenciesMapper( InternalDependenciesSet& internalDependencies, PointerSaveState state );

   // -------------------------------------------------------------------------
   // Serializer implementation
   // -------------------------------------------------------------------------
   void operator<<(std::string& str);
   void operator<<(D3DXMATRIX& mtx);
   void operator<<(D3DXVECTOR3& vec);
   void operator<<(Serializable** ptr);

protected:
   void serializeBuf(byte* buf, size_t size);

   PointerSaveState getState() const { return m_state; }

private:
   template< typename T >
   void saveDependency( typename std::vector< T* >& depsMap, T* ptr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Saves a hierarchy of objects using the specified serialization implementation.
 */
class Saver : public Serializer
{
private:
   SerializerImpl*               m_impl;
   ExternalDependenciesSet*      m_externalDependencies;
   InternalDependenciesSet       m_internalDependencies;

public:
   Saver( SerializerImpl* impl );
   ~Saver();

   /**
    * Use this method to save a resource capable of being serialized and
    * learn about other resources that it uses that may need to be saved as well.
    *
    * @param resource                  resource to save
    * @param outExternalDependencies   other resources used by this resource
    */
   void save( Resource& resource, ExternalDependenciesSet& outExternalDependencies = ExternalDependenciesSet() );

   // -------------------------------------------------------------------------
   // Serializer implementation
   // -------------------------------------------------------------------------
   PointerSaveState getState() const { return PSS_SAVE; }
   void operator<<(std::string& str);
   void operator<<(D3DXMATRIX& mtx);
   void operator<<(D3DXVECTOR3& vec);
   void operator<<(Serializable** ptr);

protected:
   void serializeBuf(byte* buf, size_t size);
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Loads a previously saved hierarchy of objects.
 */
class Loader : public Serializer
{
private:
   SerializerImpl*                  m_impl;
   InternalDependenciesSet          m_dependencies;
   ResourcesManager*                m_resMgr;

public:
   Loader( SerializerImpl* impl );
   ~Loader();

   /**
    * Use this method to load a serialized resource.
    *
    * @param resMgr              external resources manager
    * @return                    instance of the load resource
    */
   Resource& load( ResourcesManager& resMgr );

   // -------------------------------------------------------------------------
   // Serializer implementation
   // -------------------------------------------------------------------------
   PointerSaveState getState() const { return PSS_LOAD; }
   void operator<<(std::string& str);
   void operator<<(D3DXMATRIX& mtx);
   void operator<<(D3DXVECTOR3& vec);
   void operator<<(Serializable** ptr);

protected:
   void serializeBuf(byte* buf, size_t size);
};

///////////////////////////////////////////////////////////////////////////////

#include "core\Serializer.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _SERIALIZER_H
