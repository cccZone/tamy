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


def export_mesh( file, mesh ):

#	HELPER METHODS
	def faceToTriangles(face):
		indices = []
		for vIdx in face.verts:
			indices.append( vIdx )

		triangles = []
		if (len(indices) == 4): #quad
			triangles.append( [ indices[0], indices[1], indices[2] ] )
			triangles.append( [indices[2], indices[3], indices[0] ] )
		else:
			triangles.append(indices)
		return triangles

#	METHOD'S BODY

#	Calculate normals
	mesh.calc_normals()

	file.write('<Mesh>\n')

#	...next - the vertices
	file.write('<Vertices>\n')
	for v in mesh.verts:
		file.write('<Vtx x="%.6f" y="%.6f" z="%.6f" />\n' % tuple(v.co))
	file.write('</Vertices>\n')

#	...and last but not least - the faces
	triangles = []
	for face in mesh.faces:
		triangles.extend(faceToTriangles(face))

	file.write('<Faces>\n')
	for tri in triangles:
		file.write('<Face i1="%d" i2="%d" i3="%d"/>\n' % tuple(tri) )
	file.write('</Faces>\n')

#	end of a mesh
	file.write('</Mesh>\n')


def export_object( file, object ):

#	HELPER METHODS
	def fixName(name):
		if name == None:
			return 'None'
		else:
			return name.replace(' ', '_')

	def getLocalMtx( obj ):

		if obj.parent is not None :
			parentMtx = obj.parent.matrix
			childMtx = obj.matrix
			localMtx = parentMtx.copy().invert()*childMtx
			return localMtx;
		else:
			return obj.matrix

#	METHOD'S BODY

	#	Writing time - first the object's name
	if object.name == object.data.name:
		obnamestring = fixName(object.name)
	else:
		obnamestring = '%s_%s' % (fixName(object.name), fixName(object.data.name))

	file.write('<Object name=%s>\n'  % obnamestring)

	#	it's transformation matrix
	locMtx = getLocalMtx( object )
	loc = locMtx.translation_part()
	quat = locMtx.to_quat()
	rot = quat.axis
	file.write('<Transform translation="%.6f %.6f %.6f" rotation="%.6f %.6f %.6f %.6f" />\n' % \
			(loc[0], loc[1], loc[2], rot[0], rot[1], rot[2], quat.angle) )

#	if this is a mesh
	if object.type == 'MESH':
		me = object.create_mesh(True, 'PREVIEW')
		if (len(me.faces)+len(me.verts)): # Make sure there is somthing to write
			export_mesh( file, me )
		else:
#			clean up
			bpy.data.meshes.remove(me)

#	save the object's children
	for child in object.children:
		export_object( file, child )

	file.write('</Object>\n');

def export_scene( filename, context,
				  EXPORT_SEL_ONLY = True ):

#	METHOD'S BODY
# 	We only want to export current scene
	export_scene = context.scene

	if EXPORT_SEL_ONLY:
		export_objects = context.selected_objects
	else:
		export_objects = export_scene.objects

#	open a file for writing
	file = open(filename, "w")

#	append an xml header
	file.write('<?xml version="1.0" encoding="UTF-8"?>\n')

# 	go through all root objects ( ones withut parents
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
			export_object( file, ob )

#	close the file
	file.close()


class ExportTamy(bpy.types.Operator):
	'''Export mesh data to Tamy game engine'''
	bl_idname = "export_mesh.btm"
	bl_label = "Export Tamy"

	path = StringProperty(name="File Path", description="File path used for exporting the Tamy file", maxlen= 1024, default= "")
	check_existing = BoolProperty(name="Check Existing", description="Check and warn on overwriting existing files", default=True, options={'HIDDEN'})

	# context group
	use_selection = BoolProperty(name="Selection Only", description="", default= False)
	
	def execute(self, context):
		export_scene(self.properties.path, context,
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

