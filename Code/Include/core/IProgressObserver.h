/// @file   core/IProgressObserver.h
/// @brief  common interface for all progress observers

#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * Common interface for all progress observers.
 */
class IProgressObserver
{
public:
   virtual ~IProgressObserver() {}

   /**
    * Call this method to initialize a progress counting sequence.
    *
    * @param tag           observed section tag
    * @param elemsCount    number of elements in the observed section
    */
   virtual void initialize( const std::string& tag, unsigned int elemsCount ) = 0;

   /**
    * Call this method to advance the progress counting sequence.
    */ 
   virtual void advance() = 0;
};

///////////////////////////////////////////////////////////////////////////////

class NullProgressObserver : public IProgressObserver
{
public:
   void initialize( const std::string& tag, unsigned int elemsCount ) {}
   void advance() {}
};

///////////////////////////////////////////////////////////////////////////////
