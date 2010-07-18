/// @file   core/Assert.h
/// @brief  definition of code asserts

#pragma once


///////////////////////////////////////////////////////////////////////////////

/**
 * An assert without a message
 */
void ASSERT( bool condition );

/**
 * An assert with some message.
 */
void ASSERT( bool condition, const char* format, ... );


///////////////////////////////////////////////////////////////////////////////
