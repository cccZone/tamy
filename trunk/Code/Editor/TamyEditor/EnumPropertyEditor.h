#pragma once

/// @file   TamyEditor\EnumPropertyEditor.h
/// @brief  enum property editor

#include "core\Property.h"
#include "core\Enum.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;
class Enum;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit enum properties.
 */
class EnumPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< Enum >*    m_property;
   Enum*                         m_enumeration;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   EnumPropertyEditor( TEditableProperty< Enum >* property );
   ~EnumPropertyEditor();

public slots:
   void valChanged( const QString& val );

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
