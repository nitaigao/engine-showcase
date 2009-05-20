#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

## python

"""
Module Example -- custom attribute definitions

All global identifiers in this file are available as custom attributes.
See Demo/Demos/Common/Api/CustomAttributes for an example using these attributes.
"""

import attributes

class Attribute(attributes.Attribute):
    def cppclass(self):
        return "Example"+self.__class__.__name__

## simple attributes

class StringAttribute(Attribute):
    """Single string attribute.
    """
    def __call__(self, value):
        #return 'static const ExampleStringAttribute %s = { "%s" };' % (self.symbol, value)
        return 'static const %s %s = { "%s" };' % (self.cppclass(), self.symbol, value)

class BooleanAttribute(Attribute):
    """Single boolean attribute.
    """
    def __call__(self, value):
        return 'static const %s %s = { %s };' % ( self.cppclass(), self.symbol, str(value).lower() )

class StringPairsAttribute(Attribute):
    """List of string key,value pairs
    """
    def __call__(self, **kw):
        items = "%s_items" % self.symbol
        code = ["static const %s::Item %s[] = { " % (self.cppclass(), items) ]
        code.append( ", ".join('{"%s","%s"}' % (k,v) for k,v in kw.items() ) )
        code.append(" };\n")
        code.append("static const %s %s = { %s, HK_COUNT_OF(%s) };" % (self.cppclass(), self.symbol, items, items))
        return "".join(code)

class IntListAttribute(Attribute):
    """List of integer values
    """
    def __call__(self, *args):
        items = "%s_items" % self.symbol
        code = ["static const int %s[] = { " % items]
        code.append( ", ".join('%s' % a for a in args) )
        code.append(" };\n")
        code.append("static const %s %s = { %s, HK_COUNT_OF(%s) };" % (self.cppclass(), self.symbol, items, items))
        return "".join(code)


## These attributes use predefined attribute types.
## Think of these assignments as "X isA Y"
GroupWith = StringAttribute
IconPath = StringAttribute
VisibleInTool = BooleanAttribute
Filename = StringPairsAttribute
GuiLayoutOrder = IntListAttribute

## more complex attributes

class StringRole(Attribute):
    # See customAttributes.Attribute.__init__ for the variables which are
    # set in the constructor.
    def __call__(self, role):
        return 'static const %s %s = { %s::%s };' % (self.cppclass(), self.symbol, self.cppclass(), role)


class RangeReal(Attribute):
    def __call__(self, **kw ):
        # this calling style puts all keyword arguments (foo=bar) into a dictionary
        members = "absmin", "absmax", "softmin", "softmax"
        validbits = "|".join( "%s::VALID_%s" % (self.cppclass(),m.upper()) for m in members if kw.has_key(m)) or "0"
        add_dot_f = lambda s: s+".0f" if s.find(".")==-1 else s+"f"
        values = ",".join( add_dot_f(str(kw.get(m,0))) for m in members )
        return 'static const %s %s = { %s, %s };' % (self.cppclass(), self.symbol, validbits, values)


class RawByteData(Attribute):
    """This attribute references externally defined data.
    """
    def __call__(self, data):
        return "extern const %s %s;\nstatic const %s& %s = %s;" % (self.cppclass(), data, self.cppclass(), self.symbol, data)
IconData = RawByteData




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
