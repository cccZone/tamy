/// @file   core/OutArrayStream.h
/// @brief  an output stream that loads data from a memory buffer
#pragma once

#include "core/OutStream.h"
#include "core/Array.h"
#include "core/types.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * An output stream that loads data from a memory buffer.
 */
class OutArrayStream : public OutStream
{
private:
   Array< byte >&          m_buffer;

public:
   /**
    * Constructor.
    *
    * @param buffer
    */
   OutArrayStream( Array< byte >& buffer );

protected:
   // ----------------------------------------------------------------------
   // OutStream implementation
   // ----------------------------------------------------------------------
   void save( const void* val, unsigned int dataSize );
};

///////////////////////////////////////////////////////////////////////////////
