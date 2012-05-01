/// @file   TamyEditor\ResourcePropertyEditor.h
/// @brief  resource property editor
#pragma once


#include "core\ReflectionProperty.h"
#include "QPropertyEditor.h"
#include "core\Resource.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class DropArea;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit resource properties.
 */
class ResourcePropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableReflectionProperty< Resource* >*  m_property;

   DropArea*                        m_resourceName;
   std::vector< std::string >       m_paths;

public:
   /**
    * Constructor.
    *
    * @param property         property this editor edits
    */
   ResourcePropertyEditor( TEditableReflectionProperty< Resource* >* property );
   ~ResourcePropertyEditor();

public slots:
   void valChanged();
   void valErased( bool );

protected:
   void onInitialize();

private:
   void refreshPropertyName();
};

///////////////////////////////////////////////////////////////////////////////
