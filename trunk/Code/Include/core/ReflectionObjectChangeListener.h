/// @file   core/ReflectionObjectChangeListener.h
/// @brief  listener interface that will get notified about the changes the observed object undergoes
#pragma once


///////////////////////////////////////////////////////////////////////////////

class ReflectionProperty;

///////////////////////////////////////////////////////////////////////////////

/**
 * Listener interface that will get notified about the changes the observed object undergoes.
 */
class ReflectionObjectChangeListener
{
public:
   virtual ~ReflectionObjectChangeListener() {}

   /**
    * This method will be called if the edited property gets changed externally and the editor
    * should update the value.
    *
    * @param property
    */
   virtual void onPropertyChanged( ReflectionProperty& property ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
