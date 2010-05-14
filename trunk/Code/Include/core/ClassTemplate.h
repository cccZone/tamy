#pragma once

/// @file   core\ClassTemplate.h
/// @brief  template of a class

#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class ClassCreator
{
public:
   virtual ~ClassCreator() {}

   virtual void* create() const = 0;

   virtual ClassCreator* clone() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Template of a class - many Class instances will refer to a single 
 * ClassTemplate instance.
 *
 * A ClassTemplate instance describes the features of a single class,
 * while a Class instance can be thought of as a handle to that class type.
 */
class ClassTemplate
{
private:
   std::string                m_name;
   unsigned int               m_handle;
   std::vector< std::string > m_parents;
   ClassCreator*              m_creator;

public:
   /**
    * Default constructor.
    * 
    * @param handle
    * @param className
    * @param creator    instance creator
    */
   ClassTemplate( unsigned int handle, const std::string& className, ClassCreator* creator );

   /**
    * Copy constructor.
    */
   ClassTemplate( const ClassTemplate& rhs );
   ~ClassTemplate();

   /**
    * Returns the name of the class type.
    */
   inline const std::string& getName() const { return m_name; }

   /**
    * Returns a unique handle assigned to this class type.
    */
   inline unsigned int getHandle() const { return m_handle; }

   /**
    * Creates a new class instance.
    */
   inline void* instantiate() const { return m_creator->create(); }

   /**
    * This method fills the specified collection with handles
    * of the classes that are the parents of the class registered
    * under the specified handle.
    *
    * @throw std::out_of_range   if the class hasn't been registered
    *
    * @param handle              handle to the class parents of whose 
    *                            we want to check
    * @param outParentHandles    collection that will be filled with
    *                            the parents' handles
    */
   inline const std::vector<std::string>& getParents() const { return m_parents; }

   /**
    * Defines a new parent for the class.
    */
   void addParent( const std::string& classTypeName );

   /**
    * The operator performs an is-exactly-a comparison
    * of the two types.
    */
   bool operator==(const ClassTemplate& rhs) const;

   /**
    * The operator performs an is-exactly-a comparison
    * of the two types and returns true if the types are different.
    */
   bool operator!=(const ClassTemplate& rhs) const;

   /**
    * This operator allows to use the classes as keys in hash-maps and hash-sets.
    * Is-exactly-a comparison of the two types is performed.
    */
   bool operator<( const ClassTemplate& rhs ) const;

   /**
    * Stream operator storing info about the class template.
    */
   friend std::ostream& operator<<( std::ostream& out, const ClassTemplate& rhs );
};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
class SolidCreator : public ClassCreator
{
public:
   void* create() const { return new T(); }

   ClassCreator* clone() const { return new SolidCreator< T >(); }
};

///////////////////////////////////////////////////////////////////////////////
