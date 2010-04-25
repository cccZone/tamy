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
      PSS_UPDATE
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

typedef std::vector<Serializable*>  DependenciesSet;

///////////////////////////////////////////////////////////////////////////////

/**
 * An internal class used in the process of serialization to find 
 * all the dependencies between the objects (pointers) and save/restore
 * their status.
 */
class DependenciesMapper : public Serializer
{
private:
   DependenciesSet&     m_dependencies;
   PointerSaveState     m_state;
  
public:
   DependenciesMapper( DependenciesSet& dependencies, PointerSaveState state );

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
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Saves a hierarchy of objects using the specified serialization implementation.
 */
class Saver : public Serializer
{
private:
   SerializerImpl*      m_impl;
   DependenciesSet      m_dependencies;

public:
   Saver( SerializerImpl* impl );
   ~Saver();

   /**
    * Use this method to save an object capable of being serialized.
    *
    * @param serializable  object to save
    */
   void save( Serializable& serializable );

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
   DependenciesSet                  m_dependencies;

public:
   Loader( SerializerImpl* impl );
   ~Loader();

   /**
    * Use this method to load a serialized object.
    *
    * @param Derived    expected type of the object
    * @return           instance of the load object
    */
   template< typename Derived >
   Derived* load();

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
