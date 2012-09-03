import bpy
from . import tamy_material
from . import tamy_mesh
from . import tamy_scene
from ctypes import *


### ===========================================================================

### This method initializes the export procedure, making sure all relevant input data has correct values
### and that the editor is in the correct state
def initialize_exporter( globalMatrix ):

	if globalMatrix is None:
		globalMatrix = mathutils.Matrix()

	if bpy.ops.object.mode_set.poll():
		bpy.ops.object.mode_set(mode='OBJECT')


### ===========================================================================
		
### This method performs the actual scene export        
def proxy_scene_export_to_tamy( filesystemRoot, exportDir, textures, materials, scene ):

	import os
	tamyExportModule = cdll.LoadLibrary( "%s/addons/io_export_tamy/ml-Blender.dll" % os.environ['BLENDER_SCRIPTS'] )
	
	if tamyExportModule is None:
		print( "\nERROR: ml-Blender.dll not found" )
		return
	
	
	tamyExportModule.begin_export( filesystemRoot.encode( "utf-8" ), exportDir.encode( "utf-8" ) )
	
	# 1. export the textures
	texturesCount = len( textures )
	exportTexturesArr = ( tamy_material.TamyTexture * texturesCount )()
	for i in range( texturesCount ):
		exportTexturesArr[i] = textures[i]
		
	tamyExportModule.export_textures( exportTexturesArr, texturesCount )
	
	
	# 2. export the materials
	materialsCount = len( materials )
	exportMaterialsArr = ( tamy_material.TamyMaterial * materialsCount )()
	for i in range( materialsCount ):
		exportMaterialsArr[i] = materials[i]
		
	tamyExportModule.export_materials( exportMaterialsArr, materialsCount )

	
	# 3. export the scene
	tamyExportModule.export_scene( scene )
	
	
	tamyExportModule.end_export()
		
### ===========================================================================

def export_scene(operator, context, filesystemRoot="", filepath="", bUseSelection=True, globalMatrix=None, ):

	# measure the execution time - just in case
	import time
	time1 = time.clock()
	
	# initialize export
	initialize_exporter( globalMatrix )
	
	# dismantle the specified filepath into the filesystem path and the name of the saved scene
	import os
	exportDir, sceneName = os.path.split( filepath )
	
	# make a list of all materials and entities in the scene
	textures = []
	materials = []
	scene = tamy_scene.get_materials_and_entities( context, bUseSelection, globalMatrix, sceneName, textures, materials )
	
	# and now export it all
	proxy_scene_export_to_tamy( filesystemRoot, exportDir, textures, materials, scene )
	
	# and the execution took...
	print("Tamy export time: %.4f" % (time.clock() - time1))
	
	return {'FINISHED'}

### ===========================================================================
