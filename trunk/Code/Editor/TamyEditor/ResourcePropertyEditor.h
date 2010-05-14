#pragma once

/// @file   TamyEditor\ResourcePropertyEditor.h
/// @brief  resource property editor

#include "core\Property.h"
#include "QPropertyEditor.h"
#include "core\Resource.h"


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit resource properties.
 */
class ResourcePropertyEditor : public QPropertyEditor
{
private:
   Resource*& m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   ResourcePropertyEditor( Resource*& property, const std::string& label );

public slots:

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
