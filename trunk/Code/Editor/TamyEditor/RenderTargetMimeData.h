/// @file   TamyEditor\RenderTargetMimeData.h
/// @brief  render target related drag&drop data

#pragma once

#include "MimeDataEncoder.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * Render target related drag&drop data.
 */
class RenderTargetMimeData : public MimeDataEncoder
{
private:
   std::string&               m_id;

public:
   /**
    * Constructor.
    *
    * @param items      items info about which we want to drag
    */
   RenderTargetMimeData( std::string& id );

   // -------------------------------------------------------------------------
   // MimeDataEncoder implementation
   // -------------------------------------------------------------------------
   void save( QMimeData& data );
   void load( const QMimeData& data );
};

///////////////////////////////////////////////////////////////////////////////
