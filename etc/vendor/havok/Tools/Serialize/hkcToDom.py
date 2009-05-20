#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###
### This script is used to read a havok data description xml file and
### turn it into more usable python objects. The other scripts don't
### use the xml directly, but use the python objects instead.
###

import xml.dom.minidom
import os
import re
import util


overridetypeMacros = {"hkZeroPtr":"hkZero<void*>",
                      "hkZeroPtrArray":"hkZero<hkArray<void*>>"}


class HkcParseError(Exception):
    def __init__(self, msg):
        Exception.__init__(self, msg)
#
## Utility methods
#
def dump(o):
    ret = []
    for k in dir(o):
        ret.append("%s : %s" % (k, getattr(o,k)))
    return "\n".join(ret)
def all_slot_names(klass):
    todo = [klass]
    cl = [klass]
    while len(todo):
        k = todo.pop()
        cl.extend( list(k.__bases__) )
        todo.extend( list(k.__bases__) )
    for k in cl:
        if hasattr(k,"__slots__"):
            for s in k.__slots__:
                yield s
def slot_values_as_keywords(o):
    return ",".join( ["%s=%s" % (a, str(getattr(o,a))) for a in all_slot_names(o.__class__) if getattr(o,a) ] )

#
## Schema representation as python objects
#

class Object(object):
    __slots__= ("name","comment")

    def __init__(self):
        self.name = ""
        self.comment = []

#    def __str__(self):
#        ret = []
#        for klass in self.hierarchy():
#            for s in klass.__slots__:
#                a = getattr(self,s)
#                if type(a) == type([]):
#                    x = "\n".join( [str(i) for i in a ] )
#                else:
#                    x = str(a)
#                ret.append( "%s : %s" % (s, x.replace("\n","\n\t")) )
#        ret.insert(0, "<%s at %x>" % (self.__class__, id(self)))
#        return "\n\t".join(ret)


class EnumItem(Object):
    __slots__ = ("value", "initializer", "description")
    def __init__(self):
        Object.__init__(self)
        self.value = None # integer value
        self.initializer = "" # string value e.g. "0x5" or "FOO_8"
        self.description = ""

class Enum(Object):
    __slots__ = ("description", "item", "scope", "defineflags", "reflected")
    def __init__(self):
        Object.__init__(self)
        self.description = ""
        self.item = []
        self.scope = None
        self.defineflags = False
        self.reflected = True
    def full_scope(self,separator="::"):
        r = []
        s = self.scope
        while s:
            r.append( s.name )
            s = s.scope
        r.reverse()
        return separator.join(r)

class TaggedUnion(Enum):
    def __init__(self):
        Enum.__init__(self)

class TaggedUnionItem(EnumItem):
    __slots__ = ("klass",)
    def __init__(self):
        EnumItem.__init__(self)
        self.klass = None

class Class(Object):
    __slots__ = ("abstract", "vtable", "description", "reflected",
                 "parent", "interface", "enum", "_class", "member",
                 "method", "fromheader", "scope", "typedef", "memory_declaration",
                 "serializable", "attributes", "defineattribute", "version" )
    def __init__(self):
        Object.__init__(self)
        self.abstract = False
        self.vtable = False
        self.description = ""
        self.reflected = False
        self.parent = None
        self.interface = []
        self.enum = []
        self._class = []
        self.member = []
        self.method = []
        self.fromheader = False
        self.scope = None
        self.typedef = {}
        self.memory_declaration = None
        self.serializable = True
        self.attributes = {}
        self.defineattribute = False
        self.version = 0
    def full_scope(self,separator="::"):
        r = []
        s = self.scope
        while s:
            r.append( s.name )
            s = s.scope
        r.reverse()
        return separator.join(r)

class Interface(Object):
    __slots__ = ()
    def __init__(self):
        Object.__init__(self)

class Member(Object):
    __slots__ = (
        "declaredtype", "overridetype", "overrideoffset",
        "type", "description", "visibility",
        "default", "absmin", "absmax", "softmin", "softmax", "owned",
        "flags", "align", "sourcecode", "reflected", "serialized", "attributes" )
    def __init__(self):
        Object.__init__(self)
        self.declaredtype = None
        self.overridetype = None
        self.overrideoffset = None
        self.type = ""
        self.description = ""
        self.visibility = ""
        self.default = None
        self.absmin = None
        self.absmax = None
        self.softmin = None
        self.softmax = None
        self.owned = True
        self.flags = ""
        self.align = 0
        self.sourcecode = ""
        self.reflected = True
        self.serialized = True
        self.attributes = {}

class Parameter(Object):
    __slots__ = ("type", "description", "default")
    def __init__(self):
        Object.__init__(self)
        self.description = ""
        self.type = None
        self.default = None

class Method(Object):
    __slots__ = ("description", "visibility", "inline", "static",
                 "virtual", "purevirtual", "const", "returns", "parameter")
    def __init__(self):
        Object.__init__(self)
        self.description = ""
        self.visibility = ""
        self.inline = True
        self.static = False
        self.virtual = False
        self.purevirtual = False
        self.const = False
        self.returns = ""
        self.parameter = []

class File(Object):
    __slots__ = (
                 "product", "platform", "visibility",
                 "overridedestination", "includeheaders",
                 "enum", "struct", "_class", "taggedunion")
    def __init__(self):
        Object.__init__(self)
        self._class = []
        self.enum = []
        self.struct = []
        self.product = ""
        self.platform = ""
        self.visibility = ""
        self.overridedestination = ""
        self.includeheaders = ""
        self.taggedunion = []

    def mergeWith(self, file):
        self.product = util.mergeTags(self.product, file.product)
        self.platform = util.mergeTags(self.platform, file.platform)
        self._class.extend(file._class)
        self.enum.extend(file.enum)
        self.struct.extend(file.struct)
        self.taggedunion.extend(file.taggedunion)
    def destinationFilename(self, filepath, newextenstion):
        stem, ext = os.path.splitext( filepath )
        if self.overridedestination:
            head, tail = os.path.split( stem )
            stem = os.path.normpath( os.path.join(head, self.overridedestination, tail) )
        return (stem + newextenstion).replace("\\","/")

class Document(Object):
    __slots__ = ("file", "origfilename", "localfilename", "pchfile")
    def __init__(self, pathname):
        Object.__init__(self)
        self.file = None
        self.origfilename = pathname # unchanged, passed to ctor
        self.localfilename = pathname # relative to include path
        self.pchfile = ""
        # try to canonicalize the filename and find the pch and relative include
        realname = []
        leading = os.path.abspath(pathname)
        while len(leading) and leading not in (os.path.sep, os.path.altsep):
            leading, tail = os.path.split( leading )
            if tail:
                realname.insert(0, tail)
            else: # handle drive names split("c:\\") -> "c:\\", ""
                # we dont want missing settings.build warnings when the customer is rebuilding havokassembly
                # to avoid this we check for the existence of build.py, and do not warn if this is a release build
                if os.path.exists('../../Build/ReleaseSystem/build.py'):
                    print "Couldn't find settings.build in a parent of", pathname
                break
            buildsettings =  os.path.join(leading, "settings.build")
            if os.path.exists(buildsettings):
                settingFile = open(buildsettings).read()
                match = re.search('setPchFile\s*\(\s*"([^"]+)"\s*\)', settingFile, re.M )
                if match:
                    self.pchfile = match.group(1)

                for match in  re.finditer('addIncludePath\s*\(\s*"([^"]+)"\s*\)', settingFile, re.M ):
                    pathMatch = re.search( match.group(1), leading, re.IGNORECASE )
                    if pathMatch:
                        s,e = pathMatch.span(0)
                        self.localfilename = os.path.join( leading[e+1:] , *realname )
                        return

                self.localfilename = os.path.join( os.path.basename(leading) , *realname )
                break
#
## Parsing code
#

def _getChildElements(node):
    return filter(lambda x: x.nodeType == x.ELEMENT_NODE, node.childNodes)

def _getComments(node):
    return filter(lambda x: x.nodeType == x.COMMENT_NODE, node.childNodes)

def getNodePath(node):
    path = []
    while node:
        path.append(node.nodeName)
        for child in node.childNodes:
            if child.nodeName in ("name", "filename"):
                try:
                    name = child.childNodes[0].nodeValue.strip()
                except (IndexError, TypeError):
                    pass
                else:
                    path[-1] = "%s(%s)" % (path[-1], name)
        node = node.parentNode
    path.reverse()
    return ":".join( path )

def _parseSimpleElement(obj, node):
    name = node.nodeName
    try:
        value = "".join( [ n.nodeValue for n in node.childNodes ] )
    except TypeError:
        raise HkcParseError("%s\n(This node should contain only plain text)" % getNodePath(node))

    try:
        attr = getattr(obj, name)
    except AttributeError:
        return #XXX
        raise RuntimeError("'%s' should not have element '%s'\n" \
            "Allowed subelements are '%s'" % (
            obj.__class__.__name__.split(".")[-1],
            name,
            obj.__class__.__slots__ ))

    if not value.strip(): return
    if type(attr) == type(None):
        setattr(obj, name, value)
    elif type(attr) == type([]):
        attr.append( value )
    elif type(attr) in (type(""), unicode):
        if ( len(attr) !=0 ):
            raise RuntimeError("Attribute %s has been set twice in %s\n" % (name, attr))
        setattr(obj, name, value)
    elif type(attr) == type(0):
        setattr(obj, name, int(value))
    elif type(attr) == type({}):
        d = getattr(obj, name)
        for k,v in map( lambda x: x.split(":"), value.split(",") ):
            d[ k.strip() ] = v.strip()
    else:
        print "ignoring node", name, "in", obj.__class__.__name__
        print type(attr)

def parseParameter(node):
    #print ">member", node
    assert node.nodeName in ("parameter",)
    p = Parameter()
    for n in _getChildElements(node):
        _parseSimpleElement(p, n)
    #print "<member", node
    return p

def parseClassMethod(node):
    #print ">member", node
    assert node.nodeName.endswith("method")
    m = Method()
    for n in _getChildElements(node):
        if n.nodeName == "parameter":
            m.parameter.append( parseParameter(n) )
        else:
            _parseSimpleElement(m, n)
    #print "<member", node
    return m

def parseClassMember(node):
    #print ">member", node
    assert node.nodeName == "member"
    m = Member()
    for n in _getChildElements(node):
        # replace the override macros here

        if (n.nodeName == "overridetype") and (n.childNodes[0].nodeValue in overridetypeMacros):
            n.childNodes[0].nodeValue = overridetypeMacros[n.childNodes[0].nodeValue]

        _parseSimpleElement(m, n)
    #print "<member", node
    return m

def parseInterface(node):
    assert node.nodeName == "interface"
    i = Interface()
    for c in _getChildElements(node):
        _parseSimpleElement(i, c)
    return i

def parseEnumItem(node):
    #print ">member", node
    assert node.nodeName == "item"
    i = EnumItem()
    i.name = "".join( [n.data for n in node.childNodes if n.nodeType == n.TEXT_NODE] ).strip()
    if node.attributes:
        i.value = node.attributes["value"].value
    for n in _getChildElements(node):
        _parseSimpleElement(i, n)
    #print "<member", node
    return i

def parseEnum(node):
    #print ">class", node
    assert node.nodeName == "enum"
    e = Enum()
    for n in _getChildElements(node):
        if n.nodeName == "item":
            e.item.append( parseEnumItem(n) )
        else:
            _parseSimpleElement(e, n)
    #print "<class", node
    return e

#def parseStruct(node):
#    assert node.nodeName == "struct"
#    s = Struct()
#    for n in _getChildElements(node):
#        if n.nodeName == "member":
#            s.member.append( parseClassMember(n) )
#        else:
#            _parseSimpleElement(s, n)
#    return s

def parseClass(node):
    #print ">class", node
    assert node.nodeName in "class struct".split()
    c = Class()
    for n in _getChildElements(node):
        if n.nodeName == "member":
            c.member.append( parseClassMember(n) )
        elif n.nodeName == "method":
            c.method.append( parseClassMethod(n) )
        elif n.nodeName == "enum":
            c.enum.append( parseEnum(n) )
        elif n.nodeName in "class struct".split():
            c._class.append( parseClass(n) )
        elif n.nodeName == "interface":
            c.interface.append( parseInterface(n) )
        else:
            _parseSimpleElement(c, n)
    #print "<class", node
    return c

def parseFile(node):
    #print ">file", node
    if node.nodeName != "file":
        raise HkcParseError('This XML file does not contain the "file" tag - are you sure it contains serialisation data?')

    f = File()
    for n in _getChildElements(node):
        if n.nodeName == "class":
            f._class.append( parseClass(n) )
        elif n.nodeName == "enum":
            f.enum.append( parseEnum(n) )
        elif n.nodeName == "struct":
            f._class.append( parseClass(n) )
        else:
            _parseSimpleElement(f, n)
    #print "<file", node
    return f

def parseDocument(node, pathname):
    #print ">document", node
    assert node.nodeName == "#document"
    d = Document(pathname)
    for n in node.childNodes:
        if n.nodeType == n.ELEMENT_NODE:
            assert d.file == None
            d.file = parseFile(n)
        elif n.nodeType == n.COMMENT_NODE:
            pass
        else:
            print "ignoring node", n, n.nodeType
    #print "<document", node
    return d

def hkcToDom(filename):
    try:
        return parseDocument( xml.dom.minidom.parse(filename), filename )
    except HkcParseError, e:
        raise HkcParseError("\n\n%s\n%s" % (filename,e))

def main():
    import sys
    for arg in sys.argv[1:]:
        print hkcToDom(arg)

if __name__=="__main__":
    main()

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
