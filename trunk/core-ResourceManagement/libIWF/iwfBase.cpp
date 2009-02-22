///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//          _ _ _    _____        _______   ____       _                     //
//         | (_) |__|_ _\ \      / /  ___| | __ )  ___| |_ __ _              //
//         | | | '_ \| | \ \ /\ / /| |_    |  _ \ / _ \ __/ _` |             //
//         | | | |_) | |  \ V  V / |  _|   | |_) |  __/ || (_| |             //
//         |_|_|_.__/___|  \_/\_/  |_|     |____/ \___|\__\__,_|             //
//                                                          v0.0.1           //
//                                                                           //
//---------------------------------------------------------------------------//
//                                                                           //
//                          File: iwfBase.cpp                                //
//                                                                           //
//                           Date: 26/08/2002                                //
//                                                                           //
//          Desc: This file houses the base IWF structure support.           //
//                                                                           //
//   Copyright (c) 1997-2002 Daedalus Developments. All rights reserved.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// CBaseIWF Specific Includes
//-----------------------------------------------------------------------------
#include "libIWF.h"

//-----------------------------------------------------------------------------
// Name : CBaseIWF () (Constructor)
// Desc : CBaseIWF Class Constructor
//-----------------------------------------------------------------------------
CBaseIWF::CBaseIWF()
{
    // Reset / Clear all required values
    m_CurrentMode    = MODE_CLOSED;
    m_pOpenChunks    = NULL;
    m_pFile          = NULL;
    m_pChunkProc     = NULL;
    m_AuthorLength   = 0;
    m_StreamStartPos = 0;
    m_LastEOCPos     = 0;
}

//-----------------------------------------------------------------------------
// Name : CBaseIWF () (Alternate Constructor)
// Desc : CBaseIWF Class Constructor. Attaches this IWF to a stream.
//-----------------------------------------------------------------------------
CBaseIWF::CBaseIWF( LPCSTR FileName, IWFPROCESSMODE Mode, IWFFILEHEADER * pHeader )
{
    // Reset / Clear all required values
    m_CurrentMode    = MODE_CLOSED;
    m_pOpenChunks    = NULL;
    m_pFile          = NULL;
    m_pChunkProc     = NULL;
    m_AuthorLength   = 0;
    m_StreamStartPos = 0;
    m_LastEOCPos     = 0;

    // Attach here
    Open( FileName, Mode, pHeader );
}

//-----------------------------------------------------------------------------
// Name : ~CBaseIWF () (Destructor)
// Desc : CBaseIWF Class Destructor
//-----------------------------------------------------------------------------
CBaseIWF::~CBaseIWF()
{
    ClearChunkProcs();
    Close();
}

//-----------------------------------------------------------------------------
// Name : Close ()
// Desc : Closes the currently open file (if any)
//-----------------------------------------------------------------------------
void CBaseIWF::Close()
{
    CHUNKDETAILS * Details;

    // Close the file
    if (m_pFile) fclose( m_pFile );

    // Free up any open chunks
    while (Details = GetOpenChunkHead()) 
    {
        RemoveOpenChunk(Details);
        delete Details;
    } // Next Chunk

    // Reset any variables
    SetAuthorID( NULL, 0 );
    m_CurrentMode    = MODE_CLOSED;
    m_pOpenChunks    = NULL;
    m_AuthorLength   = 0;
    m_StreamStartPos = 0;
    m_LastEOCPos     = 0;
}

//-----------------------------------------------------------------------------
// Name : SetAuthorID ()
// Desc : The author ID system is a mechanism by which individual chunk authors
//        can filter out ANY other chunks within the file not specifically
//        created by them originally. This also allows multiple custom chunks
//        to be added, which have the same type ID, by different authors and
//        prevent any kind of interferance between the two whilst reading.
// Note : Call this function just prior to reading whilst using the ProcessIWF
//        function, to only have 'Standard' and you're custom chunks passed for
//        process. Similarly call this function just prior to writing to have
//        this author data written out along with your custom chunks.
//-----------------------------------------------------------------------------
void CBaseIWF::SetAuthorID( LPVOID pAuthor, unsigned char Length )
{
    // Clear if null passed
    if (pAuthor == NULL) { m_AuthorLength = 0; return; }

    // Store Vars
    m_AuthorLength = Length;

    // Copy author ID over
    memcpy( m_pAuthor, pAuthor, Length );
}

//-----------------------------------------------------------------------------
// Name : CheckValidAuthor () (Private)
// Desc : Used to determine if the currently set author ID is the same as the
//        author ID specified, or the standard IWF author ID.
//-----------------------------------------------------------------------------
bool CBaseIWF::CheckValidAuthor( LPVOID pAuthor, unsigned char Length ) const
{
    // Bail if no author set.. ALL chunks are valid
    if (!m_pAuthor || !Length) return true;
    // If lengths are different, can't possibly be the same ID
    if (Length == m_AuthorLength) {
        // Do a memory compare of the two
        if (memcmp(pAuthor, m_pAuthor, Length) == 0) return true;
    } // End If
    // Skip this chunk
    return false;
}

//-----------------------------------------------------------------------------
// Name : Open () (Virtual)
// Desc : This is the function we call to open the file, and prepare to start
//        either reading or writing the IWF file.
// Note : We must specify a processing mode in this function (either MODE_READ
//        or MODE_WRITE) to allow the IWF functionality to correctly interpret
//        the correct use of some of the multi-purpose read+write functions.
//-----------------------------------------------------------------------------
void CBaseIWF::Open( LPCSTR FileName, IWFPROCESSMODE Mode, IWFFILEHEADER * pHeader )
{
    ULONG Version    = MAKEVERSION(2,0,1), Flags = 0;
    ULONG CheckSum   = 0;
    ULONG ulDummy    = 0;

    // Validate requirements
    if (Mode != MODE_READ && Mode != MODE_READUNFILTERED && Mode != MODE_WRITE) throw IWF_ERR_INVALIDPROCESSMODE;

    // Close any currently open file
    Close();
   
    // Reset any stream info here.
    m_StreamStartPos = 0;

    // Mode Specific Options
    if (Mode == MODE_READ || Mode == MODE_READUNFILTERED) 
    {
        // Check if this is a valid IWF if in read mode
        if (!IsValid(FileName)) throw IWF_ERR_INVALIDFORMAT;

        // Attempt to open the file
        m_pFile = fopen( FileName, "rb" );
        if (!m_pFile) throw IWF_ERR_ACCESSDENIED;

        // Read file header if requested etc.
        if ( pHeader )
        {
            fread( pHeader->Signature  , 1, FILE_SIG_SIZE, m_pFile );
            fread( &pHeader->Version   , 1, sizeof(ULONG), m_pFile );
            fread( &pHeader->Flags     , 1, sizeof(ULONG), m_pFile );
            fread( &pHeader->Checksum  , 1, sizeof(ULONG), m_pFile );
        } // End if
    
    } 
    else if (Mode == MODE_WRITE) 
    {
        // Attempt to open the file
        m_pFile = fopen( FileName, "wb" );
        if (!m_pFile) throw IWF_ERR_ACCESSDENIED;

        // Write out the file header
        Write( (void*)FILE_SIG, FILE_SIG_SIZE);
        Write( &Version, sizeof(ULONG));
        Write( &Flags, sizeof(ULONG));
        Write( &CheckSum, sizeof(ULONG));
        Write( &ulDummy, sizeof(ULONG));
        Write( &ulDummy, sizeof(ULONG));
    
    } // End If

    // Store the current mode
    m_CurrentMode = Mode;
        
}

//-----------------------------------------------------------------------------
// Name : IsValid () (Static)
// Desc : This function will test any stream to determine if it is a valid
//        IWF file. This assumes that the stream is already positioned at the
//        start of the stream expected to be tested, and will also return the
//        stream back to that same position once testing has completed.
//-----------------------------------------------------------------------------
bool CBaseIWF::IsValid( LPCSTR FileName )
{
    bool            SuccessCode = true;
    IWFFILEHEADER   FileHeader;

    // Attempt to open the file
    FILE * pFile = fopen( FileName, "r" );
    if (!pFile) return false;

    // Read file signature etc.
    if ( fread( FileHeader.Signature  , 1, FILE_SIG_SIZE, pFile ) < FILE_SIG_SIZE) SuccessCode = false;
    if ( fread( &FileHeader.Version   , 1, sizeof(ULONG), pFile ) < sizeof(ULONG)) SuccessCode = false;
    if ( fread( &FileHeader.Flags     , 1, sizeof(ULONG), pFile ) < sizeof(ULONG)) SuccessCode = false;
    if ( fread( &FileHeader.Checksum  , 1, sizeof(ULONG), pFile ) < sizeof(ULONG)) SuccessCode = false;

    // If we read them successfully
    if (SuccessCode) 
    {
        // Ensure they are correct
        if (memcmp( FileHeader.Signature, FILE_SIG, FILE_SIG_SIZE ) != 0) SuccessCode = false;
        if (FileHeader.Version < MAKEVERSION( 2, 0, 0 ) ||  FileHeader.Version > MAKEVERSION( 2, 0, 65535 ) )
            SuccessCode = false;

    } // End If

    // Close the file
    fclose( pFile );

    // Success?
    return SuccessCode;
}

//-----------------------------------------------------------------------------
// Name : NextChunk () (Private)
// Desc : This function will move the stream onto the next chunk in the file
//        skipping all that chunks children also. The stream position can be 
//        anywhere within the currently open chunk, not necessarily at the
//        start for the move to occur correctly. 
// Note : This function optionally fills out the CHUNKHEADER structure passed.
//        It will also optionally step INTO the child (if any) of the current
//        chunk if StepInto = true. Returns true if more chunks are available
//        or false if all chunks at this level are exhausted.
//-----------------------------------------------------------------------------
bool CBaseIWF::NextChunk( CHUNKHEADER * pHeader, bool StepInto )
{
    CHUNKHEADER    Header;
    CHUNKFOOTER    Footer;
    CHUNKDETAILS * Details  = NULL;
    __int64        ChunkPos = 0;
    
    // Validate Requirements
    if (m_CurrentMode != MODE_READ && m_CurrentMode != MODE_READUNFILTERED ) throw IWF_ERR_INVALIDPROCESSMODE;

    // Have we found any chunks yet ?
    if (!m_pOpenChunks) {
        // If we have not found a chunk yet seek to first chunk
        Seek( (long)(m_StreamStartPos + (FILE_SIG_SIZE + (sizeof(ULONG) * 5))), SEEK_SET );
    } else {
        // Should we skip over the child chunks?
        if (!StepInto) {
            // Seek to the beginning of the next chunk.
            if (Details = GetOpenChunkHead()) {
                Seek( (long)(Details->Position +  GetHeaderLength(&Details->Header) + Details->Header.ChunkLength), SEEK_SET );
                // Read the chunk footer.
                ReadChunkFooter(&Footer);
                // Remove the previous last open chunk
                RemoveOpenChunk( Details ); 
                delete Details;
                // If we finished reading all the chunks in this level then we're all done
                if (Footer.EOCValue == CHUNK_EOC_EXIT) return false;
            } else {
                throw IWF_ERR_MALFORMED;
            } // End If
        } else {
            // Step to the beginning of the child chunk
            if (Details = GetOpenChunkHead()) {
                Seek( (long)(Details->Position +  GetHeaderLength(&Details->Header) + Details->Header.DataLength), SEEK_SET );
            } // End If
        } // End If StepInto
    } // End If

    // Read in the next chunk
    ChunkPos = GetPosition();

    // If we are attempting to step into a child
    if ( StepInto )
    {
        
            // If there was no header here, there are no children
            if ( ReadChunkHeader( &Header ) == false )
            {
                Seek((long)ChunkPos, SEEK_SET);
                return false;
            
            } // End if no header
    
    } // End if step into child
    else
    {
        // Just read the header (will throw on failure)
        if ( ReadChunkHeader( &Header ) == false )
            throw IWF_ERR_INVALIDSIGNATURE;

    } // End If No Children
    
    // Allocate and new details structure
    if (!(Details = new CHUNKDETAILS)) throw IWF_ERR_OUTOFMEMORY;
    ZeroMemory(Details, sizeof(CHUNKDETAILS));

    // Fill out the required details
    Details->Header   = Header;
    Details->Position = ChunkPos;

    // Store the new open chunk
    AddOpenChunk( Details );

    // Return header info if requested
    if (pHeader) *pHeader = Header;
    
    // More chunks available
    return true;
}

//-----------------------------------------------------------------------------
// Name : RemoveOpenChunk () (Private)
// Desc : Removes the specified chunk from the open chunk list
// Note : Does not delete the specified chunk.
//-----------------------------------------------------------------------------
void CBaseIWF::RemoveOpenChunk( CHUNKDETAILS * pDetails )
{
    // Validate Requirements
    if (!pDetails) return;

    // Update the linked list items
    if (pDetails->Child ) pDetails->Child->Parent = pDetails->Parent;
    if (pDetails->Parent) pDetails->Parent->Child = pDetails->Child;
    if (m_pOpenChunks == pDetails)  m_pOpenChunks = pDetails->Parent;
}

//-----------------------------------------------------------------------------
// Name : AddOpenChunk () (Private)
// Desc : Adds the specified chunk to the HEAD of the linked list.
//-----------------------------------------------------------------------------
void CBaseIWF::AddOpenChunk( CHUNKDETAILS * pDetails )
{
    CHUNKDETAILS * pHead = GetOpenChunkHead();

    // Validate Requirements
    if (!pDetails) return;

    // Clear next / prev pointers just in case
    pDetails->Parent = NULL;
    pDetails->Child  = NULL;

    // If no head was returned, just store and return
    if (!pHead) { m_pOpenChunks = pDetails; return; }

    // Insert as the child of the last opened
    pHead->Child     = pDetails;
    pDetails->Parent = pHead;
    m_pOpenChunks    = pDetails;
}

//-----------------------------------------------------------------------------
// Name : GetOpenChunkHead () (Private)
// Desc : Get the chunk linked list head item if any.
//-----------------------------------------------------------------------------
CHUNKDETAILS * CBaseIWF::GetOpenChunkHead() const
{
    return m_pOpenChunks;
}

//-----------------------------------------------------------------------------
// Name : GetOpenChunkTail () (Private)
// Desc : Get the chunk linked list tail item if any.
//-----------------------------------------------------------------------------
CHUNKDETAILS * CBaseIWF::GetOpenChunkTail() const
{
    // Validate Requirements
    if (!m_pOpenChunks) return NULL;

    // Cycle through until we find the earliest opened chunk
    CHUNKDETAILS * Iterator = m_pOpenChunks;
    while ( Iterator->Parent ) Iterator = Iterator->Parent;
    
    // Return last chunk in list
    return Iterator;
}

//-----------------------------------------------------------------------------
// Name : ProcessIWF ()
// Desc : This function automatically steps through the file informing you of
//        any chunks it encounters by triggering the ProcessChunk function.
// Note : Once you have attached this to a stream, you can call this function
//        to start the whole reading process off.
//-----------------------------------------------------------------------------
void CBaseIWF::ProcessIWF( LPVOID pCustomData )
{
    // Validate Requirements
    if (m_CurrentMode != MODE_READ && m_CurrentMode != MODE_READUNFILTERED) throw IWF_ERR_INVALIDPROCESSMODE;
    if (!m_pFile || !IsOpen()) throw IWF_ERR_FILENOTOPEN;
    
    // Lets start the process off (StepInto will automatically step into the first chunk)
    StepInto( pCustomData );
}

//-----------------------------------------------------------------------------
// Name : StepInto ()
// Desc : Because the IWF structure is by it's very nature heirarchical, this
//        function is provided to allow you to step down into the child chunks
//        should you wish to do so.
// Note : While you can still step through the chunks manually, by calling this
//        function from within the triggered ProcessChunk function, you can
//        take advantage of recursion, and passing through any details you need
//        into that next instance of the function call.
//-----------------------------------------------------------------------------
void CBaseIWF::StepInto( LPVOID pCustomData )
{
    CHUNKCALLBACK  *CallBack        = NULL;
    CHUNKDETAILS   *CurrentChunk    = NULL;
    bool            ChunksExhausted = false;
    
    // Validate Requirements
    if (m_CurrentMode != MODE_READ && m_CurrentMode != MODE_READUNFILTERED) throw IWF_ERR_INVALIDPROCESSMODE;
    if (!IsOpen()) throw IWF_ERR_FILENOTOPEN;
    
    // Read in the first chunk at the lower level
    if (!NextChunk( NULL, true )) return;
    
    // Lets start reading
    while (!ChunksExhausted)
    {
        // Retrieve the current chunk details
        CurrentChunk = GetOpenChunkHead();
        
        // If they specified an author chunk ID then check it
        // If this is an entity chunk, all chunks must be passed through
        if ( m_CurrentMode == MODE_READUNFILTERED || CurrentChunk->Header.ChunkTypeID == CHUNK_ENTITY ||
             CheckValidAuthor( CurrentChunk->Header.AuthorID, CurrentChunk->Header.AuthorIDLength ) ) 
        {
            // Loop through any callbacks registered for this chunk type
            for ( CallBack = m_pChunkProc; CallBack; CallBack = CallBack->Next )
            {
                if ( CurrentChunk->Header.ChunkTypeID == CallBack->ChunkTypeID )
                {
                    // Call into their registered callback
                    CHUNKPROC ChunkProc = CallBack->pCallback;
                    ChunkProc( CallBack->pContext, CurrentChunk->Header, pCustomData );
                    break;
                } // End if
            } // Next CallBack

            // If no callback was registered or found, call default processor
            if ( !CallBack ) ProcessChunk( CurrentChunk->Header, pCustomData );
        } // End If ID's Match

        // Read in the next chunk
        if (!NextChunk( )) ChunksExhausted = true;
    } // Next Chunk
}

//-----------------------------------------------------------------------------
// Name : ReadChunkHeader () (Private)
// Desc : Provided simply to read in the chunk header and test for validity.
//-----------------------------------------------------------------------------
bool CBaseIWF::ReadChunkHeader( CHUNKHEADER * pHeader )
{
    CHUNKHEADER Header, *pTempHeader;
    
    // Read into specified header or our stack one
    if (!pHeader) pTempHeader = &Header; else pTempHeader = pHeader;

    // Validate Requirements
    if (m_CurrentMode != MODE_READ && m_CurrentMode != MODE_READUNFILTERED) throw IWF_ERR_INVALIDPROCESSMODE;
    
    // Read in the chunk signature
    Read( pTempHeader->Signature, HEAD_SIG_SIZE );

    // Compare the signature to see if it was valid.
    if (memcmp( pTempHeader->Signature, HEAD_SIG, HEAD_SIG_SIZE ) != 0) 
        return false;

    // Read in the rest of the header.
    Read( &pTempHeader->ChunkTypeID, sizeof(USHORT) );
    Read( &pTempHeader->ChunkLength, sizeof(ULONG) );
    Read( &pTempHeader->DataLength, sizeof(ULONG) );
    Read( &pTempHeader->AuthorIDLength, sizeof(UCHAR) );
    if (pTempHeader->AuthorIDLength > 0 )
        Read( pTempHeader->AuthorID, pHeader->AuthorIDLength );

    // Success (valid header)!
    return true;
    
}

//-----------------------------------------------------------------------------
// Name : ReadChunkFooter () (Private)
// Desc : Provided simply to read in the chunk footer and test for validity.
//-----------------------------------------------------------------------------
void CBaseIWF::ReadChunkFooter( CHUNKFOOTER * pFooter )
{
    CHUNKFOOTER Footer, *pTempFooter;
    
    // Read into specified footer or our stack one
    if (!pFooter) pTempFooter = &Footer; else pTempFooter = pFooter;

    // Validate Requirements
    if (m_CurrentMode != MODE_READ && m_CurrentMode != MODE_READUNFILTERED) throw IWF_ERR_INVALIDPROCESSMODE;
    
    // Read in the chunk signature
    Read( pTempFooter->Signature, FOOT_SIG_SIZE );

    // Compare the signature to see if it was valid.
    if (memcmp( pTempFooter->Signature, FOOT_SIG, FOOT_SIG_SIZE ) != 0) throw IWF_ERR_INVALIDSIGNATURE;

    // Read in the rest of the footer.
    Read( &pTempFooter->EOCValue, sizeof(USHORT) );

}

//-----------------------------------------------------------------------------
// Name : WriteChunkHeader () (Private)
// Desc : Provided simply to write out the chunk header.
//-----------------------------------------------------------------------------
void CBaseIWF::WriteChunkHeader( CHUNKHEADER * pHeader )
{
    // Validate Requirements
    if (m_CurrentMode != MODE_WRITE) throw IWF_ERR_INVALIDPROCESSMODE;
    
    // Write out header or skip
    if (pHeader) {
        Write( pHeader->Signature, HEAD_SIG_SIZE );
        Write( &pHeader->ChunkTypeID, sizeof(USHORT) );
        Write( &pHeader->ChunkLength, sizeof(ULONG) );
        Write( &pHeader->DataLength, sizeof(ULONG) );
        Write( &pHeader->AuthorIDLength, sizeof(UCHAR) );
        if (pHeader->AuthorIDLength > 0) Write( pHeader->AuthorID, pHeader->AuthorIDLength );
    } else {
        Seek( GetHeaderLength(pHeader), SEEK_CUR );
    } // End If

}

//-----------------------------------------------------------------------------
// Name : WriteChunkFooter () (Private)
// Desc : Provided simply to write out the chunk footer.
//-----------------------------------------------------------------------------
void CBaseIWF::WriteChunkFooter( CHUNKFOOTER * pFooter )
{
    // Validate Requirements
    if (m_CurrentMode != MODE_WRITE) throw IWF_ERR_INVALIDPROCESSMODE;

    // Write out footer or skip
    if (pFooter) {
        Write( pFooter->Signature, FOOT_SIG_SIZE );
        Write( &pFooter->EOCValue, sizeof(USHORT) );
    } else {
        Seek( FOOT_SIG_SIZE + sizeof(USHORT),  SEEK_CUR );
    } // End If

}

//-----------------------------------------------------------------------------
// Name : ProcessChunk () (Virtual)
// Desc : This is essentially an event function that will be triggered during
//        the automatic IWF processing procedures to inform you of any valid
//        chunks that have been encountered, and not filtered out.
// Note : The base implementation may do some extra processing at some point
//        so should ideally be called by the derived classes override anyway.
//-----------------------------------------------------------------------------
void CBaseIWF::ProcessChunk( CHUNKHEADER& Header, LPVOID pCustomData )
{
    // Validate Requirements
    if (m_CurrentMode != MODE_READ && m_CurrentMode != MODE_READUNFILTERED) throw IWF_ERR_INVALIDPROCESSMODE;
}

//-----------------------------------------------------------------------------
// Name : OpenChunk ()
// Desc : Used during writing, this function will open a new chunk of the type
//        specified, log all it's details, and write it out to file.
//-----------------------------------------------------------------------------
void CBaseIWF::OpenChunk( unsigned short ChunkID )
{
    __int64        Position;
    CHUNKDETAILS * Details = NULL;

    // Validate Requirements
    if (m_CurrentMode != MODE_WRITE) throw IWF_ERR_INVALIDPROCESSMODE;
    if (IsLockedChunk(ChunkID)) throw IWF_ERR_CHUNKTYPELOCKED;

    // Allocate and new details structure
    if (!(Details = new CHUNKDETAILS)) throw IWF_ERR_OUTOFMEMORY;
    ZeroMemory(Details, sizeof(CHUNKDETAILS));

    // Store current position
    Position = GetPosition();

    // Fill out the required details (Author must always be written for entities)
    if (!IsStandardChunk( ChunkID ) || ChunkID == CHUNK_ENTITY ) {
        Details->Header.AuthorIDLength = m_AuthorLength;
        if (m_AuthorLength) memcpy(Details->Header.AuthorID, m_pAuthor, m_AuthorLength );
    } // End If Non-Standard
    Details->Header.ChunkTypeID    = ChunkID;
    memcpy(Details->Header.Signature, HEAD_SIG, HEAD_SIG_SIZE );
    Details->Position = Position;
    Details->Header.DataLength = -1;
    
    // Write this chunk to the stream
    WriteChunkHeader( &Details->Header );

    // Store the new open chunk
    AddOpenChunk( Details );

    // If no data length for parent has yet been stored, store it
    if (Details = Details->Parent) {
        if (Details->Header.DataLength < 0) {
            Details->Header.DataLength = (ULONG)(Position - (Details->Position + GetHeaderLength(&Details->Header)));
        } // End If No Data Length
    } // End If

}

//-----------------------------------------------------------------------------
// Name : CloseChunk ()
// Desc : Used during writing to close a specific chunk. This is an extremely
//        important step because it will go ahead and calculate the final
//        length of the chunk written, and step back and insert it into the 
//        chunk header automatically. It also inserts the EOC data into the
//        file to signal the closing of that particular chunk branch.
//-----------------------------------------------------------------------------
void CBaseIWF::CloseChunk()
{
    CHUNKFOOTER    Footer;
    CHUNKDETAILS * Details;
    __int64        FooterPos, LastEOCPos;

    // Validate Requirements
    if (m_CurrentMode != MODE_WRITE) throw IWF_ERR_INVALIDPROCESSMODE;

    // Fill out required details
    memcpy( Footer.Signature, FOOT_SIG, FOOT_SIG_SIZE );
    Footer.EOCValue = CHUNK_EOC_EXIT;
    
    // Store current stream position
    FooterPos = GetPosition();

    // Calculate the length of the current chunk
    if (Details = GetOpenChunkHead()) {
        // Calculate total length of the chunk
        Details->Header.ChunkLength = (ULONG)(FooterPos - (Details->Position + GetHeaderLength(&Details->Header)));
        // If no data length for parent has yet been stored, store it
        if (Details->Header.DataLength < 0) {
            Details->Header.DataLength = (ULONG)(FooterPos - (Details->Position + GetHeaderLength(&Details->Header)));
        } // End If No Data Pos
        // Jump back and write out the length of the chunk & the data pos
        Seek( (long)(Details->Position + HEAD_SIG_SIZE + sizeof(USHORT)), SEEK_SET );
        Write( &Details->Header.ChunkLength, sizeof(ULONG) );
        Write( &Details->Header.DataLength , sizeof(ULONG) );
        // Jump back to the previous EOC (if any) and change it from an EOC_EXIT to a standard EOC
        if (Details = Details->Parent ) LastEOCPos = Details->LastEOCPos; else LastEOCPos = m_LastEOCPos;
        if ( LastEOCPos > 0 ) {
            USHORT StandardEOC = CHUNK_EOC;
            Seek( (long)(LastEOCPos + FOOT_SIG_SIZE), SEEK_SET );
            Write( &StandardEOC, sizeof(USHORT) );
        } // End If Last EOC
    } // End If

    // Jump back to the position we were in before
    Seek( (long)FooterPos, SEEK_SET );

    // Write this footer to the stream
    WriteChunkFooter( &Footer );

    // Remove this from the list and delete it
    if (Details = GetOpenChunkHead()) {
        RemoveOpenChunk( Details );
        delete Details;
    } // End If

    // Store this as the last chunk written under the parent chunk
    if (Details = GetOpenChunkHead()) Details->LastEOCPos = FooterPos; else m_LastEOCPos = FooterPos;

}

//-----------------------------------------------------------------------------
// Name : WriteFileSummary ()
// Desc : Writes out the file summary to the currently attached stream.
//-----------------------------------------------------------------------------
void CBaseIWF::WriteFileSummary()
{

}

//-----------------------------------------------------------------------------
// Name : IsStandardChunk () (Static)
// Desc : Checks the specified chunk to see if it is one of the standard ID's
//-----------------------------------------------------------------------------
bool CBaseIWF::IsStandardChunk( USHORT ChunkTypeID )
{
    // Test if standard value
    switch (ChunkTypeID) {
        case CHUNK_ROOT:
        case CHUNK_FILESUMMARY:
        case CHUNK_CONTAINER:
        case CHUNK_THUMBNAIL:
        case CHUNK_GROUP:
        case CHUNK_MESH:
        case CHUNK_SURFACES:
        case CHUNK_INDICES:
        case CHUNK_VERTEXCOMPONENTS:
        case CHUNK_ENTITY:
        case CHUNK_MATERIALS:
        case CHUNK_TEXTURES:
        case CHUNK_SHADERS:
        case CHUNK_SCRIPTS:
        case CHUNK_COORDSYSINFO:
            return true;
    } // End Switch

    // Non Standard Chunk ID
    return false;
}

//-----------------------------------------------------------------------------
// Name : IsLockedChunk () (Static)
// Desc : Checks the specified chunk to see if it is a LOCKED chunk type
//-----------------------------------------------------------------------------
bool CBaseIWF::IsLockedChunk( USHORT ChunkTypeID )
{
    // Test if locked value
    switch (ChunkTypeID) {
        case CHUNK_ROOT:
        case CHUNK_FILESUMMARY:
            return true;
    } // End Switch

    // Non Locked Chunk ID
    return false;
}

//-----------------------------------------------------------------------------
// Name : GetHeaderLength () (Static)
// Desc : Quick helper function returns the length of the specified header
//-----------------------------------------------------------------------------
unsigned short CBaseIWF::GetHeaderLength( CHUNKHEADER * pHeader )
{
    if (!pHeader) return 0;
    return HEAD_SIG_SIZE + 11 + pHeader->AuthorIDLength;
}

//-----------------------------------------------------------------------------
// Name : RegisterChunkProc ()
// Desc : Function used by application to register a chunk read procedure.
//-----------------------------------------------------------------------------
bool CBaseIWF::RegisterChunkProc( USHORT ChunkID, CHUNKPROC pFunc, LPVOID pContext )
{
    CHUNKCALLBACK * CallBack = m_pChunkProc;

    // First determine if it already exists
    for ( ; CallBack; CallBack = CallBack->Next )
    {
        if ( CallBack->ChunkTypeID == ChunkID ) break;
    } // Next CallBack

    // If we didn't find one, then create one
    if ( !CallBack )
    {
        if (!(CallBack = new CHUNKCALLBACK)) return false;
        CallBack->Next = m_pChunkProc;
        m_pChunkProc = CallBack;

    } // Does not exist

    // Set values in new, or overwrite existing
    CallBack->ChunkTypeID = ChunkID;
    CallBack->pCallback   = pFunc;
    CallBack->pContext    = pContext;

    // Success!
    return true;
}

//-----------------------------------------------------------------------------
// Name : UnregisterChunkProc ()
// Desc : Function used by application to unregister a chunk read procedure.
//-----------------------------------------------------------------------------
void CBaseIWF::UnregisterChunkProc( USHORT ChunkID )
{
    CHUNKCALLBACK * CallBack = m_pChunkProc;
    CHUNKCALLBACK * Previous = NULL;

    // First determine if it already exists
    for ( ; CallBack; CallBack = CallBack->Next )
    {
        if ( CallBack->ChunkTypeID == ChunkID ) break;
        Previous = CallBack;

    } // Next CallBack

    // Bail if it wasn't registered
    if (!CallBack) return;

    // Remove the callback from the linked list
    if ( m_pChunkProc == CallBack ) m_pChunkProc = CallBack->Next;
    if ( Previous ) Previous->Next = CallBack->Next;
    
    // Delete the callback object
    delete CallBack;
}

//-----------------------------------------------------------------------------
// Name : ClearChunkProcs ()
// Desc : Remove all currently registered chunk procedures.
//-----------------------------------------------------------------------------
void CBaseIWF::ClearChunkProcs()
{

    CHUNKCALLBACK * CallBack = m_pChunkProc;
    CHUNKCALLBACK * Next = NULL;

    // First determine if it already exists
    for ( ; CallBack; CallBack = Next )
    {
        Next = CallBack->Next;
        delete CallBack;
    
    } // Next CallBack

    // Clear Variables
    m_pChunkProc = NULL;
}

//-------------------------------------------------------------------------
// CBaseIWF Stream IO functions
// Note : The stream IO functions shown here are for clarity only, they do
//        not support large file sizes as a rule. You should replace these
//        with your own read / write routines. In future versions, the
//        library will expose callback functionality to allow you to read
//        from any source.
//-------------------------------------------------------------------------
size_t CBaseIWF::Read( LPVOID pBuffer, unsigned long BufferSize ) 
{ 
    return fread( pBuffer, 1, BufferSize, m_pFile );
} // End Function

size_t CBaseIWF::Write( LPVOID pBuffer, unsigned long BufferSize ) 
{ 
    return fwrite( pBuffer, 1, BufferSize, m_pFile );   
} // End Function

LPSTR CBaseIWF::ReadString( LPSTR strData, unsigned long BufferSize ) 
{ 
    return fgets( strData, BufferSize, m_pFile );
} // End Function

int CBaseIWF::WriteString( LPCSTR strData ) 
{ 
    return fputs( strData, m_pFile );
} // End Function

int CBaseIWF::Seek( long Offset, int Origin ) 
{ 
    return fseek( m_pFile, Offset, Origin );
} // End Function

int CBaseIWF::Flush( ) 
{ 
    return fflush( m_pFile );
} // End Function

bool CBaseIWF::IsOpen( ) const 
{ 
    return (m_pFile != NULL); 
} // End Function

bool CBaseIWF::IsEOF()
{ 
    return (feof( m_pFile ) != 0);
} // End Function

long CBaseIWF::GetPosition( ) 
{ 
    return ftell( m_pFile );
} // End Function

long CBaseIWF::GetLength() 
{
    return _filelength( m_pFile->_file );
} // End Function

bool CBaseIWF::SetLength( long Length ) 
{
    return (_chsize( m_pFile->_file, Length ) == 0);
} // End Function

//-----------------------------------------------------------------------------
// Name : ReadScriptRef () (Protected)
// Desc : Private function used to quickly load in SCRIPT_REF structures.
//-----------------------------------------------------------------------------
void CBaseIWF::ReadScriptRef( SCRIPT_REF *pScript )
{
    if ( !pScript ) throw IWF_ERR_INVALIDPARAMS;

    // Read in the structure elements
    Read( &pScript->ScriptSource, sizeof(UCHAR) );
    Read( &pScript->ScriptSize, sizeof(USHORT) );
    pScript->ScriptData = NULL;

    // Is there any script data ?
    if ( pScript->ScriptSize > 0 )
    {
        // Allocate and read the script data stored.
        pScript->ScriptData = new char[ pScript->ScriptSize ];
        if ( !pScript->ScriptData ) throw IWF_ERR_OUTOFMEMORY;
        Read( pScript->ScriptData, pScript->ScriptSize );

    } // End if any data
}

//-----------------------------------------------------------------------------
// Name : SeekScriptRef () (Protected)
// Desc : Private function used to quickly seek over any script ref structure.
//-----------------------------------------------------------------------------
void CBaseIWF::SeekScriptRef( )
{
    UCHAR  ScriptSource;
    USHORT ScriptSize;

    // Read in the structure elements
    Read( &ScriptSource, sizeof(UCHAR) );
    Read( &ScriptSize, sizeof(USHORT) );
    
    // Skip over the script data
    if ( ScriptSize > 0 ) Seek( ScriptSize );

}

//-----------------------------------------------------------------------------
// Name : SeekName () (Protected)
// Desc : Private function used to quickly seek over any objects name.
// Note : LengthSize specifies the size in bytes of the 'NameLength' variable
//        stored within the file.
//-----------------------------------------------------------------------------
void CBaseIWF::SeekName( UCHAR LengthSize )
{
    __int64 NameLength = 0;
    Read( &NameLength, LengthSize );
    if (NameLength > 0) Seek( (long)NameLength );
}

//-----------------------------------------------------------------------------
// Name : ReadName () (Protected)
// Desc : Private function used to quickly read any objects name.
// Note : LengthSize specifies the size in bytes of the 'NameLength' variable
//        stored within the file.
//-----------------------------------------------------------------------------
void CBaseIWF::ReadName( UCHAR LengthSize, char ** Name )
{
    __int64 NameLength = 0;
    
    // Read in the name length
    Read( &NameLength, LengthSize );

    if ( NameLength > 0) 
    {
        // Allocate name memory
        *Name = new char[ (long)NameLength + 1 ];
        if ( !(*Name) ) throw IWF_ERR_OUTOFMEMORY;

        // Read the name
        Read( (*Name), (long)NameLength );

        // Insert terminator
        (*Name)[NameLength] = _T('\0');

    } // End if any name
}


//-----------------------------------------------------------------------------
// Name : WriteName () (Protected)
// Desc : Private function used to quickly write any objects name.
// Note : LengthSize specifies the size in bytes of the 'NameLength' variable
//        stored within the file.
//-----------------------------------------------------------------------------
void CBaseIWF::WriteName( UCHAR LengthSize, char * Name )
{
    __int64 NameLength = 0;

    if ( Name ) NameLength = strlen( Name );
    
    // Write in the name length
    Write( &NameLength, LengthSize );

    if ( NameLength > 0 ) 
    {
        // Write the name
        Write( Name, (long)NameLength );

    } // End if any name
}