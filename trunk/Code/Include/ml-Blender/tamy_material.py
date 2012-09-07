### @file	ml-Blender/tamy_material.py
### @brief	structures that represent Tamy material data
### ===========================================================================

from ctypes import *
import bpy
from . import tamy_misc

		
### ===========================================================================
	
### This class represents a texture exported to Tamy
class TamyTexture( Structure ):

	_fields_ = [ ("path", c_char_p) ]
	
	### Constructor
	def __init__( self, texturePath ):
		self.path = texturePath.encode( "utf-8" )
	
		
### ===========================================================================
	
### A class that represents a material
class TamyMaterial( Structure ):
	_fields_ = [("name", c_char_p),
				("ambientColor", tamy_misc.TamyColor),
				("diffuseColor", tamy_misc.TamyColor),
				("specularColor", tamy_misc.TamyColor),
				("normalTextureIndex", c_int),
				("diffuseTexturesIndices", POINTER(c_int)),
				("diffuseTexturesCount", c_int) ]
	
	# -------------------------------------------------------------------------
	
	def __init__( self, material, image, texturesDict ):
	
		materialName = material.name if material else "None"
		self.name = materialName.encode( "utf-8" )

		if not material:
			self.ambientColor = tamy_misc.TamyColor( 0.0, 0.0, 0.0, 1.0 )
			self.diffuseColor = tamy_misc.TamyColor( 0.0, 0.0, 0.0, 1.0 )
			self.specularColor = tamy_misc.TamyColor( 0.0, 0.0, 0.0, 1.0 )
		else:
			r, g, b = ( material.ambient * material.diffuse_color )[:]
			self.ambientColor = tamy_misc.TamyColor( r, g, b, 1.0  )
			
			r, g, b = material.diffuse_color[:]
			self.diffuseColor = tamy_misc.TamyColor( r, g, b, 1.0 )
			
			r, g, b = material.specular_color[:]
			self.specularColor = tamy_misc.TamyColor( r, g, b, 1.0 )

		slots = self.get_material_image_texslots( material )  # can be None

		if slots:

			# we only want one of each special texture ( i.e. normal, specular and opacity maps )
			normal = [s for s in slots if s.use_map_normal]
			if len( normal ) > 0:
				self.normalTextureIndex = get_texture_index( normal[0] )
			else:
				self.normalTextureIndex = -1

			# make sure no textures are lost. Everything that doesn't fit
			# into a channel is exported as diffuse texture with a
			# warning.

			diffuseTextureIndices = []
			for s in slots:
				if ( s.use_map_color_diffuse ):
					texIdx = get_texture_index( s )
					if texIdx >= 0:
						diffuseTextureIndices.append( texIdx )

			# copy the diffuse maps to the dedicated export array
			self.diffuseTexturesCount = len( diffuseTextureIndices )
			self.diffuseTexturesIndices = ( c_int * self.diffuseTexturesCount )()
			for i in range( self.diffuseTexturesCount ):
				self.diffuseTexturesIndices[i] = diffuseTextureIndices[i]
				
	# -------------------------------------------------------------------------
	
	### Returns an export index assinged to this texture
	def get_texture_index( self, textureSlot, texturesDict ):
	
		textureIdx = -1
		
		if ( textureSlot.texture.type == 'IMAGE' ):
			texturePath = bpy.path.abspath( normal[0].texture.image.filepath )
				
			# check if the texture was mapped and exported
			if ( texturePath in texturesDict ):
				textureIdx = texturesDict[texturePath]
			else:
				textureIdx = len( texturesDict )
				texturesDict[texturePath] = textureIdx
			
		# This is not a correct texture, or it hasn't been exported - anyway, it doesn't have an index assigned
		return textureIdx

	# -------------------------------------------------------------------------
	
	### This helper functions enumerates all occupied texture slots
	def get_material_image_texslots( self, material ):
		# blender utility func.
		if material:
			return [s for s in material.texture_slots if s and s.texture.type == 'IMAGE' and s.texture.image]

		return []
	
### ===========================================================================
