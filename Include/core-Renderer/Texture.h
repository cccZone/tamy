#pragma once

#include <string>
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

class TextureImpl
{
public:
   virtual ~TextureImpl() {}
};

///////////////////////////////////////////////////////////////////////////////

template<typename T>
class TTextureImplDestructor
{
public:
   void destroy(T* impl);
};

///////////////////////////////////////////////////////////////////////////////

template<typename T>
class TTextureImpl : public TextureImpl
{
private:
   T* m_impl;
   TTextureImplDestructor<T> m_destructor;

public:
   TTextureImpl() : m_impl(NULL) {}
   ~TTextureImpl() {m_destructor.destroy(m_impl); m_impl = NULL;}

   T& get() {return *m_impl;}

   void set(T* impl) {m_impl = impl;}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a texture of a graphical object
 */
class Texture
{
private:
   std::string m_name;

public:
   Texture(const std::string& name) : m_name(name) {}
   virtual ~Texture() {}

   const std::string& getName() const {return m_name;}

   bool operator==(const Texture& rhs) const {return m_name == rhs.m_name;}
   bool operator!=(const Texture& rhs) const {return !(*this == rhs);}

   /**
    * This method returns the type specific implementation
    * of the texture
    */
   virtual TextureImpl& getImpl() = 0;
};

///////////////////////////////////////////////////////////////////////////////
