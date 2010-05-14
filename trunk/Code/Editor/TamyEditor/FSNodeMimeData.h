#pragma once

/// @file   TamyEditor\FSNodeMimeData.h
/// @brief  This class manages the data used during a during a drag&drop operation
///         to describe nodes in a file system

#include "MimeDataEncoder.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class FSNodeMimeData : public MimeDataEncoder
{
private:
   std::vector< std::string >&   m_paths;

public:
   /**
    * Constructor.
    *
    * @param data       data the encoder operates on
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
