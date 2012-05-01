/// @file   core\ReflectionPropertiesView.h
/// @brief  a generic factory of property editors

#ifndef REFLECTION_PROPERTIES_VIEW_H
#define REFLECTION_PROPERTIES_VIEW_H

#include <vector>
#include "core\GenericFactory.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

class ReflectionProperty;
class ReflectionPropertyEditor;
class ReflectionProperties;

///////////////////////////////////////////////////////////////////////////////

/**
 * Interface for all properties views.
 */
class ReflectionPropertiesView
{
public:
   virtual ~ReflectionPropertiesView() {}

   /**
    * This method allows to set a new properties set with this view,
    * allowing to create new editors for it.
    *
    * @param properties    properties set we want to view
    */
   virtual void set( ReflectionProperties* properties ) = 0;

   /**
    * This method removes all editors from the view.
    */
   virtual void reset() = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This view will create editors for the properties allowing to change.
 * It's called a view, because it allows to 'visualize' the properties
 * using the editors, expose them to the user that way.
 *
 * It's generic - this level only allows to create editors.
 * However it's the extensions that will actually be passed to the created
 * instances of the editors, allowing those editors to interact with the system.
 * Such a derived view could be for instance a window - and editors
 * could be edit fields showing the values.
 *
 * Each editor should be derived from TPropertyEditor class and specialized
 * for a particular child view class. This way the child view class can contain
 * all the applicable implementation data (window details etc), and expose
 * them to the editors that are meant to operate with that view.
 */
template <typename Derived>
class TReflectionPropertiesView : public ReflectionPropertiesView
{
private:
   // -------------------------------------------------------------------------
   // internal types definitions
   // -------------------------------------------------------------------------

   class Creator
   {
   public:
      virtual ~Creator() {}

      virtual ReflectionPropertyEditor* operator()( ReflectionProperty* obj ) = 0;

      virtual bool doesMatch( const ReflectionType& checkedClassType ) const = 0;
   };

   // -------------------------------------------------------------------------

   template< class REPR_IMPL, class ENTITY_IMPL >
   class TStaticCreator : public Creator
   {
   private:
      const ReflectionType&    m_refClass;

   public:
      TStaticCreator()
         : m_refClass( TReflectionProperty< ENTITY_IMPL >::getRTTIClass() )
      {
      }

      ReflectionPropertyEditor* operator()( ReflectionProperty* property )
      {
         TEditableReflectionProperty< ENTITY_IMPL >* editableProperty = new TEditableReflectionProperty< ENTITY_IMPL >( *property );
         return new REPR_IMPL( editableProperty );
      }

      bool doesMatch( const ReflectionType& checkedClassType ) const
      {
         return m_refClass.isExactlyA( checkedClassType );
      }
   };

   // -------------------------------------------------------------------------

   template< class REPR_IMPL, class ENTITY_IMPL >
   class TPolymorphicCreator : public Creator
   {
   private:
      const ReflectionType&    m_refClass;

   public:
      TPolymorphicCreator()
         : m_refClass( TReflectionProperty< ENTITY_IMPL >::getRTTIClass() )
      {
      }

      ReflectionPropertyEditor* operator()( ReflectionProperty* property )
      {
         TEditableReflectionProperty< ENTITY_IMPL >* editableProperty = new TEditableReflectionProperty< ENTITY_IMPL >( *property );
         return new REPR_IMPL( editableProperty );
      }

      bool doesMatch( const ReflectionType& checkedClassType ) const
      {
         return checkedClassType.isA( m_refClass );
      }
   };

   // -------------------------------------------------------------------------

   template< class REPR_IMPL >
   class TEnumCreator : public Creator
   {
   private:
      const ReflectionType&      m_genericEnumType;

   public:
      TEnumCreator()
         : m_genericEnumType( ReflectionTypesRegistry::getInstance().getGenericEnumType() )
      {
      }

      ReflectionPropertyEditor* operator()( ReflectionProperty* property )
      {
         TEditableReflectionProperty< ReflectionEnum >* editableProperty = new TEditableReflectionProperty< ReflectionEnum >( *property );
         return new REPR_IMPL( editableProperty );
      }

      bool doesMatch( const ReflectionType& checkedClassType ) const
      {
         return checkedClassType.isA( m_genericEnumType );
      }
   };

   // -------------------------------------------------------------------------

   typedef std::vector< Creator* > CreatorsVec;

private:
   ReflectionProperties*                        m_properties;
   std::vector< ReflectionPropertyEditor* >     m_editors;

   CreatorsVec                                  m_creators;

public:
   /**
    * Constructor.
    */
   TReflectionPropertiesView();
   virtual ~TReflectionPropertiesView();

   /**
    * This method tells the number of currently active editors.
    *
    * @return  number of editors working in the view.
    */
   inline uint getEditorsCount() const;

   /**
    * Returns the specified editor
    */
   inline ReflectionPropertyEditor& getEditor( uint idx ) const;

   // -------------------------------------------------------------------------
   // ReflectionPropertiesView implementation
   // -------------------------------------------------------------------------
   void set( ReflectionProperties* properties );

   void reset();

   // -------------------------------------------------------------------------
   // Factory related functionality
   // -------------------------------------------------------------------------
   /**
    * This method associates an entity class with another class representing it.
    * From now on whenever a create method on such 'entity' class is 
    * called, this very representation will be created for it.
    *
    * The call to this method can be chained, allowing to set up
    * the whole factory in a single code block.
    *
    * @param PROPERTY_IMPL             solid type property
    * @param EDITOR_IMPL               property editor class
    */
   template< class PROPERTY_IMPL, class EDITOR_IMPL >
   void associate()
   {
      m_creators.push_back( new TStaticCreator< EDITOR_IMPL, PROPERTY_IMPL >() );
   }

   /**
    * This method associates an abstract property with an editor.
    *
    * @param ABSTRACT_PROPERTY_IMPL    abstract type property
    * @param EDITOR_IMPL               property editor class
    */
   template< class ABSTRACT_PROPERTY_IMPL, class EDITOR_IMPL >
   void associateAbstract()
   {
      m_creators.push_back( new TPolymorphicCreator< EDITOR_IMPL, ABSTRACT_PROPERTY_IMPL >() );
   }

   /**
    * This method associates the enum properties with an editor.
    *
    * @param EDITOR_IMPL               property editor class
    */
   template< class EDITOR_IMPL >
   void associateEnums()
   {
      m_creators.push_back( new TEnumCreator< EDITOR_IMPL >() );
   }

   /**
    * The method creates a representation for an entity class.
    * Such an entity class needs to be registered with the factory
    * prior to this call.
    *
    * @param property   property for which we want to create a representation
    * @return           representation
    */ 
   ReflectionPropertyEditor* create( ReflectionProperty& property );

protected:
   // -------------------------------------------------------------------------
   // Notifications
   // -------------------------------------------------------------------------

   /**
    * Called when new properties are being set in the editor.
    *
    * @param ownerNpropertiesame  properties that are being set
    */
   virtual void onSet( ReflectionProperties& properties ) {}
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ReflectionPropertiesView.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // REFLECTION_PROPERTIES_VIEW_H
