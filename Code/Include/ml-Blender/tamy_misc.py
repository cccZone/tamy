### @file	ml-Blender/tamy_misc.py
### @brief	helper structures
### ===========================================================================

from ctypes import *


### ===========================================================================

### This class represents a color exported to Tamy
class TamyColor( Structure ):

	_fields_ = [ ("r", c_float), ("g", c_float), ("b", c_float), ("a", c_float) ]
	
### ===========================================================================

### A helper structure that represents an exportable matrix	
class TamyMatrix( Structure ):
	_fields_= [ ( "m", c_float * 16 ) ]
	
	def __init__( self, blenderMtx ):
		sideVec, upVec, frontVec, transVec = blenderMtx[:]
		self.m[0] = sideVec[0]
		self.m[1] = upVec[0]
		self.m[2] = frontVec[0]
		self.m[3] = transVec[0]
		
		self.m[4] = sideVec[1]
		self.m[5] = upVec[1]
		self.m[6] = frontVec[1]
		self.m[7] = transVec[1]
		
		self.m[8] = sideVec[2]
		self.m[9] = upVec[2]
		self.m[10] = frontVec[2]
		self.m[11] = transVec[2]
		
		self.m[12] = sideVec[3]
		self.m[13] = upVec[3]
		self.m[14] = frontVec[3]
		self.m[15] = transVec[3]
		
	def invertRotation( self ):
		for i in range( 11 ):
			self.m[i] = -self.m[i]
		
### ===========================================================================
