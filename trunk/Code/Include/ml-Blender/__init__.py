bl_info = {
	"name": "Tamy exporter",
	"author": "Piotr Trochim",
	"blender": (2, 5, 7),
	"location": "File > Import-Export",
	"description": "Export Tamy models and animations",
	"warning": "",
	"wiki_url": "http://code.google.com/p/tamy/wiki/TableOfContents?tm=6",
	"tracker_url": "",
	"support": 'OFFICIAL',
	"category": "Import-Export"}

if "bpy" in locals():
	import imp
	if "export_tamy" in locals():
		imp.reload(export_tamy)


import bpy
from bpy.props import StringProperty, FloatProperty, BoolProperty, EnumProperty

from bpy_extras.io_utils import ( ImportHelper, ExportHelper, axis_conversion, )

class ExportTamy( bpy.types.Operator, ExportHelper ):
	'''Export to Tamy file format'''
	bl_idname = "export_scene.tamy"
	bl_label = 'Export Tamy'

	filename_ext = ".tsc"
	filter_glob = StringProperty(
			default="*.tsc",
			options={'HIDDEN'},
			)
			
	filesystemRoot = bpy.props.StringProperty(
			name="Filesystem root",
			description="Filesystem root directory",
			default="G:/Docs/Projects/Tamy/Assets/" )
			
	bUseSelection = BoolProperty(
			name="Selection Only",
			description="Export selected objects only",
			default=False,
			)

	axis_forward = EnumProperty(
			name="Forward",
			items=(	('X', "X Forward", ""),
					('Y', "Y Forward", ""),
					('Z', "Z Forward", ""),
					('-X', "-X Forward", ""),
					('-Y', "-Y Forward", ""),
					('-Z', "-Z Forward", ""),
					),
			default='Y',
			)

	axis_up = EnumProperty(
			name="Up",
			items=(	('X', "X Up", ""),
					('Y', "Y Up", ""),
					('Z', "Z Up", ""),
					('-X', "-X Up", ""),
					('-Y', "-Y Up", ""),
					('-Z', "-Z Up", ""),
					),
			default='Z',
			)

	def execute(self, context):
		from . import export_tamy

		keywords = self.as_keywords( ignore=( "axis_forward", "axis_up", "filter_glob", "check_existing", ) )
		globalMatrix = axis_conversion( to_forward=self.axis_forward, to_up=self.axis_up, ).to_4x4()
		keywords["globalMatrix"] = globalMatrix
		keywords["filesystemRoot"] = self.filesystemRoot

		return export_tamy.export_scene( self, context, **keywords )
	
# Add to a menu
def menu_func_export(self, context):
	self.layout.operator(ExportTamy.bl_idname, text="Tamy scene (.tsc)")

def register():
	bpy.utils.register_module(__name__)

	bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
	bpy.utils.unregister_module(__name__)

	bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
	register()
