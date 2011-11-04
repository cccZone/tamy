#ifndef _OBJECT_H
#define _OBJECT_H

/// @file   core\Object.h
/// @brief  generic hierarchical object with RTTI and properties

#include <vector>
#include "core\Properties.h"
#include "core\Serializable.h"
#include "core\Class.h"
#include "core\Casts.h"


///////////////////////////////////////////////////////////////////////////////

class PropertiesView;
class Resource;

template< typename T >
class TResourceHandle;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is an abstract object with RTTI and properties, that can be serialized
 * and which can be operated on using the reflection mechanisms.
 *
 * Properties:
 * ------------
 * Each object can have multiple editable properties defined.
 * Those properties are exposed to the editor so that one can
 * edit them. 
 * Good candidate for a property is a member that's unique and native
 * to an object - a number, a string, a texture, a color - something
 * that can differ from one object instance to another.
 * A bad one would be something shared between object - such as
 * a camera that is used to render the entire scene, or a renderer
 * instance.
 *
 * Management by a resource:
 * -------------------------
 * An object can be embedded in a resource. In order to allow other objects to reference
 * it after the resource's been deserialized, we're exposing a system of resource
 * handles.
 * An object can be added to and managed by a resource - this resource becomes 
 * its host resource and from now on a handle to this object can be queried.
 * An object can have only a single host resource - adding it to another one
 * will cause an exception to be thrown.
 */
class Object : public Serializable, public RTTIObject, public PropertyObserver
{
   DECLARE_RTTI_CLASS

private:
   std::vector< Properties* >    m_properties;

public:
   /**
    * Constructor.
    */
   Object();
   virtual ~Object();

   /**
    * The method allows to attach a view allowing to access
    * the properties of the entire object's hierarchy
    */
   void viewProperties(PropertiesView& view);

   // -------------------------------------------------------------------------
   // Serializable implementation
   // -------------------------------------------------------------------------
   void onSave(Serializer& serializer);
   void onLoad(Serializer& serializer);
   int getClassID() const;

protected:
   /**
    * Adds a new set of properties - to be called when a new derived class is created.
    */
   void addProperties(Properties* properties);

   /**
    * Called when initializing the object's properties
    */
   virtual void implementObject() {}

   // -------------------------------------------------------------------------
   // PropertyObserver implementation
   // -------------------------------------------------------------------------
   void onPropertyChanged(Property& property);
};

///////////////////////////////////////////////////////////////////////////////

#define DECLARE_CLASS(ClassName)                                              \
   DECLARE_RTTI_CLASS                                                         \
private:                                                                      \
   Properties* m_properties;                                                  \
protected:                                                                    \
   void implementObject();                                                    \

///////////////////////////////////////////////////////////////////////////////

#define DECLARE_STRUCT(ClassName)                                             \
   DECLARE_RTTI_STRUCT                                                        \
private:                                                                      \
   Properties* m_properties;                                                  \
protected:                                                                    \
   void implementObject();                                                    \
public:

///////////////////////////////////////////////////////////////////////////////

#define IMPLEMENT_OBJECT(ClassName)                                           \
   void ClassName::implementObject()                                          \
   {                                                                          \
      __super::implementObject();                                             \
      m_properties = new Properties(#ClassName);                              \
      addProperties(m_properties);

///////////////////////////////////////////////////////////////////////////////

#define BEGIN_OBJECT(ClassName, ParentClassName)                              \
   BEGIN_RTTI(ClassName)                                                      \
      PARENT(ParentClassName)                                                 \
   END_RTTI                                                                   \
   IMPLEMENT_OBJECT(ClassName)

///////////////////////////////////////////////////////////////////////////////

#define BEGIN_ABSTRACT_OBJECT(ClassName, ParentClassName)                     \
   BEGIN_ABSTRACT_RTTI(ClassName)                                             \
   PARENT(ParentClassName)                                                    \
   END_RTTI                                                                   \
   IMPLEMENT_OBJECT(ClassName)

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro defines a non-editable property.
 */
#define PROPERTY(type, variable)                                              \
   {                                                                          \
      Property& p = m_properties->add< type >( variable, #variable );         \
      p.setParams( #variable, "", false, true );                              \
      p.setObserver(*this);                                                   \
   }

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro defines an editable property.
 */
#define PROPERTY_EDIT(label, type, variable)                                  \
   {                                                                          \
      Property& p = m_properties->add< type >( variable, #variable );         \
      p.setParams( #variable, label, true, true );                            \
      p.setObserver(*this);                                                   \
   }

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro defines an editable property that will not be saved ( is valid
 * only for one runtime ).
 */
#define PROPERTY_EDIT_RUNTIME(label, type, variable)                          \
   {                                                                          \
      Property& p = m_properties->add< type >( variable, #variable );         \
      p.setParams( #variable, label, true, false );                           \
      p.setObserver(*this);                                                   \
   }

///////////////////////////////////////////////////////////////////////////////

#define END_OBJECT() }

///////////////////////////////////////////////////////////////////////////////

#endif // _OBJECT_H
