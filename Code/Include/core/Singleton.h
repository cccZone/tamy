/// @file   core/Singleton.h
/// @brief  Singleton design pattern

#ifndef _SINGLETON_H
#define _SINGLETON_H


///////////////////////////////////////////////////////////////////////////////

/**
 * Singleton design pattern.
 */
template< typename T >
class Singleton 
{
private:
   static T          s_theInstance;

public:
   virtual ~Singleton() {}

   /**
    * Returns the singleton instance of the class.
    */
   static T&         getInstance()        { return s_theInstance; } 
};

///////////////////////////////////////////////////////////////////////////////

#define INIT_SINGLETON( ClassName ) template<> ClassName Singleton< ClassName >::s_theInstance;

///////////////////////////////////////////////////////////////////////////////

#endif // _SINGLETON_H
