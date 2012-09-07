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

def export_scene(operator, context, filesystemRoot="", filepath="", bUseSelection=True, globalMatrix=None ):

	# measure the execution time - just in case
	import time
	time1 = time.clock()
	
	# initialize export
	initialize_exporter( globalMatrix )
	
	# dismantle the specified filepath into the filesystem path and the name of the saved scene
	import os
	exportDir, sceneName = os.path.split( filepath )
	
	# make a list of all materials and entities in the scene
	tamyScene = tamy_scene.TamyScene( sceneName )
	tamy_scene.compile_scene( context, bUseSelection, globalMatrix, tamyScene )
	
	# and now export it all
	tamyScene.export( filesystemRoot, exportDir )
	
	# and the execution took...
	print("Tamy export time: %.4f" % (time.clock() - time1))
	
	return {'FINISHED'}

### ===========================================================================
