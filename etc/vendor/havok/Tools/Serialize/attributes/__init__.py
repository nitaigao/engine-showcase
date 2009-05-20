#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

## python

"""
Custom attribute base class. Note the members initialized in the default constructor.

All global identifiers in this file are available as custom attributes.
"""

class Attribute:
    """Base class for all attributes.

    Derived classes should implement the __call__ method. The method may take
    any number and type of arguments. See http://docs.python.org/tut/node6.html
    for how arguments are passed from your header file to __call__.
    """
    def __init__(self, **kw):
        """Keyword arguments:
            symbol : The c++ symbol we should generate
            member : The member to which the attribute applies
            klass : The class containing the member
        """
        self.__dict__.update(kw)
        
    def __call__(self):
        """Method to turn the attribute parameters into code.

        This method should return a string of c++ code which is embedded
        into the Class.cpp file. The code should define a variable with
        the name self.symbol
        """        
        raise NotImplementedError("You must implement a __call__ method")

#
# Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
# 
# Confidential Information of Havok.  (C) Copyright 1999-2009
# Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
# Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
# rights, and intellectual property rights in the Havok software remain in
# Havok and/or its suppliers.
# 
# Use of this software for evaluation purposes is subject to and indicates
# acceptance of the End User licence Agreement for this product. A copy of
# the license is included with this software and is also available at www.havok.com/tryhavok.
# 
#
