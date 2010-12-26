/// @file   TamyEditor\FSNodeMimeData.h
/// @brief  Filesystem node drag&drop related data

#pragma once

#include "MimeDataEncoder.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class manages the data used during a during a drag&drop operation
 * to describe nodes in a file system.
 */
class FSNodeMimeData : public MimeDataEncoder
{
private:
   std::vector< std::string >&   m_paths;

public:
   /**
    * Constructor.
    *
    * @param paths      paths we want to persist
    */
   FSNodeMimeData( std::vector< std::string >& paths );

   // -------------------------------------------------------------------------
   // MimeDataEncoder implementation
   // -------------------------------------------------------------------------
   void save( QMimeData& data );
   void load( const QMimeData& data );
};

///////////////////////////////////////////////////////////////////////////////
