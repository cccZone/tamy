/// @file   TamyEditor\ArrayPropertyEditor.h
/// @brief  array container property editor ( featuring an std::vector as the array ;)
#pragma once

#include "core\Property.h"
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
   TEditableProperty< VectorProperty >*   m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   ArrayPropertyEditor( TEditableProperty< VectorProperty >* property );
   ~ArrayPropertyEditor();

public slots:

protected:
   void onInitialize();
};

///////////////////////////////////////////////////////////////////////////////
