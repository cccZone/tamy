#include "core\StringUtils.h"


namespace StringUtils
{

///////////////////////////////////////////////////////////////////////////////

void tokenize( const std::string& str, 
               const std::string& tokens, 
               std::vector<std::string>& output)
{
   std::size_t start, end;
   std::string tmp;
   start = 0;
   end = 0;

   while (end != std::string::npos)
   {
      end = str.find_first_of(tokens, start);
      tmp = str.substr(start, end - start);
      if (tmp.size() > 0)
      {
         output.push_back(tmp);
      }
      start = end + 1;
   }
}

///////////////////////////////////////////////////////////////////////////////

unsigned int calculateHash( const std::string& str )
{
   unsigned int hash = INT_MAX;

   unsigned int count = str.length() / sizeof( unsigned int );
   if ( str.length() % sizeof( unsigned int ) > 0 )
   {
      count++;
   }
   unsigned int tmpStrSize = count * sizeof( unsigned int );
   static char tempStr[ 1024 ];
   memset( tempStr, 0, tmpStrSize );
   memcpy( tempStr, str.c_str(), str.length() );
   const unsigned int* valsArr = ( const unsigned int* )( tempStr );
   for ( unsigned int i = 0; i < count; ++i )
   {
      hash ^= valsArr[ i ];
   }

   return hash;
}

///////////////////////////////////////////////////////////////////////////////

} // StringUtils
