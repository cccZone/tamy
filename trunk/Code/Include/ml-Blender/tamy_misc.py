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
		col0, col1, col2, col3 = blenderMtx[:]
		self.m[0] = 1 #sideVec[0]
		self.m[1] = 0 #upVec[0]
		self.m[2] = 0 #frontVec[0]
		self.m[3] = 0 #transVec[0]
		
		self.m[4] = 0 #sideVec[1]
		self.m[5] = 1 #upVec[1]
		self.m[6] = 0 #frontVec[1]
		self.m[7] = 0 #transVec[1]
		
		self.m[8] = 0 #sideVec[2]
		self.m[9] = 0 #upVec[2]
		self.m[10] = 1 #frontVec[2]
		self.m[11] = 0 #transVec[2]
		
		self.m[12] = col0[3]
		self.m[13] = col1[3]
		self.m[14] = col2[3]
		self.m[15] = col3[3]

		
### ===========================================================================
