#ifndef _PROPERTIES_VIEW_H
#define _PROPERTIES_VIEW_H

/// @file   core-Scene\PropertiesView.h
/// @brief  a generic factory of property editors

#include <vector>
#include "core\GenericFactory.h"


///////////////////////////////////////////////////////////////////////////////

class Property;
class PropertyEditor;
class Properties;

///////////////////////////////////////////////////////////////////////////////

/**
 * Interface for all properties views.
 */
class PropertiesView
{
public:
   virtual ~PropertiesView() {}

   /**
    * This method allows to set a new properties set with this view,
    * allowing to create new editors for it.
    *
    * @param properties    properties set we want to view
    */
   virtual void set(Properties& properties) = 0;

   /**
    * This method removes all editors from the view.
    */
   virtual void reset() = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This view will create editors for the properties allowing to change.
 * It's called a view, because it allows to 'visualise' the properties
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
template <typename Derrived>
class TPropertiesView : public PropertiesView,
                        public GenericFactory<Property, PropertyEditor>
{
private:
   std::vector<PropertyEditor*> m_editors;

public:
   virtual ~TPropertiesView();

   /**
    * This method tells the number of currently active editors.
    *
    * @return  number of editors working in the view.
    */
   unsigned int getEditorsCount() const;

   // -------------------------------------------------------------------------
   // PropertiesView implementation
   // -------------------------------------------------------------------------
   void set(Properties& properties);

   void reset();
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Scene\PropertiesView.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _PROPERTIES_VIEW_H
