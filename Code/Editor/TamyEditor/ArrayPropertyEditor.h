/// @file   TamyEditor\ArrayPropertyEditor.h
/// @brief  array container property editor ( featuring an std::vector as the array ;)
#pragma once

#include "core\ReflectionProperty.h"
#include "QPropertyEditor.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * Array container property editor (featuring an std::vector as the array ;)
 */
class ArrayPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableReflectionProperty< VectorReflectionProperty >*   m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   ArrayPropertyEditor( TEditableReflectionProperty< VectorReflectionProperty >* property );
   ~ArrayPropertyEditor();

public slots:

protected:
   void onInitialize();
};

///////////////////////////////////////////////////////////////////////////////
