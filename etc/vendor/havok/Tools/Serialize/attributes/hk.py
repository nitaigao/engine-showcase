#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

## python

"""
Module hk -- custom attribute definitions

All global identifiers in this file are available as custom attributes.
"""

import attributes

add_dot_f = lambda s: "%#ff" % s

class Attribute(attributes.Attribute):
    @classmethod
    def cppclass(cls):
        return cls.__module__.rsplit(".",1)[1]+cls.__name__
    def struct_initialzer(self, values):
        return 'static const %s %s = { %s };' % (self.cppclass(), self.symbol, values)
    def constructor_initialzer(self, values):
        return 'static const %s %s( %s );' % (self.cppclass(), self.symbol, values)

## simple attributes

class RangeAttribute(Attribute):
    def __call__(self, absmin=None, absmax=None, softmin=None, softmax=None):
        setwithdefault = lambda a, b: a if a != None else b
        absmin = setwithdefault(absmin,self.default_absmin)
        absmax = setwithdefault(absmax,self.default_absmax)
        softmin = setwithdefault(softmin,absmin)
        softmax = setwithdefault(softmax,absmax)
        return self.constructor_initialzer(", ".join( self.cast(a) for a in (absmin,absmax,softmin,softmax) ) )

class RangeRealAttribute(RangeAttribute):
    cast = staticmethod(lambda x: add_dot_f(x) if isinstance(x, float) or isinstance(x, int) else str(x))
    default_absmin = "-HK_REAL_MAX"
    default_absmax = "HK_REAL_MAX"

class RangeInt32Attribute(RangeAttribute):
    cast = staticmethod(lambda x: "%i"%x if isinstance(x, int) else str(x))
    default_absmin = "HK_INT32_MIN"
    default_absmax = "HK_INT32_MAX"

class UiAttribute(Attribute):
    __keywords = ("visible", "hideInModeler", "label", "group", "endGroup", "endGroup2", "advanced", "isALink", "childedOffRB", "isAChild")
    def __call__(self, **kw):
        errors = ""
        for k in kw.keys():
            if not k in self.__keywords:
                errors += "// Unused arg %s\n" % k
        if errors: print errors
        return '%sstatic const hkUiAttribute %s = { %s, %s::%s, "%s", "%s", %s, %s, %s };' % ( errors, self.symbol, str(kw.get("visible","True")).lower(), self.cppclass(), str(kw.get("hideInModeler","NONE")), str(kw.get("label","")), str(kw.get("group","")), str(kw.get("endGroup","False")).lower(), str(kw.get("endGroup2","False")).lower(), str(kw.get("advanced","False")).lower() )

class GizmoAttribute(Attribute):
    __keywords = ("visible", "label", "type")
    def __call__(self, **kw):
        errors = ""
        for k in kw.keys():
            if not k in self.__keywords:
                errors += "// Unused arg %s\n" % k
        if errors: print errors
        return '%sstatic const %s %s = { %s, "%s", %s::%s };' % ( errors, self.cppclass(), self.symbol, str(kw.get("visible","True")).lower(), str(kw.get("label","")), self.cppclass(), str(kw.get("type","POINT")) )

class ModelerNodeTypeAttribute(Attribute):
    def __call__(self, type):
        return 'static const %s %s = { %s::%s };' % (self.cppclass(), self.symbol, self.cppclass(), type)

class LinkAttribute(Attribute):
    def __call__(self, type):
        return 'static const %s %s = { %s::%s };' % (self.cppclass(), self.symbol, self.cppclass(), type)

class ArrayTypeAttribute(Attribute):
    def __call__(self, type):
        return 'static const %s %s = { %s::%s };' % (self.cppclass(), self.symbol, self.cppclass(), type)

class SemanticsAttribute(Attribute):
    def __call__(self, type):
        return 'static const %s %s = { %s::%s };' % (self.cppclass(), self.symbol, self.cppclass(), type)

class DescriptionAttribute(Attribute):
   def __call__(self, description=""):
        return self.struct_initialzer( '"%s"'%description )

class DataObjectTypeAttribute(Attribute):
    def __call__(self, typename=""):
        return self.struct_initialzer( '"%s"'%typename )

class DocumentationAttribute(Attribute):
    __keywords = ("docsSectionTag")
    def __call__(self, **kw):
        errors = ""
        for k in kw.keys():
            if not k in self.__keywords:
                errors += "// Unused arg %s\n" % k
        if errors: print errors
        return '%sstatic const %s %s = { "%s"};' % ( errors, self.cppclass(), self.symbol, str(kw.get("docsSectionTag","")) )

Ui = UiAttribute
Gizmo = GizmoAttribute
ModelerNodeType = ModelerNodeTypeAttribute
Link = LinkAttribute
ArrayType = ArrayTypeAttribute
Semantics = SemanticsAttribute
RangeReal = RangeRealAttribute
RangeInt32 = RangeInt32Attribute
Description = DescriptionAttribute
DataObjectType = DataObjectTypeAttribute
Documentation = DocumentationAttribute


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
