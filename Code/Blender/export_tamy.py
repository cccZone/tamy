# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

__author__ = ["Piotr Trochim"]
__version__ = '0.1'
__bpydoc__ = """\
This script exports a Mesh to Tamy engine using a simple raw data format.

Usage:<br>
    Execute this script from the "File->Export" menu.

"""

import bpy
from bpy.props import *

import xml
from xml import *
from xml.etree.ElementTree import ElementTree


class TamyMesh:

	def __init__( self, mesh ):
		self.mesh = mesh


#	HELPER METHODS
	def faceToTriangles( self, face ):
		indices = []
		for vIdx in face.verts:
			indices.append( vIdx )

		triangles = []
		if (len(indices) == 4): #quad
			triangles.append( [ indices[0], indices[1], indices[2] ] )
			triangles.append( [ indices[2], indices[3], indices[0] ] )
		else:
			triangles.append(indices)
		return triangles
			
	def export_resource( self, resourceRoot ):
	
#		Calculate normals
		self.mesh.calc_normals()

		meshElem = xml.etree.ElementTree.SubElement( resourceRoot, "Resource", type="MESH", name=self.mesh.name )

#		...next - the vertices
		verticesElem = xml.etree.ElementTree.SubElement( meshElem, "Vertices" )
		for v in self.mesh.verts:
			xml.etree.ElementTree.SubElement( verticesElem, "Vtx",
											pos="{0.x} {0.y} {0.z}".format( v.co ),
											norm="{0.x} {0.y} {0.z}".format( v.normal ) )

#		...and last but not least - the faces
		triangles = []
		for face in self.mesh.faces:
			triangles.extend( self.faceToTriangles( face ) )

		facesElem = xml.etree.ElementTree.SubElement( meshElem, "Faces" )
		for tri in triangles:
			xml.etree.ElementTree.SubElement( facesElem, "Face", idx="{0} {1} {2}".format( tri[0], tri[1], tri[2] ) )


class TamyMaterial:

	def __init__( self, name, material, image, scene ):
		self. name =  name
		self.mat = material
		self.image = image
		
		world = scene.world
		self.worldAmb = world.ambient_color
		
#	a helper method that copies an image to the export directory
	def copy_image( self ):
		fn = bpy.utils.expandpath( image.filename )
		fn_strip = os.path.basename(fn)
		
		rel = fn_strip
		fn_abs_dest = os.path.join(dest_dir, fn_strip)
		if not os.path.exists(fn_abs_dest):
			shutil.copy(fn, fn_abs_dest)
		else:
			rel = fn

		return rel

#	material export method
	def export_resource( self, resourceRoot ):

# 		Get the Blender data for the material and the image.
# 		Having an image named None will make a bug, dont do it :)
		materialElem = xml.etree.ElementTree.SubElement( resourceRoot, "Resource", type="MATERIAL", name=self.name )

		if self.mat:
			ambient = tuple( [ c * self.mat.ambient for c in self.worldAmb ] )
			diffuse = tuple( [ c * self.mat.diffuse_intensity for c in self.mat.diffuse_color ] )
			specular = tuple( [ c * self.mat.specular_intensity for c in self.mat.specular_color ] )
			xml.etree.ElementTree.SubElement( materialElem, "Hardness", val="{0}".format( ( self.mat.specular_hardness-1 ) * 1.9607843137254901 ) )
			xml.etree.ElementTree.SubElement( materialElem, "Ambient", val="{0} {1} {2}".format( ambient[0], ambient[1], ambient[2] ) )
			xml.etree.ElementTree.SubElement( materialElem, "Diffuse", val="{0} {1} {2}".format( diffuse[0], diffuse[1], diffuse[2] ) )
			xml.etree.ElementTree.SubElement( materialElem, "Specular", val="{0} {1} {2}".format( specular[0], specular[1], specular[2] ) )

			if hasattr( self.mat, "ior" ):
				xml.etree.ElementTree.SubElement( materialElem, "Refraction", idx="{0}".format( self.mat.ior ) )
			else:
				xml.etree.ElementTree.SubElement( materialElem, "Refraction", idx="1.0" )

			xml.etree.ElementTree.SubElement( materialElem, "Alpha", val="{0}".format( self.mat.alpha ) )

#			0 to disable lighting, 1 for ambient & diffuse only (specular color set to black), 2 for full lighting.
			if self.mat.shadeless:
				xml.etree.ElementTree.SubElement( materialElem, "Illumination", type="NO_LIGHT" )
			elif self.mat.specular_intensity == 0:
				xml.etree.ElementTree.SubElement( materialElem, "Illumination", type="NO_SPECULAR" )
			else:
				xml.etree.ElementTree.SubElement( materialElem, "Illumination", type="REGULAR" )

		else:
#			write a dummy material
			xml.etree.ElementTree.SubElement( materialElem, "Hardness", val="0" )
			xml.etree.ElementTree.SubElement( materialElem, "Ambient", val="%.6f %.6f %.6f".format( tuple( [ c for c in self.worldAmb ] ) ) )
			xml.etree.ElementTree.SubElement( materialElem, "Diffuse", val="0.8 0.8 0.8" )
			xml.etree.ElementTree.SubElement( materialElem, "Specular", val="0.8 0.8 0.8" )

			xml.etree.ElementTree.SubElement( materialElem, "Refraction", idx="1.0" )
			xml.etree.ElementTree.SubElement( materialElem, "Alpha", val="1.0" )
			xml.etree.ElementTree.SubElement( materialElem, "Illumination", type="REGULAR" )

#		Write images!
		if self.image:	 # We have an image on the face!
# 			write relative image path
			rel = copy_image( self.image )
			xml.etree.ElementTree.SubElement( materialElem, "Image", path=rel )

		elif self.mat: # No face image. if we havea material search for MTex image.
			for mtex in self.mat.texture_slots:
				if mtex and mtex.texture.type == 'IMAGE':
					try:
						filename = copy_image( mtex.texture.image )
						xml.etree.ElementTree.SubElement( materialElem, "Image", path=filename )
						break
					except:
# 						Texture has no image though its an image type, best ignore.
						pass


class ExportTamy(bpy.types.Operator):
	'''Export mesh data to Tamy game engine'''
	bl_idname = "export_mesh.btm"
	bl_label = "Export Tamy"

	path = StringProperty(name="File Path", description="File path used for exporting the Tamy file", maxlen= 1024, default= "")
	check_existing = BoolProperty(name="Check Existing", description="Check and warn on overwriting existing files", default=True, options={'HIDDEN'})

# 	context group
	use_selection = BoolProperty(name="Selection Only", description="", default= False)

# a global materials dictionary
	MaterialsDict = {}
	
#	HELPER METHODS
	def fixName( self, name ):
		if name == None:
			return 'None'
		else:
			return name.replace(' ', '_')

	def getLocalMtx( self, obj ):

		if obj.parent is not None :
			parentMtx = obj.parent.matrix
			childMtx = obj.matrix
			localMtx = parentMtx.copy().invert()*childMtx
			return localMtx;
		else:
			return obj.matrix

	def parse_mesh_materials( self, mesh, resources ):

		outResources = set()

#		create a list of all materials and thir names
		materialNames = []
		materialItems = [m for m in mesh.materials]
		if mesh.materials:
			for mat in mesh.materials:
				if mat: # !=None
					materialNames.append(mat.name)
				else:
					materialNames.append(None)
#		Possible there null materials, will mess up indicies
#		but at least it will export, wait until Blender gets fixed.
		materialNames.extend( ( 16-len( materialNames ) ) * [ None ] )
		materialItems.extend( ( 16-len( materialItems ) ) * [ None ] )

		for f_index, f in enumerate( mesh.faces ):
			f_mat = min( f.material_index, len( materialNames )-1 )
			tface = mesh.active_uv_texture.data[ f_index ]
			f_image = tface.image
			f_uv = tface.uv
			
			if f_image: # Object is always true.
				key = materialNames[f_mat],	f_image.name
			else:
				key = materialNames[f_mat],	None # No image, use None instead.

#			find the material in the materials db, and if it's not there - create a new one
			mat_idx = self.MaterialsDict.get(key)
			if not mat_idx:
#				create a new material resource
				if key[1] == None:
					matName = ( '%s'% self.fixName( key[0] ) )
				else:
					matName =  ( '%s_%s' % ( self.fixName( key[0] ), self.fixName( key[1] ) ) )

				mat_idx = self.MaterialsDict[ key ] = len( resources )
				resources.append( TamyMaterial( matName, materialItems[f_mat], f_image, self.exported_scene ) )

#			materials are assigned to particular faces, which means that a single object may have
#			many materials on it.
#			For now assume that an object has only one material, and remove the duplicates from this list.
#			TODO: break meshes into submeshes, each having a distinct material assigned
			outResources.add( mat_idx )

		return outResources
	
	def export_object( self, objectsRoot, object, resources ):

		#	Writing time - first the object's name
		if object.name == object.data.name:
			obnamestring = self.fixName(object.name)
		else:
			obnamestring = '%s_%s' % (self.fixName(object.name), self.fixName(object.data.name))

		objectElem = xml.etree.ElementTree.SubElement( objectsRoot, "Object", name=obnamestring )

		#	it's transformation matrix
		locMtx = self.getLocalMtx( object )
		loc = locMtx.translation_part()
		quat = locMtx.to_quat()
		rot = quat.axis
		xml.etree.ElementTree.SubElement( objectElem, "Transform", 
											translation="{0} {1} {2}".format( loc[0], loc[1], loc[2] ),
											rotation="{0} {1} {2} {3}".format( rot[0], rot[1], rot[2], quat.angle ) )
		
		if object.type == 'MESH':
			me = object.create_mesh(True, 'PREVIEW')
			if (len(me.faces)+len(me.verts)): # Make sure there is something to write
	#			save the mesh
				xml.etree.ElementTree.SubElement( objectElem, "Resource", id="{0}".format( len( resources ) ) )
				resources.append( TamyMesh( me ) )
				
	#			parse materials used
				used_materials = self.parse_mesh_materials( me, resources )
				for matIdx in used_materials:
					xml.etree.ElementTree.SubElement( objectElem, "Resource", id="{0}".format( matIdx ) )
			else:
	#			clean up
				bpy.data.meshes.remove(me)

	#	save the object's children
		for child in object.children:
			self.export_object( objectElem, child, resources )

	def export_scene( self, filename, context,
					  EXPORT_SEL_ONLY = True ):

		if EXPORT_SEL_ONLY:
			export_objects = context.selected_objects
		else:
			export_objects = self.exported_scene.objects

	#	open a file for writing
		sceneRoot = xml.etree.ElementTree.Element( "Scene" )

	#	append an xml header	
		resources = []

	# 	gather all the resources used in the scene
		objectsRoot = xml.etree.ElementTree.SubElement( sceneRoot, "Objects" )

		for main_obj in export_objects:

			if main_obj.parent is not None:
				continue

	# 		1.) first linerarize the object's hierarchy
			if main_obj.parent and main_obj.parent.dupli_type != 'NONE':
				print(obj.name, 'is a duplicated child - ignoring')
				continue

			obs = []
			if main_obj.dupli_type != 'NONE':
				print('creating dupli_list on', main_obj.name)
				main_obj.create_dupli_list()

				obs = [(dob.object) for dob in main_obj.dupli_list]

	# 			debug - info about object's children
				print(main_obj.name, 'has', len(obs), 'dupli children')
			else:
				obs = [(main_obj)]

	# 		2.) go throgough the analyzed object's hierarchy and serialize the meshes
			for ob in obs:
				self.export_object( objectsRoot, ob, resources )
		
	#	save the resources
		resourcesRoot = xml.etree.ElementTree.SubElement( sceneRoot, "Resources" )
		for res in resources:
			res.export_resource( resourcesRoot )

	#	close the file
		tree = xml.etree.ElementTree.ElementTree( sceneRoot )
		try:
			tree.write( filename, "UTF-8" )
		except EnvironmentError as err:
			print("{0}: export error: {1}".format( os.path.basename( sys.argv[0] ), err ) )
	
	def execute(self, context):

		self.exported_scene = context.scene

		self.export_scene(self.properties.path, context,
					EXPORT_SEL_ONLY=self.properties.use_selection )
		return {'FINISHED'}

	def invoke(self, context, event):
		wm = context.manager
		wm.add_fileselect(self)
		return {'RUNNING_MODAL'}

# Add to a menu
def menu_func(self, context):
	default_path = bpy.data.filename.replace(".blend", ".btm")
	self.layout.operator(ExportTamy.bl_idname, text="Tamy Export(.btm)").path = default_path


def register():
	bpy.types.register(ExportTamy)
	bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
	bpy.types.unregister(ExportTamy)
	bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
	register()