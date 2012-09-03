### @file	ml-Blender/tamy_scene.py
### @brief	structures that represent Tamy scene data
### ===========================================================================

from ctypes import *
from . import tamy_mesh
from . import tamy_material

### ===========================================================================

### A helper structure that represents an exportable matrix	
class TamyMatrix( Structure ):
	_fields_= [ ( "m", c_float * 16 ) ]
	
	def __init__( self, blenderMtx ):
		sideVec, upVec, frontVec, transVec = blenderMtx[:]
		self.m[0] = sideVec[0]
		self.m[1] = sideVec[1]
		self.m[2] = sideVec[2]
		self.m[3] = sideVec[3]
		self.m[4] = upVec[0]
		self.m[5] = upVec[1]
		self.m[6] = upVec[2]
		self.m[7] = upVec[3]
		self.m[8] = frontVec[0]
		self.m[9] = frontVec[1]
		self.m[10] = frontVec[2]
		self.m[11] = frontVec[3]
		self.m[12] = transVec[0]
		self.m[13] = transVec[1]
		self.m[14] = transVec[2]
		self.m[15] = transVec[3]
		
### ===========================================================================

#### A class that represents a SpatialEntity
class TamyEntity( Structure ):
	
	_fields_= [	( "name", c_char_p ),
				( "meshesList", POINTER( tamy_mesh.TamyMesh ) ),
				( "localMatrix", TamyMatrix ),
				( "meshesCount", c_int ) ]
				
	def __init__( self, meshes, matrix, name ):
	
		self.name = name.encode( "utf-8" )
		self.localMatrix = TamyMatrix( matrix )
		
		self.meshesCount = len(meshes)
		self.meshesList = ( tamy_mesh.TamyMesh * self.meshesCount )()
		i = 0
		for mesh in meshes:
			self.meshesList[i] = mesh
			i += 1
		
### ===========================================================================

#### A class that represents a Model
class TamyScene( Structure ):
	
	_fields_= [	( "name", c_char_p ),
				( "entities", POINTER( TamyEntity ) ),
				( "entitiesCount", c_int ) ]
				
	def __init__( self, tamyEntities, name ):
	
		self.name = name.encode( "utf-8" )
		
		self.entitiesCount = len(tamyEntities)
		self.entities = ( TamyEntity * self.entitiesCount )()
		i = 0
		for tamyEntity in tamyEntities:
			self.entities[i] = tamyEntity
			i += 1
		
### ===========================================================================

### Creates a dictionary of all materials  used in the scene and the mesh objects that use them
def get_materials_and_entities( context, bUseSelection, globalMatrix, sceneName, outArrTextures, outArrMaterials ):

	scene = context.scene

	if bUseSelection:
		objects = (ob for ob in scene.objects if ob.is_visible(scene) and ob.select)
	else:
		objects = (ob for ob in scene.objects if ob.is_visible(scene))
	
	from bpy_extras.io_utils import create_derived_objects, free_derived_objects
	
	materialDict = {}
	meshObjects = []
	for ob in objects:
		
		# get derived objects
		free, derived = create_derived_objects(scene, ob)

		if derived is None:
			continue

		for obDerived, mat in derived:
			if ob.type not in {'MESH', 'CURVE', 'SURFACE', 'FONT', 'META'}:
				continue
			
			try:
				derivedBlenderMesh = obDerived.to_mesh( scene, True, 'PREVIEW' )
			except:
				derivedBlenderMesh = None
				
			if derivedBlenderMesh:
				matrix = globalMatrix * mat
				derivedBlenderMesh.transform(matrix)
				meshObjects.append( ( obDerived, derivedBlenderMesh, matrix ) )
				matLs = derivedBlenderMesh.materials
				matLsLen = len(matLs)

				# get material/image tuples.
				if derivedBlenderMesh.tessface_uv_textures:
					if not matLs:
						mat = matName = None

					for f, uf in zip( derivedBlenderMesh.tessfaces, derivedBlenderMesh.tessface_uv_textures.active.data ):
						if matLs:
							matIndex = f.material_index
							if matIndex >= matLsLen:
								matIndex = f.mat = 0
							mat = matLs[matIndex]
							matName = None if mat is None else mat.name
						# else the name is already set to 'none'

						img = uf.image
						imgName = None if img is None else img.name

						materialDict.setdefault( ( matName, imgName ), ( mat, img ) )

				else:
					for mat in matLs:
						if mat:  # material may be None so check its not.
							materialDict.setdefault( ( mat.name, None ), ( mat, None ) )

					# Why 0 Why!
					for f in derivedBlenderMesh.tessfaces:
						if f.material_index >= matLsLen:
							f.material_index = 0

			if free:
				free_derived_objects(ob)
	
	# Create the materials
	texturesDict = {}
	for matAndImage in materialDict.values():
		outArrMaterials.append( tamy_material.TamyMaterial( matAndImage[0], matAndImage[1], texturesDict ) )
		
	# Create texture entires
	outArrTextures = texturesDict.keys()
	
	# Create the entities
	entities = []
	for ob, blenderMesh, matrix in meshObjects:
	
		meshes = []
		tamy_mesh.create_tamy_meshes( blenderMesh, meshes )		
		entities.append( TamyEntity( meshes, matrix, ob.name ) )
		
	tamyScene = TamyScene( entities, sceneName )
	return tamyScene
		
### ===========================================================================
