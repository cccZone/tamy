#pragma once

/// @file   TamyEditor\StringPropertyEditor.h
/// @brief  string property editor

#include "core-Scene\Property.h"
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
   TProperty<std::string>& m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   StringPropertyEditor(TProperty<std::string>& property);

public slots:
   void valChanged(const QString& val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
