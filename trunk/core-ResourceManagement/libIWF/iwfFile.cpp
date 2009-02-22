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
//                          File: iwfFile.cpp                                //
//                                                                           //
//                           Date: 26/08/2002                                //
//                                                                           //
//            Desc: Performs complete file loading and storage.              //
//                                                                           //
//   Copyright (c) 1997-2002 Daedalus Developments. All rights reserved.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// iwfFile Specific Includes
//-----------------------------------------------------------------------------
#include "iwfFile.h"
#include "iwfObjects.h"

//-----------------------------------------------------------------------------
// Name : CFileIWF () (Constructor)
// Desc : CFileIWF Class Constructor
//-----------------------------------------------------------------------------
CFileIWF::CFileIWF()
{
    // Reset / Clear all required values
    m_LoadFlags = 0;
}

//-----------------------------------------------------------------------------
// Name : ~CFileIWF () (Destructor)
// Desc : CFileIWF Class Destructor
//-----------------------------------------------------------------------------
CFileIWF::~CFileIWF()
{
    ClearObjects();
}

//-----------------------------------------------------------------------------
// Name : ClearObjects () (Destructor)
// Desc : Clears out all loaded objects.
//-----------------------------------------------------------------------------
void CFileIWF::ClearObjects()
{
    unsigned long i;

    // Destroy any loaded meshes
    for ( i = 0; i < m_vpMeshList.size(); i++ )
    {
        if ( m_vpMeshList[i] ) delete m_vpMeshList[i];

    } // Next Mesh

    // Destroy any loaded textures
    for ( i = 0; i < m_vpTextureList.size(); i++ )
    {
        if ( m_vpTextureList[i] ) delete m_vpTextureList[i];

    } // Next Texture

    // Destroy any loaded shaders
    for ( i = 0; i < m_vpShaderList.size(); i++ )
    {
        if ( m_vpShaderList[i] ) delete m_vpShaderList[i];

    } // Next Shader

    // Destroy any loaded maerials
    for ( i = 0; i < m_vpMaterialList.size(); i++ )
    {
        if ( m_vpMaterialList[i] ) delete m_vpMaterialList[i];

    } // End If

    // Destroy any loaded entities
    for ( i = 0; i < m_vpEntityList.size(); i++ )
    {
        if ( m_vpEntityList[i] ) delete m_vpEntityList[i];

    } // End If

    // Destroy any loaded groups
    for ( i = 0; i < m_vpGroupList.size(); i++ )
    {
        if ( m_vpGroupList[i] ) delete m_vpGroupList[i];

    } // End If

    // Clear the vectors
    m_vpMeshList.clear();
    m_vpTextureList.clear();
    m_vpMaterialList.clear();
    m_vpEntityList.clear();
    m_vpShaderList.clear();
    m_vpGroupList.clear();

}

//-----------------------------------------------------------------------------
// Name : Load ()
// Desc : Starts off the IWF loading process
//-----------------------------------------------------------------------------
void CFileIWF::Load( LPCSTR FileName, ULONG Flags )
{
    IWFFILEHEADER FileHeader;

    // First attempt to open the file.
    Open( FileName, MODE_READ, &FileHeader );

    
    // Note : Registering is not required, you can process all the
    //        encountered chunks by overloading the ProcessChunk 
    //        function, from there you can choose how to handle 
    //        each individual chunk type manually.

    // Attempt to register all the callback procedures
    RegisterChunkProc( CHUNK_GROUP           , ReadGroup    , this );
    RegisterChunkProc( CHUNK_MESH            , ReadMesh     , this );
    RegisterChunkProc( CHUNK_SURFACES        , ReadSurfaces , this );
    RegisterChunkProc( CHUNK_INDICES         , ReadIndices  , this );
    RegisterChunkProc( CHUNK_VERTEXCOMPONENTS, ReadVertices , this );
    RegisterChunkProc( CHUNK_TEXTURES        , ReadTextures , this );
    RegisterChunkProc( CHUNK_MATERIALS       , ReadMaterials, this );
    RegisterChunkProc( CHUNK_ENTITY          , ReadEntity   , this );
    RegisterChunkProc( CHUNK_SHADERS         , ReadShaders  , this );
    RegisterChunkProc( CHUNK_SCRIPTS         , ReadScripts  , this );

    // Store flags
    m_LoadFlags = Flags;

    // Start processing the IWF for loading
    ProcessIWF();
}

//-----------------------------------------------------------------------------
// Name : ReadScriptRef () (Protected)
// Desc : Private function used to quickly load in iwfScriptRef classes.
//-----------------------------------------------------------------------------
void CFileIWF::ReadScriptRef( iwfScriptRef *pScript )
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
        pScript->AllocateData( pScript->ScriptSize );
        if ( !pScript->ScriptData ) throw IWF_ERR_OUTOFMEMORY;
        Read( pScript->ScriptData, pScript->ScriptSize );

    } // End if any data
}

//-----------------------------------------------------------------------------
// Name : ReadGroup () (Static CHUNKPROC)
// Desc : Function associated with the CHUNK_GROUP type ID.
//-----------------------------------------------------------------------------
void CFileIWF::ReadGroup( LPVOID pContext, CHUNKHEADER& Header, LPVOID pCustomData )
{
    PARENTINFO  StepInfo, * pParentInfo;
    CFileIWF * pFile = (CFileIWF*)pContext;

    // Should we retain group information ?
    if ( pFile->m_LoadFlags & IWF_RETAIN_GROUPS )
    {
        // Obtain the information passed from the parent
        pParentInfo = (PARENTINFO*)pCustomData;

        // Allocate a new group object
        iwfGroup * Group = new iwfGroup;
        if (!Group) throw IWF_ERR_OUTOFMEMORY;

        // Read in the group name
        pFile->ReadName( sizeof(UCHAR), &Group->Name );

        // What should we do with the parent ?
        if ( pParentInfo )
        {
            if ( pParentInfo->ChunkID == CHUNK_GROUP )
            {
                // Add us to our parent
                iwfGroup * Parent = ((iwfGroup*)pParentInfo->Data1);
                if ( Parent->AddChild( 1 ) < 0 ) throw IWF_ERR_OUTOFMEMORY;
                Parent->Children[ Parent->ChildCount - 1 ] = Group;

            } // End if adding us to a group
            else
            {
                // We are only allowed to add ourselves to another group
                throw IWF_ERR_MALFORMED;

            } // End if malformed

        } // End if Attach to parent
        else
        {
            // Add this to the main group list
            pFile->m_vpGroupList.push_back( Group );

        } // End if no parent

        // Build info to pass to child
        StepInfo.ChunkID    = Header.ChunkTypeID;
        StepInfo.Data1      = (__int64)Group;
        StepInfo.Data2      = 0;
    
        // Step into this chunk
        pFile->StepInto( &StepInfo );

    } // End if Retain Group Information
    else
    {
        // Simply step into children and store data
        pFile->StepInto( NULL );

    } // End if discard groups
    
}

//-----------------------------------------------------------------------------
// Name : ReadMesh () (Static CHUNKPROC)
// Desc : Function associated with the CHUNK_MESH type ID.
//-----------------------------------------------------------------------------
void CFileIWF::ReadMesh( LPVOID pContext, CHUNKHEADER& Header, LPVOID pCustomData )
{
    PARENTINFO  StepInfo, * pParentInfo;
    CFileIWF   *pFile = (CFileIWF*)pContext;

    // Obtain the information passed from the parent
    pParentInfo = (PARENTINFO*)pCustomData;

    // Allocate a new brush object
    iwfMesh * Mesh = new iwfMesh;
    if (!Mesh) throw IWF_ERR_OUTOFMEMORY;

    // Should we retain group information ?
    if ( pFile->m_LoadFlags & IWF_RETAIN_GROUPS && pParentInfo )
    {
        if ( pParentInfo->ChunkID == CHUNK_GROUP )
        {
            // Add us to our parent
            iwfGroup * Parent = ((iwfGroup*)pParentInfo->Data1);
            if ( Parent->AddChild( 1 ) < 0 ) throw IWF_ERR_OUTOFMEMORY;
            Parent->Children[ Parent->ChildCount - 1 ] = Mesh;

        } // End if adding us to a group
        else
        {
            // We are only allowed to add ourselves to another group
            throw IWF_ERR_MALFORMED;

        } // End if malformed

    } // End if Attach to parent
    else
    {
        // Add this to the loaded mesh list.
        pFile->m_vpMeshList.push_back( Mesh );

    } // End if no parent

    // Read in the object name
    pFile->ReadName( sizeof(UCHAR), &Mesh->Name );

    // Skip over both reserved variables
    pFile->Seek( sizeof(ULONG) * 2 );
    
    // Read in the mesh component information
    pFile->Read( &Mesh->Components, sizeof( ULONG ) );

    // Now read in each individual mesh component
    if ( Mesh->Components & MCOMPONENT_OMATRIX ) pFile->Read( &Mesh->ObjectMatrix, sizeof(MATRIX4) );
    if ( Mesh->Components & MCOMPONENT_BOUNDS  ) pFile->Read( &Mesh->Bounds, sizeof(AA_BBOX) );
    if ( Mesh->Components & MCOMPONENT_STYLE   ) pFile->Read( &Mesh->Style, sizeof(ULONG) );
    
    // If the custom data area is included, then read it
    if ( Mesh->Components & MCOMPONENT_CUSTOMDATA ) 
    {
        pFile->Read( &Mesh->CustomDataSize, sizeof(USHORT) );
        if ( Mesh->CustomDataSize )
        {
            // Allocate the custom data area
            Mesh->CustomData = new UCHAR[ Mesh->CustomDataSize ];
            if ( !Mesh->CustomData ) throw IWF_ERR_OUTOFMEMORY;

            // Read the data
            pFile->Read( &Mesh->CustomData, Mesh->CustomDataSize );

        } // End if any data
        
    } // End if has custom area

    // Reset Informant Variables
    pFile->m_bLoadedVNormals = false;
    
    // Build Info to pass to child
    StepInfo.ChunkID  = Header.ChunkTypeID;
    StepInfo.Data1    = (__int64)Mesh;
    StepInfo.Data2    = 0;

    // Step into this chunk
    pFile->StepInto( &StepInfo );

    // Recalculate any missing data
    //if ( !(Components & MCOMPONENT_BOUNDS) ) Mesh->CalculateBoundingBox();

    // If any required chunks were not provided...
    //if (!pFile->m_bLoadedVNormals) Mesh->CalculateVertexNormals();

}

//-----------------------------------------------------------------------------
// Name : ReadSurfaces () (Static CHUNKPROC)
// Desc : Function associated with the CHUNK_SURFACES type ID.
//-----------------------------------------------------------------------------
void CFileIWF::ReadSurfaces( LPVOID pContext, CHUNKHEADER& Header, LPVOID pCustomData )
{
    CFileIWF   *pFile = (CFileIWF*)pContext;
    PARENTINFO  StepInfo, * pParentInfo;
    USHORT      IndexCount = 0, VertexCount = 0;
    ULONG       SurfaceCount;
   
    // Obtain the information passed from the parent
    pParentInfo = (PARENTINFO*)pCustomData;
    
    // Validate Requirements
    if (!pParentInfo || !pParentInfo->Data1 ) throw IWF_ERR_MALFORMED;
    if ( pParentInfo->ChunkID != CHUNK_MESH ) throw IWF_ERR_MALFORMED;

    // Obtain the mesh passed from the parent.
    iwfMesh * Mesh = (iwfMesh*)pParentInfo->Data1;

    // Read in the number of surfaces contained within this chunk
    pFile->Read( &SurfaceCount, sizeof(ULONG) );

    // Allocate this many surfaces within the mesh
    if (Mesh->AddSurfaces( SurfaceCount ) < 0) throw IWF_ERR_OUTOFMEMORY;

    // Loop through each surface, reading any data stored
    for ( ULONG i = 0; i < SurfaceCount; i++ )
    {
        iwfSurface * Surface = Mesh->Surfaces[i];

        // Read mandatory variables
        pFile->Read( &Surface->Components, sizeof(ULONG) );

        // Read in each individual component
        if ( Surface->Components & SCOMPONENT_NORMAL ) pFile->Read( &Surface->Normal, sizeof(VECTOR3) );
        if ( Surface->Components & SCOMPONENT_ZBIAS  ) pFile->Read( &Surface->ZBias , sizeof(UCHAR) );
        if ( Surface->Components & SCOMPONENT_STYLE  ) pFile->Read( &Surface->Style , sizeof(ULONG) );
        if ( Surface->Components & SCOMPONENT_EFFECT ) pFile->Read( &Surface->RenderEffect, sizeof(short) );

        bool HasTextures   = ((Surface->Components & SCOMPONENT_TEXTURES  ) != 0);
        bool HasMaterials  = ((Surface->Components & SCOMPONENT_MATERIALS ) != 0);
        bool HasShaders    = ((Surface->Components & SCOMPONENT_SHADERS   ) != 0);
        bool HasBlendModes = ((Surface->Components & SCOMPONENT_BLENDMODES) != 0);

        // Read in per-channel components
        if ( HasTextures || HasMaterials || HasShaders || HasBlendModes )
        {
            UCHAR ChannelCount;
            pFile->Read( &ChannelCount, sizeof(UCHAR) );
            if (Surface->AddChannels( ChannelCount ) < 0) throw IWF_ERR_OUTOFMEMORY;

            // Read in Texture, Material & Shader Indices
            if ( HasTextures   ) pFile->Read( Surface->TextureIndices , ChannelCount * sizeof(short) );
            if ( HasMaterials  ) pFile->Read( Surface->MaterialIndices, ChannelCount * sizeof(short) );
            if ( HasShaders    ) pFile->Read( Surface->ShaderIndices  , ChannelCount * sizeof(short) );
            if ( HasBlendModes ) pFile->Read( Surface->BlendModes     , ChannelCount * sizeof(BLEND_MODE) );

        } // End if has any channel info
        
        // If the custom data area is included, then 
        // read the length and skip (unused here)
        if ( Surface->Components & SCOMPONENT_CUSTOMDATA ) 
        {
            pFile->Read( &Surface->CustomDataSize, sizeof(USHORT) );
            if ( Surface->CustomDataSize )
            {
                // Allocate the custom data area
                Surface->CustomData = new UCHAR[ Surface->CustomDataSize ];
                if ( !Surface->CustomData ) throw IWF_ERR_OUTOFMEMORY;

                // Read the data
                pFile->Read( &Surface->CustomData, Surface->CustomDataSize );

            } // End if any data
                
        } // End if has custom area

    } // Next Surface

    // Build Info to pass to child
    StepInfo.ChunkID  = Header.ChunkTypeID;
    StepInfo.Data1    = (__int64)Mesh;
    StepInfo.Data2    = 0;
    
    // Step into this chunk
    pFile->StepInto( &StepInfo );
}

//-----------------------------------------------------------------------------
// Name : ReadVertices () (Static CHUNKPROC)
// Desc : Function associated with several of the vertex component type ID's.
// Note : Does not currently support VPOOL_MESH or VPOOL_GLOBAL
//-----------------------------------------------------------------------------
void CFileIWF::ReadVertices( LPVOID pContext, CHUNKHEADER& Header, LPVOID pCustomData )
{
	PARENTINFO  StepInfo;
    UCHAR       VertexPool, TexChannelCount, TexCoordSize[255];
    CFileIWF   *pFile = (CFileIWF*)pContext;
    ULONG       f, v, SurfaceCount;
    USHORT      VertexCount;
    ULONG       Components;
    
    // Obtain the information passed from the parent
    PARENTINFO * pParentInfo = (PARENTINFO*)pCustomData;
    
    // Validate Requirements
    if (!pParentInfo) throw IWF_ERR_MALFORMED;

    // Obtain the mesh passed from the parent.
    iwfMesh    *Mesh = (iwfMesh*)pParentInfo->Data1;
    iwfSurface *pSurface = NULL;
    
    // Read the various global vars
    pFile->Read( &VertexPool, sizeof(UCHAR) );

    // Validate Requirements (Skip if anything other than VPOOL_SURFACE)
    if ( VertexPool != VPOOL_SURFACE  ) return;
    
    // Skip the 'TotalVertexCount'
    pFile->Seek( sizeof(ULONG) );

    // Read in the number of surfaces in this vertex pool
    pFile->Read( &SurfaceCount, sizeof(ULONG) );

    // Loop through reading in the vertex components provided
    for ( f = 0; f < SurfaceCount; f++ ) 
    {        
        pSurface = Mesh->Surfaces[f];
        pFile->Read( &pSurface->VertexFlags, sizeof(UCHAR)  );

        // Allocate memory for the vertices here
        pFile->Read( &VertexCount, sizeof(USHORT) );
        if (VertexCount > 0) pSurface->AddVertices( VertexCount );

        // Read vertex component information
        pFile->Read( &Components, sizeof(ULONG) );
        pSurface->VertexComponents = Components;

        // Fill out default texture coordinate layout
        TexChannelCount = 1; TexCoordSize[0] = 2;

        // Read in texture coordinate layout information if available
        if ( Components & VCOMPONENT_TEXCOORDLAYOUT ) 
        {
            pFile->Read( &TexChannelCount, sizeof(UCHAR) );
            pFile->Read( &TexCoordSize, sizeof(UCHAR) * TexChannelCount );
        
        } // End if Layout Provided
        
        // Store the layout details in the surface
        pSurface->TexChannelCount = TexChannelCount;
        if ( pSurface->TexChannelCount  )
        {
            // Allocate memory
            pSurface->TexCoordSize = new UCHAR[pSurface->TexChannelCount];
            if (!pSurface->TexCoordSize) throw IWF_ERR_OUTOFMEMORY;

            // Copy over data
            memcpy( pSurface->TexCoordSize, TexCoordSize, TexChannelCount );
        
        } // End if any texture info

        // Loop through each vertex provided
        for ( v = 0; v < VertexCount; v++ )
        {
            // Read vertex coordinates if supplied
            if ( Components & VCOMPONENT_XYZ ) 
            {
                pFile->Read( &pSurface->Vertices[v].x, sizeof(float) );
                pFile->Read( &pSurface->Vertices[v].y, sizeof(float) );
                pFile->Read( &pSurface->Vertices[v].z, sizeof(float) );
            
            } // End if coords

            if ( Components & VCOMPONENT_RHW ) pFile->Read( &pSurface->Vertices[v].rhw, sizeof(float) );

            // Read vertex normal if supplied
            if ( Components & VCOMPONENT_NORMAL ) 
            {
                pFile->Read( &pSurface->Vertices[v].Normal, sizeof(VECTOR3) );

            } // End if normal

            // Read other data
            if ( Components & VCOMPONENT_POINTSIZE ) pFile->Read( &pSurface->Vertices[v].PointSize, sizeof(float) );
            if ( Components & VCOMPONENT_DIFFUSE   ) pFile->Read( &pSurface->Vertices[v].Diffuse, sizeof(ULONG) );
            if ( Components & VCOMPONENT_SPECULAR  ) pFile->Read( &pSurface->Vertices[v].Specular, sizeof(ULONG) );

            // Read texture coordinates if supplied
            if ( Components & VCOMPONENT_TEXCOORDS && TexChannelCount > 0 ) 
            {
                int Channel = 0;

                // Setup our vertex tex channel data
                if (!pSurface->Vertices[v].AllocateTexCoords( TexChannelCount, TexCoordSize )) throw IWF_ERR_OUTOFMEMORY;
                
                // Loop through and read the coordinates
                for ( Channel = 0 ; Channel < TexChannelCount; Channel++ )
                {
                    if ( TexCoordSize[Channel] == 0 ) continue;
                    pFile->Read( pSurface->Vertices[v].TexCoords[Channel] , TexCoordSize[Channel] * sizeof(float) );

                } // Next Channel
                    
            } // End if texture coords

        } // Next Vertex

    } // Next Surface 

	// Build Info to pass to child
    StepInfo.ChunkID  = Header.ChunkTypeID;
    StepInfo.Data1    = (__int64)Mesh;
    StepInfo.Data2    = 0;
    
    // Step into this chunk
    pFile->StepInto( &StepInfo );
}

//-----------------------------------------------------------------------------
// Name : ReadIndices () (Static CHUNKPROC)
// Desc : Function associated with the CHUNK_INDICES type ID.
// Note : Does not currently support VPOOL_MESH or VPOOL_GLOBAL
//-----------------------------------------------------------------------------
void CFileIWF::ReadIndices( LPVOID pContext, CHUNKHEADER& Header, LPVOID pCustomData )
{
    UCHAR       VertexPool, IndexFlags, BytesPerIndex = 2;
    CFileIWF   *pFile = (CFileIWF*)pContext;
    PARENTINFO  StepInfo, *pParentInfo;
    ULONG       Index32, f, i;
    USHORT      Index16, IndexCount;
   
    // Obtain the information passed from the parent
    pParentInfo = (PARENTINFO*)pCustomData;
    
    // Validate Requirements
    if (!pParentInfo || !pParentInfo->Data1     ) throw IWF_ERR_MALFORMED;
    if ( pParentInfo->ChunkID != CHUNK_SURFACES ) throw IWF_ERR_MALFORMED;

    // Obtain the mesh passed from the parent.
    iwfMesh * Mesh = (iwfMesh*)pParentInfo->Data1;
    
    // Skip over TotalIndexCount variable
    pFile->Seek( sizeof(ULONG) );
    // Skip over SurfaceCount variable
    pFile->Seek( sizeof(ULONG) );

    // Read Index Information for each surface
    for ( f = 0; f < Mesh->SurfaceCount; f++ )
    {
        iwfSurface * pSurface = Mesh->Surfaces[f];
        
        // Read index flags etc.        
        pFile->Read( &IndexCount, sizeof(USHORT) );    
        pFile->Read( &IndexFlags, sizeof(UCHAR) );
        pFile->Read( &VertexPool, sizeof(UCHAR) );        

        // Determine the size of each index, skip if unsupported pool
        if ( IndexFlags & INDICES_32BIT  ) BytesPerIndex = 4;
        
        // Read in each index (Only VPOOL_SURFACE Supported Here )
        if ( VertexPool == VPOOL_SURFACE ) 
        {
            // Allocate enough room for these indices.
            if (pSurface->AddIndices( IndexCount ) < 0) throw IWF_ERR_OUTOFMEMORY;

            for ( i = 0; i < IndexCount; i++ ) 
            {
                if ( IndexFlags & INDICES_16BIT )
                {
                    pFile->Read( &Index16, sizeof(USHORT) );
                    pSurface->Indices[i] = Index16;
            
                } // End If 16 Bit
                else
                {
                    pFile->Read( &Index32, sizeof(ULONG) );
                    pSurface->Indices[i] = Index32;

                } // End If 32 Bit

            } // Next Index

            // Store Flags
            pSurface->IndexFlags = IndexFlags;
            pSurface->IndexVPool = VertexPool;

        } // End if Supported
        else
        {
            // Skip over the indices stored here
            pFile->Seek( BytesPerIndex * IndexCount );

        } // End if Not Supported

    } // Next Surface

	// Build Info to pass to child
    StepInfo.ChunkID  = Header.ChunkTypeID;
    StepInfo.Data1    = (__int64)Mesh;
    StepInfo.Data2    = 0;
    
    // Step into this chunk
    pFile->StepInto( &StepInfo );
}

//-----------------------------------------------------------------------------
// Name : ReadEntity () (Static CHUNKPROC)
// Desc : Function associated with the CHUNK_ENTITY type ID.
//-----------------------------------------------------------------------------
void CFileIWF::ReadEntity( LPVOID pContext, CHUNKHEADER& Header, LPVOID pCustomData )
{
    PARENTINFO StepInfo, * pParentInfo;
    CFileIWF   * pFile = (CFileIWF*)pContext;
    ULONG      DataSize;

    // Obtain the information passed from the parent
    pParentInfo = (PARENTINFO*)pCustomData;

    // Allocate a new entity object
    iwfEntity * Entity = new iwfEntity;
    if (!Entity) throw IWF_ERR_OUTOFMEMORY;

    // Should we retain group information ?
    if ( pFile->m_LoadFlags & IWF_RETAIN_GROUPS && pParentInfo )
    {
        if ( pParentInfo->ChunkID == CHUNK_GROUP )
        {
            // Add us to our parent
            iwfGroup * Parent = ((iwfGroup*)pParentInfo->Data1);
            if ( Parent->AddChild( 1 ) < 0 ) throw IWF_ERR_OUTOFMEMORY;
            Parent->Children[ Parent->ChildCount - 1 ] = Entity;

        } // End if adding us to a group
        else
        {
            // We are only allowed to add ourselves to another group
            throw IWF_ERR_MALFORMED;

        } // End if malformed

    } // End if Attach to parent
    else
    {
        // Add to the entity list
        pFile->m_vpEntityList.push_back( Entity );

    } // End if no parent

    // Duplicate the author ID information, allows verification of custom entities
    Entity->AuthorIDLength = Header.AuthorIDLength;
    if ( Header.AuthorIDLength ) memcpy( Entity->AuthorID, Header.AuthorID, Header.AuthorIDLength );
    
    // Read in the entity ID and Name details
    pFile->Read( &Entity->EntityTypeID, sizeof(USHORT) );
    pFile->ReadName( sizeof(UCHAR), &Entity->Name );

    // Skip past reserved data
    pFile->Seek( sizeof(ULONG) );

    // Read in the entities object matrix.
    pFile->Read( &Entity->ObjectMatrix, sizeof(MATRIX4) );

    // Read in the data area if any
    pFile->Read( &DataSize, sizeof(ULONG) );
    if (!Entity->AllocateData( DataSize )) throw IWF_ERR_OUTOFMEMORY;
    pFile->Read( Entity->DataArea, DataSize );

	// Build Info to pass to child
    StepInfo.ChunkID  = Header.ChunkTypeID;
    StepInfo.Data1    = (__int64)Entity;
    StepInfo.Data2    = 0;
    
    // Step into this chunk
    pFile->StepInto( &StepInfo );
    
}

//-----------------------------------------------------------------------------
// Name : ReadTextures () (Static CHUNKPROC)
// Desc : Function associated with the CHUNK_TEXTURES type ID.
//-----------------------------------------------------------------------------
void CFileIWF::ReadTextures( LPVOID pContext, CHUNKHEADER& Header, LPVOID pCustomData )
{
	PARENTINFO StepInfo;
    ULONG     TextureCount;
    CFileIWF *pFile = (CFileIWF*)pContext;

    // Read Texture Count
    pFile->Read( &TextureCount, sizeof(ULONG) );
    
    // Read in each texture
    for ( ULONG i = 0; i < TextureCount; i++ )
    {
        iwfTexture * Texture = new iwfTexture;
        if (!Texture) throw IWF_ERR_OUTOFMEMORY;

        // Add to the texture list
        pFile->m_vpTextureList.push_back( Texture );
        
        // Read in the physical source of the texture (external or internal)
        pFile->Read( &Texture->TextureSource, sizeof(UCHAR) );
        
        // Read in the reference name, or filename
        pFile->ReadName( sizeof(USHORT), &Texture->Name  );
        
        // Read in internally stored texture information
        pFile->Read( &Texture->TextureFormat, sizeof(UCHAR) );
        pFile->Read( &Texture->TextureSize, sizeof(ULONG) );
        if ( Texture->TextureSize > 0 )
        {
            Texture->AllocateData( Texture->TextureSize );
            pFile->Read( Texture->TextureData, Texture->TextureSize );

        } // End if 

    } // Next Texture

	// Build Info to pass to child
    StepInfo.ChunkID  = Header.ChunkTypeID;
    StepInfo.Data1    = 0;
    StepInfo.Data2    = 0;
    
    // Step into this chunk
    pFile->StepInto( &StepInfo );
}

//-----------------------------------------------------------------------------
// Name : ReadMaterials () (Static CHUNKPROC)
// Desc : Function associated with the CHUNK_MATERIALS type ID.
//-----------------------------------------------------------------------------
void CFileIWF::ReadMaterials( LPVOID pContext, CHUNKHEADER& Header, LPVOID pCustomData )
{
	PARENTINFO StepInfo;
    ULONG     MaterialCount;
    CFileIWF *pFile = (CFileIWF*)pContext;

    // Read Material Count
    pFile->Read( &MaterialCount, sizeof(ULONG) );

    // Read in each material
    for ( ULONG i = 0; i < MaterialCount; i++ ) 
    {
        iwfMaterial * Material = new iwfMaterial;

        // Add to the Material list
        pFile->m_vpMaterialList.push_back( Material );

        // Read in the reference name
        pFile->ReadName( sizeof(UCHAR), &Material->Name  );
        
        // Retrieve the material properties
        pFile->Read( &Material->Diffuse , sizeof(COLOUR_VALUE) );
        pFile->Read( &Material->Ambient , sizeof(COLOUR_VALUE) );
        pFile->Read( &Material->Emissive, sizeof(COLOUR_VALUE) );
        pFile->Read( &Material->Specular, sizeof(COLOUR_VALUE) );
        pFile->Read( &Material->Power   , sizeof(float) );

    } // Next Material

	// Build Info to pass to child
    StepInfo.ChunkID  = Header.ChunkTypeID;
    StepInfo.Data1    = 0;
    StepInfo.Data2    = 0;
    
    // Step into this chunk
    pFile->StepInto( &StepInfo );
}

//-----------------------------------------------------------------------------
// Name : ReadShaders () (Static CHUNKPROC)
// Desc : Function associated with the CHUNK_SHADERS type ID.
//-----------------------------------------------------------------------------
void CFileIWF::ReadShaders( LPVOID pContext, CHUNKHEADER& Header, LPVOID pCustomData )
{
	PARENTINFO StepInfo;
    ULONG     ShaderCount;
    CFileIWF *pFile = (CFileIWF*)pContext;

    // Read Shader Count
    pFile->Read( &ShaderCount, sizeof(ULONG) );
    
    // Read in each shader
    for ( ULONG i = 0; i < ShaderCount; i++ )
    {
        iwfShader * Shader = new iwfShader;
        if (!Shader) throw IWF_ERR_OUTOFMEMORY;

        // Add to the shader list
        pFile->m_vpShaderList.push_back( Shader );
        
        // Read in component data
        pFile->Read( &Shader->Components, sizeof(ULONG) );

        // Read in each of the script refs for the two shader types
        if ( Shader->Components & SHCOMPONENT_VSHADER ) pFile->ReadScriptRef( &Shader->VertexShader );
        if ( Shader->Components & SHCOMPONENT_PSHADER ) pFile->ReadScriptRef( &Shader->PixelShader );

    } // Next Shader

	// Build Info to pass to child
    StepInfo.ChunkID  = Header.ChunkTypeID;
    StepInfo.Data1    = 0;
    StepInfo.Data2    = 0;
    
    // Step into this chunk
    pFile->StepInto( &StepInfo );
}

//-----------------------------------------------------------------------------
// Name : ReadScripts () (Static CHUNKPROC)
// Desc : Function associated with the CHUNK_SCRIPTS type ID.
//-----------------------------------------------------------------------------
void CFileIWF::ReadScripts( LPVOID pContext, CHUNKHEADER& Header, LPVOID pCustomData )
{
	PARENTINFO StepInfo;
    USHORT    ScriptCount;
    CFileIWF *pFile = (CFileIWF*)pContext;

    // Read Shader Count
    pFile->Read( &ScriptCount, sizeof(USHORT) );

    // Read in each script
    for ( USHORT i = 0; i < ScriptCount; i++ )
    {
        iwfScript * Script = new iwfScript;
        if (!Script) throw IWF_ERR_OUTOFMEMORY;

        // Read in the script name
        pFile->ReadName( sizeof(UCHAR), &Script->Name );

        // Add to the script list
        pFile->m_vpScriptList.push_back( Script );

        // Read in the script reference
        pFile->ReadScriptRef( &Script->Script );

		// Skip over reserved variable
		pFile->Seek( sizeof(ULONG) );

    } // Next Script

	// Build Info to pass to child
    StepInfo.ChunkID  = Header.ChunkTypeID;
    StepInfo.Data1    = 0;
    StepInfo.Data2    = 0;
    
    // Step into this chunk
    pFile->StepInto( &StepInfo );
}