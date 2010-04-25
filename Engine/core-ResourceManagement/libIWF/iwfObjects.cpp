///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//          _ _ _    _____        _______   ____       _                     //
//         | (_) |__|_ _\ \      / /  ___| | __ )  ___| |_ __ _              //
//         | | | '_ \| | \ \ /\ / /| |_    |  _ \ / _ \ __/ _` |             //
//         | | | |_) | |  \ V  V / |  _|   | |_) |  __/ || (_| |             //
//         |_|_|_.__/___|  \_/\_/  |_|     |____/ \___|\__\__,_|             //
//                                                          v0.0.2           //
//                                                                           //
//---------------------------------------------------------------------------//
//                                                                           //
//                         File: iwfObjects.cpp                              //
//                                                                           //
//                           Date: 26/08/2002                                //
//                                                                           //
//    Desc: This file houses the various object / mesh related classes.      //
//                                                                           //
//   Copyright (c) 1997-2002 Daedalus Developments. All rights reserved.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// iwfObjects Specific Includes
//-----------------------------------------------------------------------------
#include "core-ResourceManagement\libIWF\iwfObjects.h"

//-----------------------------------------------------------------------------
// Name : iwfMesh () (Constructor)
// Desc : iwfMesh Class Constructor
//-----------------------------------------------------------------------------
iwfMesh::iwfMesh()
{
	// Reset / Clear all required values
    Name            = NULL;
    Components      = 0;
    Style           = 0;
    CustomDataSize  = 0;
    CustomData      = NULL;
    SurfaceCount    = 0;
    Surfaces        = NULL;
}

//-----------------------------------------------------------------------------
// Name : iwfMesh () (Alternate Constructor)
// Desc : iwfMesh Class Constructor, adds specified number of surfaces
//-----------------------------------------------------------------------------
iwfMesh::iwfMesh( ULONG Count )
{   
    // Reset / Clear all required values
    Name            = NULL;
    Components      = 0;
    Style           = 0;
    CustomDataSize  = 0;
    CustomData      = NULL;
    SurfaceCount    = 0;
    Surfaces        = NULL;

    // Add Surfaces
    AddSurfaces( Count );
}

//-----------------------------------------------------------------------------
// Name : ~iwfMesh () (Destructor)
// Desc : iwfMesh Class Destructor
//-----------------------------------------------------------------------------
iwfMesh::~iwfMesh()
{
	// Release our mesh components
    if ( Surfaces ) 
    {
        // Delete all individual surfaces in the array.
        for ( ULONG i = 0; i < SurfaceCount; i++ )
        {
            if ( Surfaces[i] ) delete Surfaces[i];
        
        } // Next Surface

        // Free up the array itself
        delete []Surfaces;
    
    } // End if

    // Delete Name & Custom Data
    if ( Name ) delete[] Name;
    if ( CustomData ) delete []CustomData;

    // Clear variables
    Surfaces        = NULL;
    SurfaceCount    = 0;
    Name            = NULL;
    CustomDataSize  = 0;
    CustomData      = NULL;
}

//-----------------------------------------------------------------------------
// Name : AddSurfaces()
// Desc : Adds a surface, or multiple surfaces, to this mesh.
// Note : Returns the index for the first surface added, or -1 on failure.
//-----------------------------------------------------------------------------
long iwfMesh::AddSurfaces( ULONG Count )
{

    iwfSurface ** pSurfaceBuffer = NULL;
    
    // Allocate new resized array
    if (!( pSurfaceBuffer = new iwfSurface*[ SurfaceCount + Count ] )) return -1;

    // Clear out slack pointers
    ZeroMemory( &pSurfaceBuffer[ SurfaceCount ], Count * sizeof( iwfSurface* ) );

    // Existing Data?
    if ( Surfaces )
    {
        // Copy old data into new buffer
        memcpy( pSurfaceBuffer, Surfaces, SurfaceCount * sizeof( iwfSurface* ) );

        // Release old buffer
        delete []Surfaces;

    } // End if
    
    // Store pointer for new buffer
    Surfaces = pSurfaceBuffer;

    // Allocate new surface pointers
    for ( UINT i = 0; i < Count; i++ )
    {
        // Allocate new poly
        if (!( Surfaces[ SurfaceCount ] = new iwfSurface() )) return -1;

        // Increase overall surface count
        SurfaceCount++;

    } // Next Polygon
    
    // Return first surface
    return SurfaceCount - Count;
}

//-----------------------------------------------------------------------------
// Name : iwfSurface () (Constructor)
// Desc : iwfSurface Class Constructor
//-----------------------------------------------------------------------------
iwfSurface::iwfSurface()
{
	// Reset / Clear all required values
    Components          = 0;
    Normal              = VECTOR3( 0.0f, 0.0f, 0.0f );
    ZBias               = 0;
    Style               = 0;
    RenderEffect        = -1;
    ChannelCount        = 0;
    TextureIndices      = NULL;
    MaterialIndices     = NULL;
    ShaderIndices       = NULL;
    BlendModes          = NULL;
    CustomDataSize      = 0;
    CustomData          = NULL;
    
    VertexComponents    = 0;
    VertexFlags         = 0;
    VertexCount         = 0;
    TexChannelCount     = 0;
    TexCoordSize        = NULL;

    IndexVPool          = 0;
    IndexFlags          = 0;
    IndexCount          = 0;
    
    Vertices            = NULL;
    Indices             = NULL;
}

//-----------------------------------------------------------------------------
// Name : iwfSurface () (Alternate Constructor)
// Desc : iwfSurface Class Constructor, adds specified number of vertices
//-----------------------------------------------------------------------------
iwfSurface::iwfSurface( USHORT Count )
{
	// Reset / Clear all required values
    Components          = 0;
    Normal              = VECTOR3( 0.0f, 0.0f, 0.0f );
    ZBias               = 0;
    Style               = 0;
    RenderEffect        = -1;
    ChannelCount        = 0;
    TextureIndices      = NULL;
    MaterialIndices     = NULL;
    ShaderIndices       = NULL;
    BlendModes          = NULL;
    CustomDataSize      = 0;
    CustomData          = NULL;
    
    VertexComponents    = 0;
    VertexFlags         = 0;
    VertexCount         = 0;
    TexChannelCount     = 0;
    TexCoordSize        = NULL;

    IndexVPool          = 0;
    IndexFlags          = 0;
    IndexCount          = 0;
    
    Vertices            = NULL;
    Indices             = NULL;

    // Add vertices
    AddVertices( Count );
}

//-----------------------------------------------------------------------------
// Name : ~iwfSurface () (Destructor)
// Desc : iwfSurface Class Destructor
//-----------------------------------------------------------------------------
iwfSurface::~iwfSurface()
{
	// Release our vertices, indices and shaderindices
    if ( Vertices                ) delete []Vertices;
    if ( Indices                 ) delete []Indices;
    if ( TextureIndices          ) delete []TextureIndices;
    if ( MaterialIndices         ) delete []MaterialIndices;
    if ( ShaderIndices           ) delete []ShaderIndices;
    if ( BlendModes              ) delete []BlendModes;
    if ( TexCoordSize            ) delete []TexCoordSize;
    if ( CustomData              ) delete []CustomData;
    
    // Clear variables
    Components          = 0;
    ZBias               = 0;
    Style               = 0;
    RenderEffect        = -1;
    ChannelCount        = 0;
    TextureIndices      = NULL;
    MaterialIndices     = NULL;
    ShaderIndices       = NULL;
    BlendModes          = NULL;
    CustomDataSize      = 0;
    CustomData          = NULL;
    
    VertexComponents    = 0;
    VertexFlags         = 0;
    VertexCount         = 0;
    TexChannelCount     = 0;
    TexCoordSize        = NULL;

    IndexVPool          = 0;
    IndexFlags          = 0;
    IndexCount          = 0;
    
    Vertices            = NULL;
    Indices             = NULL;
}

//-----------------------------------------------------------------------------
// Name : AddVertices()
// Desc : Adds a vertex, or multiple vertices, to this surface.
// Note : Returns the index for the first vertex added, or -1 on failure.
//-----------------------------------------------------------------------------
long iwfSurface::AddVertices( USHORT Count )
{
    iwfVertex * pVertexBuffer = NULL;
    
    // Allocate new resized array
    if (!( pVertexBuffer = new iwfVertex[ VertexCount + Count ] )) return -1;

    // Existing Data?
    if ( Vertices )
    {
        // Copy old data into new buffer
        memcpy( pVertexBuffer, Vertices, VertexCount * sizeof(iwfVertex) );

        // Release old buffer
        delete []Vertices;

    } // End if

    // Store pointer for new buffer
    Vertices = pVertexBuffer;
    VertexCount += Count;

    // Return first vertex
    return VertexCount - Count;
}

//-----------------------------------------------------------------------------
// Name : AddIndices()
// Desc : Adds an index, or multiple indices, to this surface.
// Note : Returns the index for the first index added, or -1 on failure.
//-----------------------------------------------------------------------------
long iwfSurface::AddIndices( USHORT Count )
{
    ULONG * pIndexBuffer = NULL;
    
    // Allocate new resized array
    if (!( pIndexBuffer = new ULONG[ IndexCount + Count ] )) return -1;

    // Existing Data?
    if ( Indices )
    {
        // Copy old data into new buffer
        memcpy( pIndexBuffer, Indices, IndexCount * sizeof(ULONG) );

        // Release old buffer
        delete []Indices;

    } // End if

    // Store pointer for new buffer
    Indices = pIndexBuffer;
    IndexCount += Count;

    // Return first Index
    return IndexCount - Count;
}


//-----------------------------------------------------------------------------
// Name : AddChannels()
// Desc : Creates the various channel based information.
// Note : Returns the index for the first channel added, or -1 on failure.
//-----------------------------------------------------------------------------
long iwfSurface::AddChannels( UCHAR Count )
{
    short      * pIndexBuffer = NULL;
    BLEND_MODE * pBlendBuffer = NULL;
    
    // ******* Texture Indices ********

    // Allocate new resized array
    if (!( pIndexBuffer = new short[ ChannelCount + Count ] )) return -1;

    // Existing Data?
    if ( Indices )
    {
        // Copy old data into new buffer
        memcpy( pIndexBuffer, TextureIndices, ChannelCount * sizeof(short) );

        // Release old buffer
        delete []TextureIndices;

    } // End if

    // Store pointer for new buffer
    TextureIndices = pIndexBuffer;
    
    
    // ******* Material Indices ********

    // Allocate new resized array
    if (!( pIndexBuffer = new short[ ChannelCount + Count ] )) return -1;

    // Existing Data?
    if ( Indices )
    {
        // Copy old data into new buffer
        memcpy( pIndexBuffer, MaterialIndices, ChannelCount * sizeof(short) );

        // Release old buffer
        delete []MaterialIndices;

    } // End if

    // Store pointer for new buffer
    MaterialIndices = pIndexBuffer;
    
    // ******* Shader Indices ********

    // Allocate new resized array
    if (!( pIndexBuffer = new short[ ChannelCount + Count ] )) return -1;

    // Existing Data?
    if ( Indices )
    {
        // Copy old data into new buffer
        memcpy( pIndexBuffer, ShaderIndices, ChannelCount * sizeof(short) );

        // Release old buffer
        delete []ShaderIndices;

    } // End if

    // Store pointer for new buffer
    ShaderIndices = pIndexBuffer;
    
    // ******* BlendModes ********

    // Allocate new resized array
    if (!( pBlendBuffer = new BLEND_MODE[ ChannelCount + Count ] )) return -1;

    // Existing Data?
    if ( Indices )
    {
        // Copy old data into new buffer
        memcpy( pBlendBuffer, BlendModes, ChannelCount * sizeof(BLEND_MODE) );

        // Release old buffer
        delete []BlendModes;

    } // End if

    // Store pointer for new buffer
    BlendModes = pBlendBuffer;
    
    // Increase Channel Count
    ChannelCount += Count;

    // Return first new channel Index
    return ChannelCount - Count;
}

//-----------------------------------------------------------------------------
// Name : iwfEntity () (Constructor)
// Desc : iwfEntity Class Constructor.
//-----------------------------------------------------------------------------
iwfEntity::iwfEntity( )
{
    // Reset / Cleare required vars
    EntityTypeID    = 0;
    Name            = NULL;
    DataSize        = 0;
    DataArea        = NULL;
}

//-----------------------------------------------------------------------------
// Name : iwfEntity () (Alternate Constructor)
// Desc : iwfEntity Class Constructor, allocates specified amount of data
//-----------------------------------------------------------------------------
iwfEntity::iwfEntity( ULONG Size )
{
    // Reset / Cleare required vars
    EntityTypeID    = 0;
    Name            = NULL;
    DataSize        = 0;
    DataArea        = NULL;

    // Allocate memory
    AllocateData( Size );
}

//-----------------------------------------------------------------------------
// Name : ~iwfEntity () (Destructor)
// Desc : iwfEntity Class Destructor
//-----------------------------------------------------------------------------
iwfEntity::~iwfEntity()
{
    // Release memory
	if ( Name     ) delete[] Name;
    if ( DataArea ) delete[] DataArea;

    // Reset Variables
	Name     = NULL;
    DataSize = 0;
    DataArea = NULL;

}

//-----------------------------------------------------------------------------
// Name : AllocateData ()
// Desc : Allocates the specified amount of data.
// Note : This function releases any previously owned data.
//-----------------------------------------------------------------------------
bool iwfEntity::AllocateData( ULONG Size )
{
    // Release memory
    if ( DataArea ) delete[] DataArea;

    // Reset Variables
    DataSize = Size;
    DataArea = new UCHAR[ Size ];
    
    // Success ?
    return ( DataArea != NULL );

}

//-----------------------------------------------------------------------------
// Name : EntityTypeMatches ()
// Desc : Utility function to check and see if this is of the expected type.
//-----------------------------------------------------------------------------
bool iwfEntity::EntityTypeMatches( USHORT nEntityTypeID, short nAuthorIDLength /* = -1 */, const UCHAR pAuthorID[] /* = NULL */ )
{
    // Check for failure cases
    if ( EntityTypeID != nEntityTypeID ) return false;
    if ( nAuthorIDLength >= 0 && (AuthorIDLength != (signed)nAuthorIDLength) ) return false;
    if ( (pAuthorID != NULL && AuthorIDLength > 0 ) && memcmp( AuthorID, pAuthorID, AuthorIDLength ) != 0 ) return false;

    // Matches!!
    return true;
}

//-----------------------------------------------------------------------------
// Name : EntityAuthorMatches ()
// Desc : Utility function to check and see if this is by the expected author
//-----------------------------------------------------------------------------
bool iwfEntity::EntityAuthorMatches( UCHAR nAuthorIDLength, const UCHAR pAuthorID[] )
{
    // Check for failure cases
    if ( AuthorIDLength != (signed)nAuthorIDLength ) return false;
    if ( AuthorIDLength > 0 && memcmp( AuthorID, pAuthorID, AuthorIDLength ) != 0 ) return false;

    // Matches!!
    return true;
}

//-----------------------------------------------------------------------------
// Name : iwfMaterial () (Constructor)
// Desc : iwfMaterial Class Constructor.
//-----------------------------------------------------------------------------
iwfMaterial::iwfMaterial( )
{
    // Reset / Clear required vars
    Name            = NULL;
    Power           = 0.0f;
    
    // Zero out structures
    ZeroMemory( &Diffuse , sizeof(COLOUR_VALUE) );
    ZeroMemory( &Ambient , sizeof(COLOUR_VALUE) );
    ZeroMemory( &Emissive, sizeof(COLOUR_VALUE) );
    ZeroMemory( &Specular, sizeof(COLOUR_VALUE) );
}

//-----------------------------------------------------------------------------
// Name : ~iwfMaterial () (Destructor)
// Desc : iwfMaterial Class Destructor
//-----------------------------------------------------------------------------
iwfMaterial::~iwfMaterial()
{
    // Release memory
    if ( Name ) delete[] Name;

    // Reset Variables
    Name = NULL;

}

//-----------------------------------------------------------------------------
// Name : iwfGroup () (Constructor)
// Desc : iwfGroup Class Constructor.
//-----------------------------------------------------------------------------
iwfGroup::iwfGroup( )
{
    // Reset / Clear required vars
    Name            = NULL;
    ChildCount      = 0;
    Children        = NULL;

}

//-----------------------------------------------------------------------------
// Name : ~iwfGroup () (Destructor)
// Desc : iwfGroup Class Destructor
//-----------------------------------------------------------------------------
iwfGroup::~iwfGroup()
{
    // Release memory
    if ( Name ) delete[] Name;

    // Destroy child objects
    for ( ULONG i = 0; i < ChildCount; i++ )
    {
        // Destroy the child objects
        if ( Children[i] ) delete Children[i];
    
    } // Next Child

    // Destroy the children array
    if ( Children ) delete []Children;

    // Reset Variables
    Name        = NULL;
    ChildCount  = 0;
    Children    = NULL;
}

//-----------------------------------------------------------------------------
// Name : AddChild()
// Desc : Adds a child, or multiple children, to this group.
// Note : Returns the index for the first child added, or -1 on failure.
//-----------------------------------------------------------------------------
long iwfGroup::AddChild( ULONG Count )
{

    IIWFObject ** pChildBuffer = NULL;
    
    // Allocate new resized array
    if (!( pChildBuffer = new IIWFObject*[ ChildCount + Count ] )) return -1;

    // Clear out slack pointers
    ZeroMemory( &pChildBuffer[ ChildCount ], Count * sizeof( IIWFObject* ) );

    // Existing Data?
    if ( Children )
    {
        // Copy old data into new buffer
        memcpy( pChildBuffer, Children, ChildCount * sizeof( IIWFObject* ) );

        // Release old buffer
        delete []Children;

    } // End if
    
    // Store pointer for new buffer
    Children = pChildBuffer;
    ChildCount += Count;
    
    // Return first surface
    return ChildCount - Count;
}


//-----------------------------------------------------------------------------
// Name : ~iwfVertex () (Destructor)
// Desc : iwfVertex Class Destructor
//-----------------------------------------------------------------------------
iwfVertex::~iwfVertex()
{
    // Release memory
    if ( TexCoords )
    {
        for ( ULONG i = 0; i < TexChanCount; i++ )
        {
            // Release components
            if ( TexCoords[i] ) delete TexCoords[i];

        } // Next Channel

        // Release array
        delete []TexCoords;

    } // End if TexCoords
    
    // Reset Variables
    TexCoords   = NULL;
    
}

//-----------------------------------------------------------------------------
// Name : AllocateTexCoords () 
// Desc : Allocate the texture coordinates for this vertex
//-----------------------------------------------------------------------------
bool iwfVertex::AllocateTexCoords( UCHAR ChannelCount, UCHAR ChannelSizes[] )
{
    // Release any old data.
    if ( TexCoords )
    {
        for ( ULONG i = 0; i < TexChanCount; i++ )
        {
            // Release components
            if ( TexCoords[i] ) delete TexCoords[i];

        } // Next Channel

        // Release array
        delete []TexCoords;
        TexCoords = NULL;

    } // End if TexCoords

    // Allocate the channel array
    TexCoords = new float*[ChannelCount];
    if (!TexCoords) return false;
    TexChanCount = ChannelCount;

    // Allocate each channels components
    for ( ULONG i = 0; i < TexChanCount; i++ )
    {
        // Skip if there are no components
        if ( ChannelSizes[ i ] == 0 ) continue;

        // Allocate the components
        TexCoords[i] = new float[ChannelSizes[i]];
        if (!TexCoords[i]) return false;

    } // Next Channel

    // Success!!
    return true;
}

//-----------------------------------------------------------------------------
// Name : GetTexCoord () 
// Desc : Quick wrapper to allow easy access to texture coordinates
//-----------------------------------------------------------------------------
float iwfVertex::GetTexCoord( UCHAR Channel, UCHAR Component ) const
{
    // Check if OOB
    if ( Channel >= TexChanCount ) return 0.0f;
    return (TexCoords[Channel])[Component];
}

//-----------------------------------------------------------------------------
// Desc : iwfTexture member functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : iwfTexture () (Constructor)
// Desc : iwfTexture Class Constructor.
//-----------------------------------------------------------------------------
iwfTexture::iwfTexture( )
{
    // Reset / Clear required vars
    TextureSource   = 0;
    Name            = NULL;
    TextureFormat   = 0;
    TextureSize     = 0;
    TextureData     = NULL;
}

//-----------------------------------------------------------------------------
// Name : iwfTexture () (Alternate Constructor)
// Desc : iwfTexture Class Constructor, allocates specified amount of data
//-----------------------------------------------------------------------------
iwfTexture::iwfTexture( ULONG Size )
{
    // Reset / Clear required vars
    TextureSource   = 0;
    Name            = NULL;
    TextureFormat   = 0;
    TextureSize     = 0;
    TextureData     = NULL;

    // Allocate memory
    AllocateData( Size );
}

//-----------------------------------------------------------------------------
// Name : ~iwfTexture () (Destructor)
// Desc : iwfTexture Class Destructor
//-----------------------------------------------------------------------------
iwfTexture::~iwfTexture()
{
    // Release memory
    if ( Name        ) delete[] Name;
    if ( TextureData ) delete[] TextureData;

    // Reset Variables
    TextureData = NULL;
    Name        = NULL;
}

//-----------------------------------------------------------------------------
// Name : AllocateData ()
// Desc : Allocates the specified amount of data.
// Note : This function releases any previously owned data.
//-----------------------------------------------------------------------------
bool iwfTexture::AllocateData( ULONG Size )
{
    // Release memory
    if ( TextureData ) delete[] TextureData;

    // Reset Variables
    TextureSize = Size;
    TextureData = new UCHAR[ Size ];

    // Success ?
    return ( TextureData != NULL );

}

//-----------------------------------------------------------------------------
// Desc : iwfScriptRef member functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : iwfScriptRef () (Constructor)
// Desc : iwfScriptRef Class Constructor.
//-----------------------------------------------------------------------------
iwfScriptRef::iwfScriptRef( )
{
    // Reset / Clear required vars
    ScriptSource    = 0;
    Name            = NULL;
    ScriptSource    = 0;
    ScriptSize      = 0;
    ScriptData      = NULL;
}

//-----------------------------------------------------------------------------
// Name : iwfScriptRef () (Alternate Constructor)
// Desc : iwfScriptRef Class Constructor, allocates specified amount of data
//-----------------------------------------------------------------------------
iwfScriptRef::iwfScriptRef( ULONG Size )
{
    // Reset / Clear required vars
    ScriptSource    = 0;
    Name            = NULL;
    ScriptSource    = 0;
    ScriptSize      = 0;
    ScriptData      = NULL;

    // Allocate memory
    AllocateData( Size );
}

//-----------------------------------------------------------------------------
// Name : ~iwfScriptRef () (Destructor)
// Desc : iwfScriptRef Class Destructor
//-----------------------------------------------------------------------------
iwfScriptRef::~iwfScriptRef()
{
    // Release memory
    if ( Name       ) delete[] Name;
    if ( ScriptData ) delete[] ScriptData;

    // Reset Variables
    ScriptData = NULL;
    Name       = NULL;
}

//-----------------------------------------------------------------------------
// Name : AllocateData ()
// Desc : Allocates the specified amount of data.
// Note : This function releases any previously owned data.
//-----------------------------------------------------------------------------
bool iwfScriptRef::AllocateData( ULONG Size )
{
    // Release memory
    if ( ScriptData ) delete[] ScriptData;

    // Reset Variables
    ScriptSize = (USHORT)Size;
    ScriptData = new UCHAR[ Size ];

    // Success ?
    return ( ScriptData != NULL );

}

//-----------------------------------------------------------------------------
// Desc : iwfScript member functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : iwfScript () (Constructor)
// Desc : iwfScript Class Constructor.
//-----------------------------------------------------------------------------
iwfScript::iwfScript( )
{
    // Reset / Clear required vars
    Name = NULL;
}

//-----------------------------------------------------------------------------
// Name : ~iwfScriptRef () (Destructor)
// Desc : iwfScriptRef Class Destructor
//-----------------------------------------------------------------------------
iwfScript::~iwfScript()
{
    // Release memory
    if ( Name ) delete[] Name;
    
    // Reset Variables
    Name = NULL;
}