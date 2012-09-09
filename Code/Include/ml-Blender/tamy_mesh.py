### @file	ml-Blender/tamy_mesh.py
### @brief	structures that represent Tamy mesh data
### ===========================================================================

from ctypes import *
from array import *
import bpy
import copy

### ===========================================================================

### A helper function representing a singe mesh vertex
class TamyVertex( Structure ):

	_fields_ = [("x", c_float),
				("y", c_float),
				("z", c_float),
				("nx", c_float),
				("ny", c_float),
				("nz", c_float),
				("tanx", c_float),
				("tany", c_float),
				("tanz", c_float),
				("u", c_float),
				("v", c_float) ]
	
	def __init__( self, coords, normal, uv=(0,0) ):
	
		self.x, self.y, self.z = coords[:]
		self.nx, self.ny, self.nz = normal[:]
		self.tanx, self.tany, self.tanz, = (0, 0, 0)
		self.u, self.v =  uv[:]
		
	def __init__( self, blenderVertex ):
	
		self.x, self.y, self.z = blenderVertex.co[:]
		self.nx, self.ny, self.nz = blenderVertex.normal[:]
		self.tanx, self.tany, self.tanz = ( 0, 0, 0 )
		self.u, self.v =  ( 0, 0 )
		
	def __copy__( self ):
		result = TamyVertex( ( self.x, self.y, self.z ), ( self.nx, self.ny, self.nz ), ( self.u, self.v ) )
		return result
		
	def setNormal( self, normalTuple ):
		self.nx, self.ny, self.nz = normalTuple[:]
		
	def setUV( self, u, v ):
		self.u = u
		self.v = v
		
### ===========================================================================

### A helper structure representing a single exportable mesh face		
class TamyFace( Structure ):

	_fields_ = [( "idx", c_short * 3)]
	
	def __init__( self, indices ):
		self.idx[0], self.idx[1], self.idx[2] = indices[:]
		
	### Compares two faces
	def compare( self, otherFace ):
		if ( self.idx[0] != otherFace.idx[0] ):
			return False
			
		if ( self.idx[1] == otherFace.idx[1] ):
			return False
			
		if ( self.idx[2] == otherFace.idx[2] ):
			return False
		
		return True	
		
### ===========================================================================

### A class that represents a mesh
class TamyMesh( Structure ):

	_fields_= [	( "name", c_char_p ),
				( "verticesList", POINTER( TamyVertex ) ),
				( "facesList", POINTER( TamyFace ) ),
				( "materialsList", POINTER( c_int ) ),
				( "facesCount", c_int ),
				( "verticesCount", c_int ),
				( "materialsCount", c_int ) ]
				
	# verticesList : TamyVertex[]
	# facesList : TamyFace[]	- array of triples
	
	def __init__( self, name ):
		self.name = name.encode( "utf-8" )
		self.facesCount = 0
		self.verticesCount = 0
		self.materialCount = 0
	
	# -------------------------------------------------------------------------	
	
	### Sets the faces of the mesh
	def setFaces( self, faces ):
		
		self.facesCount = len( faces )
		self.facesList = ( TamyFace * self.facesCount )()
		
		i = 0
		for face in faces:
			self.facesList[i] = face
			i += 1		
		
	# -------------------------------------------------------------------------
		
	### Compares two meshes in terms of the faces they use. If they have exactly same faces, the comparison result is positive
	def compareFaces( self, otherMesh ):
		
		# an early out - if the number of faces in the two meshes don't match, they're not equal
		if ( self.facesCount != otherMesh.facesCount ):
			return False
		
		for i in range( self.facesCount ):
			if self.facesList[i].compare( otherMesh.facesList[i] ) == False:
				# the meshes are not identical
				return False
				
		# the meshes are identical
		return True
		
	# -------------------------------------------------------------------------
		
	### Adds a new material to the mesh
	def addMaterial( self, materialIdx ):
		newMaterialsList = ( c_int * ( self.materialsCount + 1 ) )()
		
		for i in range( self.materialsCount ):
			newMaterialsList[i] = self.materialsList[i]
		
		self.materialsList = newMaterialsList
		self.materialsList[ self.materialsCount ] = materialIdx
		self.materialsCount += 1

	# -------------------------------------------------------------------------
		
	### Merges materials from both meshes, storing them in this mesh instance
	def mergeMaterials( self, otherMesh ):
		newSize = self.materialsCount + otherMesh.materialsCount
		newMaterialsList = ( c_int * newSize )()
		
		for i in range( self.materialsCount ):
			newMaterialsList[i] = self.materialsList[i]
		
		for i in range( self.materialsCount, newSize ):
			newMaterialsList[i] = otherMesh.materialsList[i - self.materialsCount]
			
		self.materialsList = newMaterialsList
		self.materialsCount = newSize
	
	# -------------------------------------------------------------------------
	
	### Maps material indices to the ones from the specified materials dictionary
	###
	### It's required that the `materialsExportOrderDict` should contain pairs ( Material : index ),
	### where the `index` stands for the ordinal number with which the material will be exported
	def mapMaterials( self, originalMeshMaterials, materialsExportOrderDict ):
		for i in range( self.materialsCount ):
			blenderMaterial = originalMeshMaterials[self.materialsList[i]]
			mappedMaterialIdx = materialsExportOrderDict[blenderMaterial]
			self.materialsList[i] = mappedMaterialIdx
			
	# -------------------------------------------------------------------------
		
	### Sets mesh vertices nad adjusts face indices to index into the local array
	def setVertices( self, vertices ):
		
		tmpVerticesList = []
		indicesMap = []
		
		for i in range( len( vertices ) ):
			indicesMap.append( -1 )
		
		# map the vertices
		for i in range( self.facesCount ):
			face = self.facesList[i]
			
			for j in range( 3 ):
				vertexIdx = face.idx[j]
				
				if ( indicesMap[ vertexIdx ] < 0 ):
					# map only the unmapped index
					remappedVtxIdx = len( tmpVerticesList )
					indicesMap[ vertexIdx ] = remappedVtxIdx
					tmpVerticesList.append( vertices[ vertexIdx ] )
				
		# copy the mapped vertices to our local structure
		self.verticesCount = len( tmpVerticesList )
		self.verticesList = ( TamyVertex * self.verticesCount )()
		i = 0
		for vtx in tmpVerticesList:
			self.verticesList[i] = vtx
			i += 1
			
		# remap face indices
		for i in range( self.facesCount ):
			face = self.facesList[i]
			for j in range( 3 ):
				face.idx[j] = indicesMap[ face.idx[j] ]
				
		
### ===========================================================================

### Since Blender meshes are not compatible with Tamy meshes in terms of
### the number of vertices per face they may have, and the number of materials per face
### they may use ( Tamy assumes a mesh uses the same number of materials to render all of its faces ),
### we need to split them into sub meshes before we can export them to Tamy.
###
### This method accomplishes that, it splits the meshes and appends them on the export list.
###
### It's required that the `materialsExportOrderDict` should contain pairs ( Material : index ),
### where the `index` stands for the ordinal number with which the material will be exported
def create_tamy_meshes( meshName, blenderMesh, materialsExportOrderDict, outMeshes ):
			
	# create a copy of all vertices and face indices - there's a possibility we'll be doing some splitting etc
	# and we might introduce new vertices as we go along
	vertices = []
	for blenderVtx in blenderMesh.vertices:
		vertices.append( TamyVertex( blenderVtx ) )
		
	faces = []	
	for poly in blenderMesh.polygons:
		faces.append( poly.vertices )
	
	# create a map of UV coordinates assigned to each vertex
	vertices, faces = calculate_unique_uv_coords( blenderMesh, vertices, faces )
	
	# split the non-triangular faces
	materialPerFace = []
	faces = split_polygons_into_triangles( blenderMesh.polygons, faces, materialPerFace, vertices )
	
	# now - create Tamy meshes, each dedicated to a single material
	singleMaterialMeshes = []
	materialsSet = set( materialPerFace )
	for checkedMatIdx in materialsSet:
	
		newMeshName = "%s_%d" % ( meshName, len( singleMaterialMeshes ) )
		newMesh = TamyMesh( newMeshName )
		
		singleMaterialMeshes.append( newMesh )
		newMesh.addMaterial( checkedMatIdx )
		
		faceIdx = 0
		meshFaces = []
		for faceMatIdx in materialPerFace:
			if ( faceMatIdx == checkedMatIdx ):
				meshFaces.append( faces[faceIdx] )
			faceIdx += 1	
			
		newMesh.setFaces( meshFaces )
	
	# and now merge the meshes that reference exactly the same faces
	meshesCount = len( singleMaterialMeshes )
	for i in range( meshesCount ):
	
		if ( singleMaterialMeshes[i] is None ):
			continue
			
		for j in range( len( singleMaterialMeshes ) ):
			if ( i == j ):
				continue;
				
			if ( singleMaterialMeshes[j] is None ):
				continue
			
		if ( singleMaterialMeshes[i].compareFaces( singleMaterialMeshes[j] ) ):
			# the meshes have exactly the same faces, so merge them together
			singleMaterialMeshes[i].mergeMaterials( singleMaterialMeshes[j] )
			
			# reset that material instance, so that we don't merge it twice
			singleMaterialMeshes[j] = None		


	# set the meshes in the output array and initialize them with vertices
	for i in range( meshesCount ):
	
		mergedMesh = singleMaterialMeshes[i]
		
		if ( mergedMesh is not None ):
			
			outMeshes.append( mergedMesh )
			mergedMesh.setVertices( vertices )
			
	# map the materials indices to the ones from our materials dictionary
	mergedMesh.mapMaterials( blenderMesh.materials, materialsExportOrderDict )			

### ===========================================================================

def uv_key( u, v ):
    return round(u, 6), round(-v, 6)
	
### The method splits vertices so that each has only a single UV coordinate assigned.
def calculate_unique_uv_coords( blenderMesh, vertices, faces ):

	if bool( blenderMesh.tessface_uv_textures ) != True:
		# No uv's present
		return vertices, faces
	
	splitVertices = []

    # for each face uv coordinate, add it to the UniqueList of the vertex
	uvsDict = [ [] for i in range( len( vertices ) ) ]
	polyIdx = 0
	for poly in blenderMesh.polygons:
	
		polyUVs = blenderMesh.tessface_uv_textures.active.data[polyIdx]
		
		uvOffset = 0
		for vtxIdx in poly.vertices:		
			uvCoord = uv_key( polyUVs.uv[uvOffset][0], polyUVs.uv[uvOffset][1] )
			
			arrUVs = uvsDict[vtxIdx]
			arrUVs.append( uvCoord )
			
			uvOffset += 1
				
		polyIdx += 1
		
	# change them all into sets to eliminate duplicate uvs
	outUVCoordsToIndicesMap = [ [] for i in range( len( vertices ) ) ]
	for vtxIdx in range( len( uvsDict ) ):
		uvsDict[vtxIdx] = set( uvsDict[vtxIdx] )

		for uvCoords in uvsDict[vtxIdx]:
			originalVertex = vertices[vtxIdx]
			
			outUVCoordsToIndicesMap[vtxIdx].append( ( uvCoords, len( splitVertices ) ) )
			
			copiedVertex = copy.deepcopy( originalVertex )
			copiedVertex.setUV( uvCoords[0], uvCoords[1] )
			splitVertices.append( copiedVertex )
			
		
	# split the polygons and duplicate the vertices so that each vertex has
	# only one set of UV coordinates assigned
	polyIdx = 0
	newFaces = []
	for poly in blenderMesh.polygons:	
	
		polyUVs = blenderMesh.tessface_uv_textures.active.data[polyIdx]
		faceVtxIndices = []
		for i in range( len( poly.vertices ) ):
		
			uvCoord = uv_key( polyUVs.uv[i][0], polyUVs.uv[i][1] )	
			vtxIdx = poly.vertices[i]
			
			for vtxUvCoords, newVtxIdx in outUVCoordsToIndicesMap[vtxIdx]:
				if vtxUvCoords == uvCoord:
					faceVtxIndices.append( newVtxIdx )
					break
			
		newFaces.append( faceVtxIndices )
		polyIdx += 1
		
	return splitVertices, newFaces
			
### ===========================================================================
	
### A helper method that splits non-triangular polygons into faces ( by rearranging their indices of course ;)
def split_polygons_into_triangles( polygons, faces, materialPerFace, vertices ):
	
	tamyFaces = []
		
	faceIdx = 0
	for faceIdx in range( len( polygons ) ):
		poly = polygons[faceIdx]
		face = faces[faceIdx]
		indicesCount = len( face )
		
		# First check to see if it's supposed to be smoothed or not
		# Smoothed polygons share vertices with other polygons, while hard polygons don't,
		# and we need to duplicate their vertices
		
		indices = array( 'i', face[:] )
		
		# 1. Duplicate the vertices as many times as necessary, so that each 
		#    vertex has only one set of UV coordinates assigned, and wears
		#    a unique normal if the face it belongs to is supposed to be rendered flat
		if ( poly.use_smooth == False ):	
			for i in range( len( indices ) ):				
				# duplicate the vertices
				originalVertex = vertices[indices[i]]
				duplicatedVertex = copy.deepcopy( originalVertex )
				duplicatedVertex.setNormal( poly.normal )
				
				indices[i] = len( vertices )
				vertices.append( duplicatedVertex )
		
		# 2. triangulate the polygon if needed and flip the winding order of the triangles
		# at the same time ( because we're switching coordinate system handedness here )
		if ( indicesCount == 3 ):
			# it's a regular face
			tamyFaces.append( TamyFace( [ indices[0], indices[2], indices[1] ] ) )
			materialPerFace.append( poly.material_index )
				
		elif ( indicesCount == 4 ):
			# it's a quad
			tamyFaces.append( TamyFace( [ indices[0], indices[2], indices[1] ] ) )
			tamyFaces.append( TamyFace( [ indices[0], indices[3], indices[2] ] ) )
			materialPerFace.append( poly.material_index )
			materialPerFace.append( poly.material_index )

		else:
			print( "\nWARNING Unsupported polygon with more than 4 vertices encountered" )
			
	return tamyFaces
			
### ===========================================================================
