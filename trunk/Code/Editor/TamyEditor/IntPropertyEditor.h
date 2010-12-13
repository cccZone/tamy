/// @file   TamyEditor\IntPropertyEditor.h
/// @brief  integer value property editor
#pragma once

#include "core\Property.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit integer value properties.
 */
class IntPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< int >*   m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   IntPropertyEditor( TEditableProperty< int >* property );
   ~IntPropertyEditor();

public slots:
   void valChanged(int val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit unsigned integer value properties.
 */
class UIntPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< unsigned int >*   m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   UIntPropertyEditor( TEditableProperty< unsigned int >* property );
   ~UIntPropertyEditor();

public slots:
   void valChanged(int val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit unsigned integer value properties.
 */
class LongPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< long >*   m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   LongPropertyEditor( TEditableProperty< long >* property );
   ~LongPropertyEditor();

public slots:
   void valChanged(int val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit unsigned long value properties.
 */
class ULongPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< unsigned long >*   m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   ULongPropertyEditor( TEditableProperty< unsigned long >* property );
   ~ULongPropertyEditor();

public slots:
   void valChanged(int val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
