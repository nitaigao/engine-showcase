#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###
### This script is used to generate a c++ header file from an hkClass xml dom.
### Now that we can extract xml directly from the c++ header, this file is
### of diminishing utility.
###
### This script can be either be executed directly or imported
###

_tkbms = """// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM   : %s
// PRODUCT    : %s
// VISIBILITY : %s
//
// ------------------------------------------------------TKBMS v1.0"""

def _add_single_dot_f(val):
    """Add a .f to the value if necessary to avoid double->float warnings
    """
    val = val.strip()
    if val[-1] != 'f':
        try:
            float(val)
        except ValueError:
            if val.find("::") == -1:
                raise ValueError("'%s' is not a floating point value" % val)
        else:
            if val[-1] == ".":
                val = val + "0f"
            elif val.find('.') == -1:
                val = val + ".0f"
            else:
                val = val + "f"
    return val

def _add_dot_fs(val):
    """Add .f to several values.
    """
    val = val.replace(" ","").replace("\t", "")
    b0 = val.find("(")
    if b0 != -1:
        bits = []
        while b0 != -1:
            b1 = val.find(")", b0)
            bits.append(", ".join([ _add_single_dot_f(f) for f in val[b0+1:b1].split(",")]))
            b0 = val.find("(", b1)
        return "(" + ") (".join(bits) + ")"
    else:
        return _add_single_dot_f(val)

def _get_array_type(a):
    """Return the type of elements in an array.
    """
    return a[ a.index("<")+1 : a.rindex(">") ]

##""" _initcode only called per member for structs
##"""
##def _initcode(scope, name, type, value):
##    """Return the c++ code to initialize the given member.
##    """
##    addf = 0
##    if type.endswith("*"):
##        init = "%(name)s = HK_NULL;", "%(name)s = %(value)s;"
##    elif type.startswith("enum"):
##        etype = type.split(":")[0].split()[-1]
##        init = "%%(name)s = %s(0);" % etype, "%%(name)s = %s(%%(value)s);" % etype
##    elif type == "hkVector4":
##        init = "%(name)s.setZero4();", "%(name)s.set%(value)s;"
##        addf = 1
##    elif type == "hkQuaternion":
##        init = "%(name)s.setIdentity();", None #"%s.set(%s);"
##        addf = 1
##    elif type == "hkQsTransform":
##        init = "%(name)s.setIdentity();", None
##        addf = 1
##    elif type in "hkMatrix3 hkRotation hkMatrix4 hkTransform".split():
##        #assert len(value.split(","))==3
##        init = "%(name)s.setIdentity();", "%(name)s.setDiagonal%(value)s;"
##        addf = 1
##    elif type.startswith("hkArray"):
##        assert value == None # array initializers in structs not supported yet
##        init = "// %(name)s", ""
##    elif type.startswith("hkSimpleArray"):
##        assert value == None # array initializers in structs not supported yet
##        init = "%(name)s = HK_NULL;\n%(name)sSize = 0;", ""
##    elif type.startswith("hkHomogeneousArray"):
##        assert value == None # array initializers in structs not supported yet
##        init = "%(name)sType = HK_NULL;\n%(name)s = HK_NULL;\n%(name)sSize = 0;", ""
##    elif type.startswith("struct"):
##        init = "// %(name)s",""
##    elif type.find("[") != -1:
##        assert value == None # array initializers in structs not supported yet
##        init = "// %(name)s", "// %(name)s = %(value)s;"
##    else:
##        init = "%(name)s = 0;", "%(name)s = %(value)s;"
##        if type == "hkReal":
##            addf = 1
##    if value == None:
##        return init[0] % {"name": name}
##    elif addf:
##        return init[1] % {"name": name, "value": _add_dot_fs(value)}
##    else:
##        return init[1] % {"name": name, "value":value}

def _membername(name):
    return "m_%s" % name

def _wrap(text, width):
    """Given a block of text and a width, reflow the text.
    """
    while 1:
        a = text.replace("\t\t", "\t")
        if a == text: break
        else: text = a        
    words = filter( len, text.replace("\t", "\\t").split() )
    if words:
        return reduce(lambda line, word: '%s%s%s' %
                  (line,
                   ' \n'[word == "\\n" or (len(line[line.rfind('\n')+1:])
                         + len(word.split('\n',1)[0]
                              ) >= width)],
                   word != "\\n" and word or " "),
                   words
                 ).replace("\\t", "\t")
    return ""
#
#
#

def domToHeader(dom, debug=0):
    """Return the text of a c++ header file from a given hkClass dom.
    """

    class ProgramList:
        """Class to generate a nicely formatted c++ listing.
        """
        def __init__(self):
            self.lines = []
            self.level = 0
        def indent(self, delta):
            self.level += delta
        def append(self, txt):
            for line in txt.split("\n"):
                self.level -= line.count("}")
                comment = self.level != 0 and line.find("///") != -1
                nline = "%s%s" % ((self.level + comment)* "\t", line)
                self.level += line.count("{")
                self.lines.append(nline)
                if debug: print nline
    ret = ProgramList()

    def getDesc(d):
        """Get a formatted comment from a piece of text.
        """
        return "/// " + "\n/// ".join(_wrap(d,80).split("\n"))

    def genMember(m):
        """Generate a c++ member definition.
        """
        mextra = ""
        if (m.type.startswith("hkSimpleArray")):
            mptrtype = m.type.split("<")[-1].split(">")[0]
            mptrname = _membername(m.name)
            mnumname = _membername("num%s%s" % (m.name[0].upper(), m.name[1:]))
            ret.append( "\n%s\n%s* %s;\nhkInt32 %s;" % (
                getDesc(m.description),
                mptrtype,
                mptrname,
                mnumname ) )
        elif (m.type.startswith("hkZero")):
            mptrtype = m.type.split("<")[-1].split(">")[0]
            mptrname = _membername(m.name)

            ret.append( "\n%s\n%s %s;" % (
                getDesc(m.description),
                mptrtype,
                mptrname) )
        elif (m.type.startswith("hkHomogeneousArray")):
            mptrname = _membername(m.name)
            mnumname = _membername("num%s%s" % (m.name[0].upper(), m.name[1:]))
            ret.append( "\n%s\nconst hkClass* %sType;\nvoid* %s;\nhkInt32 %s;" % (
                getDesc(m.description),
                mptrname,
                mptrname,
                mnumname ) )
        else:
            if m.type.startswith("enum "):
                try:    
                    name, msize = m.type.split(":")
                except ValueError:
                    name, msize = m.type, 8
                mtype = "hkEnum<%s, hkInt%s>" % (name, msize)
            elif m.type.find("[") != -1:
                mtype, mextra = m.type.split("[")
                mextra = "[" + mextra
            else:
                mtype = m.type
            ret.append( "\n%s\n%s %s%s;" % (
                getDesc(m.description),
                mtype,
                _membername(m.name),
                mextra ) )

    def genParameter(p):
        """Generate a parameter name pair.
        """
        return "%s %s" % (p.type, p.name)

    def genMethod(m,c):
        """Generate a method declaration for the given class.
        """
        if m.returns == "":
            if m.name not in (c.name, "~"+c.name):
                returns = "void "
            else:
                returns = ""
        else:
            returns = m.returns+" "
        ret.append("\n%s\n%s%s%s%s(%s)%s%s;" % (
            getDesc(m.description),
            ["", "inline "][m.inline],
            ["", "virtual "][m.virtual or m.purevirtual], 
            returns,
            m.name,
            ", ".join( map(genParameter, m.parameter) ),
            ["", " const"][m.const],
            ["", " = 0"][m.purevirtual] ) )
        
    def genFastConstructor(c):
        ret.indent(-1)
        ret.append("\npublic:")
        ret.append("\n\t\t// Constructor for initialisation of vtable fixup")
        ret.indent(1)
        if c.parent:
            ret.append("\tHK_FORCE_INLINE %s( hkFinishLoadedObjectFlag flag ) : %s(flag) {}" % (c.name, c.parent) )
        else:
            ret.append("\tHK_FORCE_INLINE %s( hkFinishLoadedObjectFlag flag ) {}" % (c.name) )
            ret.append("\tvoid* operator new(size_t, void* p) { return p; }")
        

    def genClass(c):
        """Generate the c++ class.
        """
        if c.vtable:
            ret.append("#include <Common/Base/Reflection/hkClass.h>")
            
        # Declare hkClass so it can be accessed.
        ret.append("\n/// %s meta information" % c.name)
        ret.append("extern const class hkClass %sClass;\n" % c.name)
        ret.append(getDesc(c.description) )
        # Struct decl
        parents = [i.name for i in c.interface]
        if c.parent:
            parents.insert(0, c.parent)
        if parents:
            inherit = " : " + ", ".join(["public %s" % p for p in parents])
        else:
            inherit = ""
        
        ret.append("class %s%s\n{" % (c.name, inherit))
        ret.append("public:\n")
        ret.indent(1)
        if False in [m.visibility in ("","public") for m in c.member]:
            ret.append("HK_DECLARE_REFLECTION();\n")
        else:
            # See HVK-2211 : Errors compiling filter options
            haveDefaults = len([m for m in c.member if m.default != None])
            if haveDefaults:
                ret.append("// Struct holding default values\n")
                ret.append("struct DefaultStruct;\n")

        #if not c.parent:
        #    ret.append("\tvoid* operator new(hk_size_t, void* p) { return p; }")
        #    ret.append("\tvoid  operator delete(void*, void*) { }\n")
        for e in c.enum:
            genEnum(e)
            ret.append("")
        for s in c._class:
            genStruct(s)
        ret.append("/// Default constructor\n%s() { }" % c.name)
        if len(c.method):
            ret.append("\n//\n// Methods\n//")
        for m in c.method:
            genMethod(m, c)
        if len(c.member):
            ret.append("\n//\n// Members\n//")
        def genVisible(vis):
            l = [ m for m in c.member if m.visibility in vis ]
            if len(l):
                ret.indent(-1)
                ret.append("%s:" % vis[0])
                ret.indent(1)
                [ genMember(m) for m in l ]
        #genVisible( ("public","", "private", "protected") )
        genVisible( ("public","") )
        genVisible( ("private",) )
        genVisible( ("protected",) )

        if c.vtable:
            genFastConstructor(c)
            
        ret.indent(-1)
        ret.append("};\n")
        # include inl if relevant
        found_inline = 1 in [ m.inline for m in c.method ]
        if found_inline:
            ret.append("#include <%s>\n" % (dom.localfilename.replace(".hkclass",".inl").replace("\\","/") ) )

    def genEnum(e):
        """Generate a c style enumeration.
        """
        ret.append(getDesc(e.description) )
        ret.append("enum %s\n{" % e.name)
        for i in e.item:
            ret.append("%s\n%s%s%s" % (
                getDesc(i.description),
                i.name,
                ["=%s"%i.value, ""][i.value==None],
                [",", ""][i==e.item[-1]] ) )
        ret.append("};")

    def genStruct(s):
        """Generate a struct definition.
           Structs are very similar to classes except they are assumed to be simpler
           and so the constructor is inlined.
        """
        ret.append("%s\nstruct %s\n{" % (getDesc(s.description), s.name) )
        #def _typename(t):
        #    if t.startswith("enum "):
        #        return "hkEnum<%s, hkInt8>" % t.split()[1]
        #    return t
        #ret.append("/// Default constructor\n%s()\n{" % s.name)
        #for m in s.member:
        #    ret.append(_initcode(s.name, _membername(m.name), m.type, m.default))
        #ret.append("}")
        for m in s.member:
            genMember(m)
            #ret.append("%s %s;" % ( _typename(m.type), _membername(m.name)))
        ret.append("};\n")

    def genFile(file):
        """Generate the body of the file.
           Use this order to minimize the chance of dependency errors.
           (We should really use the definition order from the xml).
        """
        ret.append( file.includeheaders )
        for e in file.enum:
            genEnum(e)
        for c in file._class:
            genClass(c)

    def genDocument(doc):
        """Generate the document skeleton.
        """
        filename = doc.localfilename.replace("\\","/")
        ret.append(_tkbms % ((doc.file.platform or 'ALL'), (doc.file.product or 'PHYSICS'), (doc.file.visibility or 'PUBLIC')) )
        guard = filename.replace("/","_").replace(".","_").upper()+"_H"
        ret.append("#ifndef %s\n#define %s" % (guard, guard))
#        ret.append("\n// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.")
        ret.append("\n//\n// File auto-generated from '%s'\n//" % filename)
        genFile(doc.file)
        ret.append("#endif // %s\n" % (guard))

    genDocument( dom )
    return "\n".join(ret.lines)

#
#
#
    
def usage():
    import os
    import sys
    print 'Usage: %s <filename.hkclass>\n' % os.path.basename(sys.argv[0])
    print 'Generate a header from the specified xml class description'
    print 'Options:'
    print '  -h --help    print this help text'

def main():    
    import sys
    import hkcToDom
    import headerToDom
    def processfile(inputfilename, debug=0):
        """Return the text of a c++ header file from a given input file.
        """
        if inputfilename.endswith(".h"):
            dom = headerToDom.headerToDom(inputfilename)
        else:
            dom = hkcToDom.hkcToDom(inputfilename)
        return domToHeader(dom, debug)
        
    args = sys.argv[1:]
    if ("-h" in args) or ("--help" in args):
        usage()
        sys.exit(0)
    if len(args) < 1:
        usage()
        sys.exit(1)
    for arg in args:
        processfile(arg, 1)

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
