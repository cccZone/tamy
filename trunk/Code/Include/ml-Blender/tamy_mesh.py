### @file	ml-Blender/tamy_mesh.py
### @brief	structures that represent Tamy mesh data
### ===========================================================================

from ctypes import *
from array import *

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
			self.verticesList[i] = TamyVertex( vtx.co, vtx.normal )
			i += 1
			
		# remap face indices
		for i in range( self.facesCount ):
			face = self.facesList[i]
			
			for j in range( 3 ):
				face.idx[j] = indicesMap[ face.idx[j] ]
				
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
		
### ===========================================================================

### Since Blender meshes are not compatible with Tamy meshes in terms of
### the number of vertices per face they may have, and the number of materials per face
### they may use ( Tamy assumes a mesh uses the same number of materials to render all of its faces ),
### we need to split them into sub meshes before we can export them to Tamy.
###
### This method accomplishes that, it splits the meshes and appends them on the export list
def create_tamy_meshes( meshName, blenderMesh, outMeshes ):
			
	# create a copy of all vertices - there's a possibility we'll be doing some splitting etc
	# and we might introduce new vertices as we go along
	vertices = blenderMesh.vertices[:]
	
	# first, split the non-triangular faces
	faces = []
	materialPerFace = []
	split_polygons_into_triangles( blenderMesh.polygons, faces, materialPerFace, vertices )
	
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
			
	
### ===========================================================================
	
### A helper method that splits non-triangular polygons into faces ( by rearranging their indices of course ;)
def split_polygons_into_triangles( polygons, faces, materialPerFace, vertices ):

	for poly in polygons:
		indicesCount = len( poly.vertices )
		
		# First check to see if it's supposed to be smoothed or not
		# Smoothed polygons share vertices with other polygons, while hard polygons don't,
		# and we need to duplicate their vertices
		
		indices = array( 'i', poly.vertices[:] )
		if ( poly.use_smooth == False ):		
			for i in range( len( indices ) ):				
				vtx = vertices[indices[i]]
				indices[i] = len( vertices )
				vertices.append( vtx )
		
		# split the polygon if needed
		if ( indicesCount == 3 ):
			# it's a regular face
			faces.append( TamyFace( indices ) )
			materialPerFace.append( poly.material_index )
		elif ( indicesCount == 4 ):
			# it's a quad
			faces.append( TamyFace( [ indices[0], indices[1], indices[2] ] ) )
			faces.append( TamyFace( [ indices[0], indices[2], indices[3] ] ) )
			materialPerFace.append( poly.material_index )
			materialPerFace.append( poly.material_index )
		else:
			print( "\nWARNING Unsupported polygon with more than 4 vertices encountered" )
			
### ===========================================================================
