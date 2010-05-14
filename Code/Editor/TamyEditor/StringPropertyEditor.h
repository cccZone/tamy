#pragma once

/// @file   TamyEditor\StringPropertyEditor.h
/// @brief  string property editor

#include "core\Property.h"
#include "QPropertyEditor.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit string properties.
 */
class StringPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   std::string& m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   StringPropertyEditor( std::string& property, const std::string& label );

public slots:
   void valChanged(const QString& val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
