#pragma once

/// @file   TamyEditor\EntityPropertyEditor.h
/// @brief  editor for properties with pointers to entities

#include "core\Property.h"
#include "core-MVC\Entity.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;
class Entity;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit a pointer to an entity.
 */
class EntityPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   Entity*& m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   EntityPropertyEditor( Entity*& property, const std::string& label );

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
