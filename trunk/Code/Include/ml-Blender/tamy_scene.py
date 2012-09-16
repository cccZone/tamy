### @file	ml-Blender/tamy_scene.py
### @brief	structures that represent Tamy scene data
### ===========================================================================

from ctypes import *
from . import tamy_mesh
from . import tamy_material
from . import tamy_entities
from . import tamy_misc

	
### ===========================================================================

#### A class that represents a Model
class TamyScene:
	
	def __init__( self, sceneName ):
		self.sceneName = sceneName
		self.textures = []
		self.entities = []
		self.materials = []
		
	def export( self, filesystemRoot, exportDir ):
		
		import os
		tamyExportModule = cdll.LoadLibrary( "%s/addons/io_export_tamy/ml-Blender.dll" % os.environ['BLENDER_SCRIPTS'] )
		
		if tamyExportModule is None:
			print( "\nERROR: ml-Blender.dll not found" )
			return
			
		tamyExportModule.begin_export( filesystemRoot.encode( "utf-8" ), exportDir.encode( "utf-8" ), len( self.entities ) )
		
		# 1. export the textures
		texturesCount = len( self.textures )
		exportTexturesArr = ( tamy_material.TamyTexture * texturesCount )()
		for i in range( texturesCount ):
			exportTexturesArr[i] = self.textures[i]
			
		tamyExportModule.export_textures( exportTexturesArr, texturesCount )
		
		
		# 2. export the materials
		materialsCount = len( self.materials )
		exportMaterialsArr = ( tamy_material.TamyMaterial * materialsCount )()
		for i in range( materialsCount ):
			exportMaterialsArr[i] = self.materials[i]
		
		tamyExportModule.export_materials( exportMaterialsArr, materialsCount )

		# 3. export individual entities.
		#
		# CAUTION: Export order is crucial here. The C counterpart also knows that and
		# it will place the exported entities in the exact order in which there are exported.
		# But in order for the scene to be assembled correctly later on, the order needs to be 
		# preserved, since individual entities reference themselves not by pointers ( since we can't
		# really do that when exporting to C ), but by their indices in the entities[] array.
		for entityId, blenderObj, localMtx, tamyEntity in self.entities:
			
			if entityId == 'MESH':
				tamyExportModule.export_geometry_entity( tamyEntity )
				
			elif entityId == 'LIGHT':
				tamyExportModule.export_light_entity( tamyEntity )
			
		# 4. export world settings
		tamyExportModule.export_world_settings( self.worldSettings ) 
		
		# 5. assemble the scene
		tamyExportModule.assemble_scene( self.sceneName.encode( "utf-8" ) )
		
### ===========================================================================

### Builds the entities present in the scene and compiles the final scene that will be exported to blender
def compile_scene( context, bUseSelection, tamyScene ):

	scene = context.scene

	if bUseSelection:
		objects = (ob for ob in scene.objects if ob.is_visible(scene) and ob.select)
	else:
		objects = (ob for ob in scene.objects if ob.is_visible(scene))
		
	materialDict = {} # pairs of ( BlenderMaterial : exportOrdinalIdx )
	entitiesDict = {}  # pairs of ( blenderObject, : exportOrdinalIndex )
	texturesDict = {}
	build_entities_and_materials( scene, objects, tamyScene.materials, tamyScene.entities, materialDict, texturesDict, entitiesDict )
	
	# Create texture entires
	tamyScene.textures = [ None for i in range( len( texturesDict ) ) ]
	for texturePath in texturesDict.keys():
		textureIdx = texturesDict[texturePath]
		tamyScene.textures[textureIdx] = tamy_material.TamyTexture( texturePath )
		
	# Set the parenting hierarchy
	define_hierarchy( tamyScene.entities, entitiesDict )
	
	# Create the world
	tamyScene.worldSettings = tamy_entities.TamyWorld( scene.world )
		
### ===========================================================================

def build_entities_and_materials( scene, objects, outMaterials, outEntities, outMaterialsDict, outTexturesDict, outEntitiesDict ):
	
	from bpy_extras.io_utils import create_derived_objects, free_derived_objects
		
	for ob in objects:	
	
		# get derived objects
		free, derived = create_derived_objects(scene, ob)

		if derived is None:
			continue

		for obDerived, objectMtx in derived:

			tamyEntity = None
			tamyEntityType = ''
			
			# The object represents a mesh
			if ob.type in {'MESH', 'CURVE', 'SURFACE', 'FONT', 'META'}:
				try:
					derivedBlenderMesh = obDerived.to_mesh( scene, True, 'PREVIEW' )
				except:
					derivedBlenderMesh = None
					
				if derivedBlenderMesh:
					build_materials( derivedBlenderMesh, outMaterials, outMaterialsDict, outTexturesDict )
					
					meshes = []
					meshName = "%s" % ob.name
					tamy_mesh.create_tamy_meshes( meshName, derivedBlenderMesh, outMaterialsDict, meshes )
					tamyEntity = tamy_entities.TamyGeometry( meshes, ob.name )
					tamyEntityType = 'MESH'
					
					# cleanup
					if free:
						free_derived_objects( derivedBlenderMesh )
			
			# The object represents a light				
			elif ob.type == 'LAMP':
				tamyEntityType = 'LIGHT'
				tamyEntity = tamy_entities.TamyLight( ob.name, ob.data )
				
			
			# add new entity to our list
			if tamyEntity is not None:
				entityIdx = len( outEntities )
				outEntitiesDict[ob] = entityIdx					
				outEntities.append( ( tamyEntityType, obDerived, objectMtx, tamyEntity ) )
					

### ===========================================================================	

def build_materials( derivedBlenderMesh, outMaterials, outMaterialsDict, outTexturesDict ):
		
	for material in derivedBlenderMesh.materials:
		if material is not None:
			outMaterialsDict[material] = len(outMaterials)
			outMaterials.append( tamy_material.TamyMaterial( material, outTexturesDict ) )

### ===========================================================================

import mathutils

### @param entities - it's a tuple: ( ELEM_ID, blenderObject, objectLocalMatrix, TamyEntity )
def define_hierarchy( entities, entitiesDict ):
	
	for i in range( len( entities ) ):
		blenderObject = entities[i][1]
		obMatrix = entities[i][2]
		tamyEntity = entities[i][3]
		
		tamyEntity.set_matrix( obMatrix )
		
		if ( blenderObject.parent_type != 'OBJECT' and blenderObject.parent_type !='BONE' and blenderObject.parent_type !='LATTICE' ):
			# Object can't be a part of any hierearchy, but nonetheless set its local matrix
			continue
		
		if blenderObject.parent is None:
			# it's the root of a hierarchy - therefore its local matrix should be multipliend by the desired global
			# matrix to convert its coordinate system to Tamy
			continue
		
		# set the entity's parent index
		parentIdx = entitiesDict[ blenderObject.parent ]
		tamyEntity.set_parent( parentIdx )
		
### ===========================================================================
