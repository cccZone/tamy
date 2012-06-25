#pragma once

/// @file      core\StringUtils.h
/// @brief     various tools operating on strings

#include <string>
#include <vector>


namespace StringUtils
{

///////////////////////////////////////////////////////////////////////////////

/**
 * Tokenizes a string by splitting it across the boundaries of the specified
 * characters (tokens).
 *
 * @param str     string to be tokenized
 * @param tokens  characters used as the splitting markers
 * @param output  collection which will be filled with the results of string slicing
 */
void tokenize( const std::string& str, 
               const std::string& tokens,
               std::vector<std::string>& output );

///////////////////////////////////////////////////////////////////////////////

/**
 * Calculates a has value based on the string contents
 *
 * @param str
 * @return     hash value
 */
unsigned int calculateHash( const std::string& str );

///////////////////////////////////////////////////////////////////////////////

/**
 * Converts all characters in the string to upper case.
 *
 * @param inOutStr
 */
void toUpper( std::string& inOutStr );

///////////////////////////////////////////////////////////////////////////////

} // StringUtils
