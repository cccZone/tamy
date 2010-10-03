/// @file   core/Assert.h
/// @brief  definition of code asserts

#pragma once


///////////////////////////////////////////////////////////////////////////////

class AssertionHandle
{
private:
   static int     s_handle;

   int            m_handle;
   const char*    m_function;
   const char*    m_file;
   unsigned int   m_line;

   bool           m_ignored;

public:
   AssertionHandle( const char* function, const char* file, unsigned int line );

   inline int getHandle() const { return m_handle; }

   inline const char* getFunction() const { return m_function; }

   inline const char* getFile() const { return m_file; }

   inline unsigned int getLine() const { return m_line; }

   inline void ignore() { m_ignored = true; }

   inline bool isIgnored() const { return m_ignored; }
};

///////////////////////////////////////////////////////////////////////////////

typedef void (*TamyAssertFunc)( const char* msg, AssertionHandle& assertHandle );

/**
 * Registers a new assert callback function.
 */
void registerAssertCallback( TamyAssertFunc func );

/**
 * Makes the system ignore the assertions with the specified handle
 */
void ignoreAssertion( int assertionHandle );

/**
 * Invokes an engine assertion.
 */
extern TamyAssertFunc _tamyAssert;

///////////////////////////////////////////////////////////////////////////////

/**
 * An assert without a message
 */
#define ASSERT( expression )  { static AssertionHandle handler( __FUNCTION__, __FILE__, __LINE__ ); (void)( ( !!( expression ) ) || ( !!( handler.isIgnored() ) ) || ( _tamyAssert( #expression, handler ), 0 ) ); }

/**
 * An assert with some message.
 */
#define ASSERT_MSG( expression, msg ) { static AssertionHandle handler( __FUNCTION__, __FILE__, __LINE__ ); (void)( ( !!( expression ) ) || ( !!( handler.isIgnored() ) ) || ( _tamyAssert( msg, handler ), 0 ) ); }

///////////////////////////////////////////////////////////////////////////////
