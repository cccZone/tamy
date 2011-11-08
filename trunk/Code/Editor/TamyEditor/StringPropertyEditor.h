#pragma once

/// @file   TamyEditor\StringPropertyEditor.h
/// @brief  string property editor

#include "core\Property.h"
#include "QPropertyEditor.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit string properties.
 */
class StringPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< std::string >* m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   StringPropertyEditor( TEditableProperty< std::string >* property );
   ~StringPropertyEditor();

public slots:
   void valChanged(const QString& val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
