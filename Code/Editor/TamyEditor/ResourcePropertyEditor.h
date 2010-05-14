#pragma once

/// @file   TamyEditor\ResourcePropertyEditor.h
/// @brief  resource property editor

#include "core\Property.h"
#include "QPropertyEditor.h"
#include "core\Resource.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;
class DropArea;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit resource properties.
 */
class ResourcePropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   Resource*&                    m_property;
   Class                         m_acceptableType;

   DropArea*                     m_resourceName;
   std::vector< std::string >    m_paths;

public:
   /**
    * Constructor.
    *
    * @param property         property this editor edits
    * @param acceptableType   type this editor can accept
    * @param label            property label
    */
   ResourcePropertyEditor( Resource*& property, const Class& acceptableType, const std::string& label );

public slots:
   void valChanged();
   void valErased( bool );

protected:
   void onInitialize();

private:
   void refreshPropertyName();
};

///////////////////////////////////////////////////////////////////////////////
