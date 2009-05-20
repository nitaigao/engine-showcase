#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###
### This script is used to generate an hkClass instance from an xml data binding.
### All the *class.cpp files in the havok tree are generated from this script.
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

import re
import sys
import hkcToDom
import headerToDom

havokTypes="""
			// plain old data types
			TYPE_BOOL, // "true", "false"
			TYPE_CHAR, // integers
			TYPE_INT8,
			TYPE_UINT8,
			TYPE_INT16,
			TYPE_UINT16,
			TYPE_INT32,
			TYPE_UINT32,
			TYPE_INT64,
			TYPE_UINT64,
			TYPE_REAL, // floating
			TYPE_VECTOR4,
			TYPE_QUATERNION,
			TYPE_MATRIX3,
			TYPE_ROTATION,
			TYPE_QSTRANSFORM,
			TYPE_MATRIX4,
			TYPE_TRANSFORM,
			TYPE_ZERO, // serialize as zero
			// complex types
			TYPE_POINTER, // generic pointer, see member flags for more info
			TYPE_FUNCTIONPOINTER,
			TYPE_ARRAY, // array
			TYPE_INPLACEARRAY, // inplace array
			TYPE_ENUM, // enumerated values
			TYPE_STRUCT, // simple struct
			TYPE_SIMPLEARRAY, // simple array (ptr(typed) and size only)
			TYPE_HOMOGENEOUSARRAY, // simple array of homogenous types, so is a class id followed by a void* ptr and size
			TYPE_CSTRING, // c-string, array of chars terminated by 0
			TYPE_ULONG,
			TYPE_VARIANT, // void* and hkClass*
			TYPE_FLAGS // named bitfield
"""

def _extractHavokTypes():
    return [ 'hk%s' % m.lower() for m in re.findall('^\s*TYPE_([^,]*)(?m)', havokTypes) ]
havokTypesList = _extractHavokTypes()
floatingPointTypes = "hkReal hkVector4 hkQuaternion hkMatrix3 hkRotation hkQsTransform hkMatrix4 hkTransform".split()
aggregateTypes = "hkVector4 hkQuaternion hkMatrix3 hkRotation hkQsTransform hkMatrix4 hkTransform".split()

def _readExtraTypes():
    import os
    fname = os.path.abspath( os.path.join( __file__, "..", "typeMapping.txt") )
    ret = {}
    try:
        for line in open(fname).read().split():
            key, value = [ x.strip() for x in line.split(":") ]
            ret[ key ] = value
    except IOError:
        pass
    return fname, ret

extraTypesFile, extraTypes = _readExtraTypes()

def _enumerate(l):
    """Workalike for the python2.3 builtin.
    """
    return zip(range(len(l)), l)

def removeFromDeclaration(decl, listToRemove):
    return "".join([x for x in decl.replace("*"," * ").replace("&"," & ").split() if x not in listToRemove])

def removeFromDeclarationSpaced(decl, listToRemove):
    return " ".join([x for x in decl.replace("*"," * ").replace("&"," & ").split() if x not in listToRemove])

def resolveExtraType(decl):
    """Get the enumerated type from a c++ type.
    """
    # the input decl must contains spaces among keywords (class, struct, union, enum), '*'/'&' chars, type name
    def resolveTypeNow(declWord):
        if declWord in extraTypes:
            return resolveTypeNow(removeFromDeclarationSpaced(extraTypes[declWord], ("const", "mutable")))
        else:
            return declWord

    return " ".join([resolveTypeNow(x) for x in decl.split()])

def _typecode(mtype):
    """Get the enumerated type from a c++ type.
    """
    mtype = mtype.strip()

    # template match
    for key, val in [
        ("hkArray<", "hkClassMember::TYPE_ARRAY"),
        ("hkInplaceArray<", "hkClassMember::TYPE_INPLACEARRAY"),
        ("hkSimpleArray<", "hkClassMember::TYPE_SIMPLEARRAY"),
        ("hkVariant ", "hkClassMember::TYPE_VARIANT"),
        ("hkEnum<", "hkClassMember::TYPE_ENUM"),
        ("hkFlags<", "hkClassMember::TYPE_FLAGS"),
        ("hkZero<", "hkClassMember::TYPE_ZERO"),
        ("hkHomogeneousArray<", "hkClassMember::TYPE_HOMOGENEOUSARRAY") ]:
        if mtype.startswith(key):
            return val

    # clean up and extract valuable type info only
    mtype = removeFromDeclarationSpaced( mtype, ("mutable","const") )
    mtype = mtype.split("[")[0]
    # try to resolve custom type and clean up
    mtype = resolveExtraType(mtype)
    mtype = mtype.replace(" *","*").replace(" &","&")

    # exact match
    try:
        return {
            "bool": "hkClassMember::TYPE_BOOL",
            "int": "hkClassMember::TYPE_INT32",
            "unsigned": "hkClassMember::TYPE_UINT32",
            "float": "hkClassMember::TYPE_REAL",
            "char": "hkClassMember::TYPE_CHAR",
            "char*": "hkClassMember::TYPE_CSTRING",
            "void": "hkClassMember::TYPE_VOID" }[mtype]
    except KeyError:
        pass

    # specials: pointer, enum, struct
    if mtype.endswith("*") or mtype.endswith("&"):
        return "hkClassMember::TYPE_POINTER"
    elif mtype.startswith("struct ") or mtype.startswith("class "):
        return "hkClassMember::TYPE_STRUCT"
    else:# mtype.startswith("hk"):
        if not mtype.lower() in havokTypesList:
            if not mtype in extraTypes:
                raise RuntimeError("'%s' not in Havok types list nor %s.\n" \
                                   "Maybe you meant 'class %s' or 'enum %s'?"% \
                                   (mtype, extraTypesFile, mtype, mtype))
            else:
                return _typecode(extraTypes[mtype])
        return "hkClassMember::TYPE_%s" % mtype[2:].upper()
    return ""

def _getReferencedEnum(e):
    """Get the name of the c++ reflection object for enum e.
    """
    return e.replace("enum ","").split("<",1)[-1].replace("::","").split(",")[0].strip()

def _getPointedClass(fullname):
    """Get the hkClass - must be type info only - no const or mutable
    """
    name = fullname.split("[")[0]
    if (name.endswith('*') or name.endswith('&')):
        # here we always expect '*' or '&' chars at the end of the declaration
        # so, strip the last char: '*' or '&'
        name = name[:-1]
    name = name.strip().split()[-1]
    if name in "void hkBool bool char char* float hkReal hkVector4 hkQuaternion hkMatrix3 " \
            "hkRotation hkQsTransform hkMatrix4 hkTransform".split():
        return None
    inttype = lambda(x) : [x+str(s) for s in [8,16,32,64]]
    if name in inttype("hkInt") or name in inttype("hkUint"):
        return None
    return name

def _add_single_dot_f(val):
    """Add .f to float values to avoid double->float warnings.
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
    """Add .fs to multiple values
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
    # 'a' do not always have '>' bracket at the end
    t = a.split("<")[-1].split(">")[0]
    if a.startswith("hkInplaceArray"):
        t = ",".join( a.split(",")[:-1] )
    return t

def _get_type_from_pointer(a):
    """Return true if this is pointer to struct.
    """
    try:
        a = resolveExtraType(a)
        a = a.replace(" *","*").replace(" &","&")
        t = _typecode( a[:-1] )
    except RuntimeError:
        return "hkClassMember::TYPE_STRUCT"
    try:
        # we do not support pointer to void and pointer to pointer types
        if t == "hkClassMember::TYPE_POINTER" or t == "hkClassMember::TYPE_VOID":
            errMsg =  "\nPointer to void and pointer to pointer types are not supported:\n\n\t%s\n" % a
            raise hkcToDom.HkcParseError(errMsg)
    except Exception:
        pass
    return t

def _infocode(type):
    """Get the 'extra' information associated with a member.
       For arrays this is the contained type.
       For enums this is the size of the enum.
    """
    firstLessThan = type.find("<")#XXX
    if type[:firstLessThan] in ("hkArray", "hkInplaceArray", "hkSimpleArray", "hkZero"):
        elem = type[firstLessThan+1:-1]
        if type[:firstLessThan] == "hkInplaceArray":
            elem = ",".join( elem.split(",")[:-1] )
        return _typecode(elem)
    elif type.startswith("enum "):
        try:
            size = int( type.split(":")[1] )
        except ValueError:
            raise RuntimeError("found an unsized enum %s" % type.name)
        return {8:1, 16:2, 32:4}[size]
    return ""

_pythonname = lambda x : x.replace("::","")

class struct:
    def __init__(self, **args):
        self.__dict__.update(args)

def _isFinishCtor(klass, method):
    return method.name == klass.name and len(method.parameter) == 1 and method.parameter[0].type == "hkFinishLoadedObjectFlag"

def _hasFinishCtor(klass):
    return True in [ _isFinishCtor(klass, m) for m in klass.method ]

def isGlobalType(pc):
    return pc.startswith("::") or pc.startswith("hk") or pc.startswith("hct") or pc.startswith("hcl")

def isForVersioning(klass):
    return klass.serializable

customAttributes = struct(imported={}, dictionary={})

def domToClass(dom, debug=0, collectAll=False, pchfile=""):
    """Generate a c++ file from the given input hkClass dom.
    """

    def _getInternalMembersName(namespace, klass):
        hasNonPublicMembers = False in [m.visibility in ("","public") for m in klass.member]
        where = ("%s::%s" % (namespace, klass.name)).lstrip(":")
        if hasNonPublicMembers and not collectAll:
            return "%s::Members" % where
        else:
            return "%sClass_Members" % where.replace("::","_")


    class ProgramList:
        """Class to format c++ code nicely.
        """
        def __init__(self):
            self.lines = []
            self.level = 0
            self.externs = {}
        def append(self, txt):
            for line in txt.split("\n"):
                self.level -= line.count("}")
                comment = self.level != 0 and line.find("///") != -1
                nline = "%s%s" % ((self.level + comment)* "\t", line)
                self.level += line.count("{")
                self.level += line.count("(")
                self.level -= line.count(")")
                if not self.externs.has_key(nline):
                    self.lines.append(nline)
                if nline.startswith("extern"):
                    self.externs[nline] = 1
                if debug: print nline
    ret = ProgramList()
    klasses = dict()

    def genMember(member, klass, namespace, notlast):
        """Generate a single member initializer.
           The fields of the initializer have different values depending
           on the type of the member. See Base/Reflection/hkClassMember.h for details.
        """
        def usesInternalType(klass, mclass):
            return klass.name not in ["hkClass", "hkClassMember", "hkClassEnum", "hkClassEnumItem"]\
            and mclass in ["hkClassClass", "hkClassMemberClass", "hkClassEnumClass", "hkClassEnumItemClass"]\
            and klass.name not in ["hkxSparselyAnimatedEnum", "hctAttributeDescription"] ### for compatibility, COM-434

        def getStructClass(mem):
            pc = _getPointedClass(mem)
            ret = "HK_NULL"
            structklass = None
            if pc:
                if collectAll:
                    scopelist = ("%s::%s" % (klass.full_scope(), klass.name)).lstrip(":").split("::") # make list of names of 'pc' possible scope
                    scopelist.append(pc.lstrip(":")) ## 'pc' may contain '::' in the name
                    while len(scopelist):
                        structname = _pythonname("".join(scopelist))
                        if klasses.has_key(structname):
                            structklass = klasses[structname]
                            break
                        del scopelist[len(scopelist)-2]
                    if not structklass:
                        print >>sys.__stderr__, "Class '%s' for member in '%s' is not found in class list and it is impossible to check if the class serializable."%(pc,klass.name)
                        raise KeyError

                if not collectAll or isForVersioning(structklass):
                    if isGlobalType(pc):
                        ret = "&%sClass" % _pythonname(pc)
                    else:
                        ret = "&%s%sClass" % (namespace or klass.name, _pythonname(pc))
            return ret

        #
        # Overwrite the original value of member.type if overridetype is specified.
        #
        if member.overridetype:
            member_type = member.overridetype
        else:
            member_type = member.type

        # extract (member.name, mclass, menum, mtype, msubtype, marraysize, mflags, moffset )

        mclass = "HK_NULL"
        menum = "HK_NULL"
        mtype = _typecode(member_type)
        mattr = "HK_NULL"
        if not collectAll and member.attributes:
            mattr = "&%s%s_%s_Attributes" % (namespace,klass.name,member.name)
        msubtype = "hkClassMember::TYPE_VOID"
        marraysize = 0

        # Get cstyle array size
        if member_type.find("[") != -1:
            member_type, rest = member_type.split("[")
            marraysize = rest.split("]")[0]
            member_type += rest.split("]")[-1] # handle array zeros: hkZero<int[3]>

        if mtype in ("hkClassMember::TYPE_ARRAY", "hkClassMember::TYPE_INPLACEARRAY", "hkClassMember::TYPE_ZERO", "hkClassMember::TYPE_SIMPLEARRAY"):
            msubtype = _infocode( member_type )
            if (msubtype == "hkClassMember::TYPE_POINTER" or msubtype == "hkClassMember::TYPE_STRUCT") and (member.flags.find("POINTER_VOIDSTAR") == -1):
                memClass = _get_array_type( member_type )
                mclass = getStructClass( removeFromDeclarationSpaced(memClass, ("const","class","struct")) )
        elif mtype == "hkClassMember::TYPE_POINTER" or mtype == "hkClassMember::TYPE_STRUCT":
            if mtype == "hkClassMember::TYPE_POINTER":
                msubtype = _get_type_from_pointer( removeFromDeclarationSpaced(member_type, ("mutable","const")) )
            if (mtype == "hkClassMember::TYPE_STRUCT" or msubtype == "hkClassMember::TYPE_STRUCT"):
                mclass = getStructClass( removeFromDeclarationSpaced(member_type, ("mutable","const","class","struct")) )
        elif mtype in ("hkClassMember::TYPE_ENUM", "hkClassMember::TYPE_FLAGS"):
            etype, esize = [x.strip() for x in member_type.split("<")[1].split(">")[0].split(",")]
            msubtype = _typecode( esize )
            if etype.startswith("enum "):
                etype = etype[5:].strip()
            if etype == "void":
                pass
            elif etype.find("::") != -1: #explicitly scoped
                menum = "%sEnum" % _getReferencedEnum(etype)
            else:
                menum = None
                # local scope
                for e in klass.enum:
                    if etype == e.name:
                        menum = "%s%s%sEnum" % (namespace.replace("::",""), klass.name, e.name)
                        break
                # enclosing class scope
                if menum==None:
                    for c in [ i for i in dom.file._class if i.name == namespace]:
                        for e in c.enum:
                            if etype == e.name:
                                menum = "%s%sEnum" % (c.name, e.name)
                                break
                # file scope
                if menum==None:
                    for i,e in _enumerate(dom.file.enum):
                        if etype == e.name:
                            menum = "%sEnum" % e.name
                            break
                assert menum != None
        where = ("%s::%s" % (namespace, klass.name)).lstrip(":")
        if member.overrideoffset: # hack for taggedunion - remove it
            offset = member.overrideoffset
        elif mtype == "hkClassMember::TYPE_HOMOGENEOUSARRAY": # offset is to the Type member
            offset = "HK_OFFSET_OF(%s,m_%sClass)" % (where, member.name)
        else:
            offset = "HK_OFFSET_OF(%s,m_%s)" % (where, member.name)
        if collectAll:
            offset = "0"
        flags = " | ".join( [ "%s" % x for x in member.flags.split() ] ) or "0"
        if member_type.startswith("enum ") and flags == "0":
            flags = "hkClassMember::ENUM_%i" % esize
        if member.align != 0:
            assert member.align in (8,16), "Only 8 & 16 byte alignment handled"
            flags += "|hkClassMember::ALIGN_%i"%member.align
        flags += "|hkClassMember::NOT_OWNED" if not member.owned else ""
        flags += "|hkClassMember::SERIALIZE_IGNORED" if not member.serialized else ""
        if usesInternalType(klass, mclass.replace("&","")):
            raise RuntimeError("\n%s::m_%s uses internal type %s\n"%(klass.name, member.name, mclass.replace("&","")))
        ret.append('\t{ "%s", %s, %s, %s, %s, %s, %s, %s, %s }%s' % (member.name, mclass, menum, mtype, msubtype, marraysize, flags, offset, mattr, notlast and "," or "") )

    def genAllMembers(members, klass, namespace, klasses):
        """Generate the data block for all members.
           We have to do some extra work to make sure all objects
           we use are declared before use.
        """
        members = [m for m in members if m.reflected]
        if len(members):
            membersName = _getInternalMembersName(namespace, klass)
            qualifiers = ["", "static "][ membersName.find("::")==-1 ]
            if not collectAll:
                qualifiers += "const "
            ret.append("%shkInternalClassMember %s[] =\n{" % (qualifiers,membersName))
            for i,m in _enumerate(members):
                genMember(m, klass, namespace, i!=len(members)-1)
            ret.append("};")

    def genDefaults(members, klass, namespace):
        defaults = []
        def zero(d):
            return d in (None, 0, "0", "false", "0.0", "0.0f", "HK_NULL")
        for i,m in _enumerate(members):
            if not zero(m.default):
                mtype = m.type.split("[",1)
                carray = len(mtype)==2 and ("[%s"%mtype[1]) or ""
                mtype = mtype[0]
                mtype = re.sub("(struct\s+)([^:])", r"\1%s%s::\2" % (namespace, klass.name), mtype)
                mtype = re.sub("hkZero<(.+)>", r"\1", mtype)
                mtype = re.sub("hkSimpleArray<(.+)>", r"\tstruct { void* p; int s; }", mtype)
                mtype = re.sub("hkHomogeneousArray", r"\tstruct { void* p; void* c; int s; }", mtype)
                mtype = mtype.replace("enum enum", "enum")
                mtype = mtype.replace("hkBool", "_hkBool")
                if m.type in aggregateTypes:
                    mtype = "_"+mtype
                if mtype.startswith("hkEnum<") or mtype.startswith("hkFlags<"):
                    enumstore = m.sourcecode.split(",")[-1].split(">")[0].strip()
                    mtype = enumstore + " /* %s */"%mtype
                decl = "%s m_%s%s;" % (mtype, m.name, carray)
                init = m.default
                #if m.type in floatingPointTypes:
                    #init = _add_dot_fs(init)
                if m.type in aggregateTypes:
                    init = "\t{%s}" % ",".join(init.split())
                defaults.append( (decl,init) )
        if len(defaults):
            ret.append("namespace\n{")
            pyscope = "%s%s" % (namespace, klass.name)
            dstructtype = "%s_DefaultStruct" % pyscope
            ret.append("struct %s\n{" % dstructtype )
            ret.append("int s_defaultOffsets[%i];" % len(members))
            # some compilers cannot handle constants of these types so we fake it.
            ret.append("typedef hkInt8 _hkBool;")
            ret.append("typedef hkReal _hkVector4[4];")
            ret.append("typedef hkReal _hkQuaternion[4];")
            ret.append("typedef hkReal _hkMatrix3[12];")
            ret.append("typedef hkReal _hkRotation[12];")
            ret.append("typedef hkReal _hkQsTransform[12];")
            ret.append("typedef hkReal _hkMatrix4[16];")
            ret.append("typedef hkReal _hkTransform[16];")
            for d,i in defaults:
                ret.append(d)
            ret.append("};")
            ret.append("const %s %s_Default =\n{" % (dstructtype, pyscope));
            ret.append( "\t{%s}," % ",".join( [ ("HK_OFFSET_OF(%s,m_%s)" % (dstructtype,m.name),"-1")[ zero(m.default) ] for m in members ] ) )
            ret.append(",".join([d[1] for d in defaults]))
            ret.append("};")
            ret.append("}")
            return "&%s_Default," % pyscope
        else: # no defaults
            return 'HK_NULL, // defaults'

    def genAttributes(members, klass, namespace):
        def genAttribute(attributes, symbol, klass, member):
            ret.append("// %s attributes" % symbol)
            attrs = []
            for attrname,val in attributes.items():
                if attrname.find(".") != -1:
                    mod = attrname.rsplit(".",1)[0]
                    customAttributes.dictionary.update( __import__("attributes."+mod).__dict__ )
            for attrname,val in attributes.items():
                keysym = "%s_%s" % (symbol, attrname.replace(".",""))
                try:
                    generator = eval(attrname,customAttributes.dictionary)(symbol=keysym, member=member, klass=klass, caller="CPP")
                except KeyError:
                    print >>sys.stderr, "Attribute '%s' was not found." % (attrname, )
                else:
                    code = eval("generator(%s)" % val, None, dict(generator=generator, true=True, false=False) )
                    if code:
                        ret.append( code )
                        k = generator.__class__
                        attrs.append( (attrname, keysym, k.__module__.replace("attributes","").replace(".","") + k.__name__) )
            if len(attrs):
                varname = "%s_Attributes" % symbol
                ret.append("static const hkInternalCustomAttributes::Attribute %sList[] =\n{" % (varname) )
                ret.append( ",\n".join( '\t{ "%s", &%s, &%sClass }' % attr for attr in attrs ) )
                ret.append("};")
                ret.append("static const hkInternalCustomAttributes %s = { %sList, HK_COUNT_OF(%sList) };" % (varname, varname, varname) )
                return "reinterpret_cast<const hkCustomAttributes*>(&%s)," % varname
            return 'HK_NULL, // attributes'

        for member in members:
            if member.attributes:
                symbol = "%s%s_%s" % (namespace, klass.name, member.name)
                genAttribute( member.attributes, symbol, klass, member)
        if klass.attributes:
            symbol = "%s%s" % (namespace, klass.name)
            return genAttribute( klass.attributes, symbol, klass, None)
        return 'HK_NULL, // attributes'

    def genAllExterns(classList):
        declarations = []
        def doExterns(classList, namespace):
            for klass in classList:
                klasses[_pythonname("%s%s"%(namespace,klass.name))] = klass
                if not collectAll or isForVersioning(klass):
                    # declare pointer types used
                    for i,m in _enumerate(klass.member):
                        mtype = m.type.split("[")[0].strip()
                        mtype = removeFromDeclarationSpaced(mtype, ("mutable","const"))
                        #mtype = " ".join( [b for b in m.type.split() if b!="const"] )
                        if mtype.endswith("*") or mtype.startswith("class ") or mtype.startswith("struct "):
                            if (mtype.endswith("*") and _get_type_from_pointer(mtype) != "hkClassMember::TYPE_STRUCT"):
                                pc = None
                            elif mtype.find("<") != -1:
                                pc = None
                            else:
                                pc = _getPointedClass(mtype)
                            #print (mtype,pc)
                            if pc:
                                cname = "%s%s" % ("" if isGlobalType(pc) else klass.name, pc)
                                declarations.append("extern const hkClass %sClass;" % (cname))
                        elif (mtype.startswith("hkEnum<") or mtype.startswith("hkFlags<")) and mtype.find("::") != -1:
                            declarations.append("extern const hkClassEnum* %sEnum;" % (_getReferencedEnum(mtype)) )
                        elif (mtype.startswith("hkArray<") or mtype.startswith("hkSimpleArray<") or mtype.startswith("hkInplaceArray<")):
                            pc = _getPointedClass(removeFromDeclarationSpaced(_get_array_type(mtype), ("*","&")))
                            #print pc
                            if pc:
                                cname = "%s%s" % ("" if isGlobalType(pc) else klass.name, pc)
                                declarations.append("extern const hkClass %sClass;" % (cname) )
                doExterns(klass._class, klass.name)
        doExterns(classList, "")
        if len(declarations):
            ret.append("\n// External pointer and enum types")
            declarations.sort()
            lastd = None
            for d in declarations:
                if d != lastd:
                    r = d.replace("::","")
                    if collectAll and r.find("hkClassEnum*") == -1:
                        r = r.replace(" const "," ")
                    ret.append(r)

##    def genConstructor(klass):
##        """Generate the constructor for the class.
##        """
##        # declare arrays of nonzero defaults
##        def canonicalify(s):
##            return ", ".join(s.replace("{"," { ").replace("}", " } ").split()).replace("{,","{").replace(", }"," }")
##        # define any static arrays used in initialization
##        for m in klass.member:
##            if m.default and m.default.strip():
##                default = m.default.strip()
##                if m.type.find("hkArray") != -1:
##                    assert default[0]=="(" and default[-1]==")" and "Array default must be bracketed with ()"
##                    default = default[1:-1]
##                    atype = _get_array_type(m.type)
##                    if atype in ("hkVector4", "hkQuaternion", "hkMatrix3", "hkRotation", "hkMatrix4", "hkTransform"):
##                        struct = "Struct"
##                    elif atype.startswith("struct") and atype.find("::") == -1:
##                        atype = atype.replace("struct ", "struct %s::" % klass.name)
##                        struct = ""
##                    else:
##                        struct = ""
##                    ret.append("static const %s%s %s%sInit[] = { %s };" % (atype, struct, klass.name, m.name, canonicalify(default)))
##                elif m.type.find("[") != -1:
##                    atype, nelem = m.type.split("[")
##                    nelem = int( nelem.split("]")[0] )
##                    if default.startswith("("):
##                        default = default[1:-1]
##                    if atype in ("hkVector4", "hkQuaternion", "hkMatrix3", "hkRotation", "hkMatrix4", "hkTransform"):
##                        struct = "Struct"
##                    else:
##                        struct = ""
##                    ret.append("static const %s%s %s%sInit[%i] = { %s };" % (atype, struct, klass.name, m.name, nelem, canonicalify(default)))
##        # define constructor
##        ret.append("\n%s::%s()\n{" % (klass.name, klass.name))
##
##        # output member initializers
##        for m in klass.member:
##            ret.append( _initcode(klass.name, m.name, m.type, m.default))
##
##        ret.append("}\n")

    def genClass(klass, namespace):
        """Generate a single class.
        """
        def genReflectionDefinition(klass, namespace=""):
            if klass.name == "hkBaseObject" or klass.abstract:
                return ""
            reflectiondefine = "HK_REFLECTION_DEFINE"
            if namespace:
                reflectiondefine = reflectiondefine + "_SCOPED"
            reflectiondefine = reflectiondefine + ((klass.vtable and "_VIRTUAL") or (_hasFinishCtor(klass) and "_NONVIRTUAL") or "_SIMPLE")
            params = klass.name
            if namespace:
                params = "%s,%s"%(namespace,params)
            return "%s(%s);"%(reflectiondefine,params)

        namespace_klass_name = ("%s::%s" % (namespace, klass.name)).lstrip(":")

        # nested enums and structs
        if klass.reflected and (not collectAll or isForVersioning(klass)):
            genEnums(klass.enum, namespace_klass_name)
        for s in klass._class:
            genClass(s, namespace_klass_name)

        # check if klass is up to the requirements
        if not klass.reflected or (collectAll and not isForVersioning(klass)):
            return

        ret.append("\n//\n// Class %s\n//" % namespace_klass_name)

        # members
        attributes = genAttributes(klass.member, klass, namespace) if not collectAll else 'HK_NULL, // attributes'
        genAllMembers(klass.member, klass, namespace, klasses)
        defaults = genDefaults(klass.member, klass, namespace)

        numInterfaces = len(klass.interface)
        if not klass.parent and klass.vtable:
            numInterfaces = 1

        const = ["const ",""][ collectAll ]
        if klass.parent:
            parent = "&%sClass" % (_pythonname(klass.parent))
            ret.append("extern %shkClass %s;\n" % (const, parent[1:]))
        else:
            parent = "HK_NULL"

        ret.append('extern %shkClass %sClass;' % (const, _pythonname(namespace_klass_name)) )
        ret.append('%shkClass %sClass(' % (const, _pythonname(namespace_klass_name)) )
        scoped_klass_name = namespace_klass_name.replace(':','')
        ret.append('\"%s\",' % scoped_klass_name)
        ret.append('%s, // parent' % parent)

        if not collectAll:
            ret.append('sizeof(%s),' % (namespace_klass_name))
        else:
            ret.append('0,')
        #ret.append(numInterfaces and ('%sClass_Interfaces,' % pythonname(namespace_klass_name)) or "HK_NULL,")
        ret.append("HK_NULL,")
        ret.append('%i, // interfaces' % numInterfaces )
        visible_enums = [k for k in klass.enum if k.reflected]
        ret.append(len(visible_enums) and ('reinterpret_cast<const hkClassEnum*>(%sEnums),' % _pythonname(namespace_klass_name)) or "HK_NULL,")
        ret.append('%s, // enums' % len(visible_enums) )
        ret.append(len(klass.member) and ('reinterpret_cast<const hkClassMember*>(%s),' % _getInternalMembersName(namespace,klass)) or "HK_NULL,")
        ret.append(len(klass.member) and ('HK_COUNT_OF(%s),' % _getInternalMembersName(namespace,klass)) or "0,")
        ret.append(defaults)
        ret.append(attributes)
        ret.append("0, // flags" if klass.serializable else "hkClass::FLAGS_NOT_SERIALIZABLE,")
        ret.append("%i // version" % klass.version )
        ret.append(');')
        if not collectAll:
            ret.append(genReflectionDefinition(klass, namespace))

    def genEnums(enums, namespace):
        enums = [e for e in enums if e.reflected]
        if namespace == "":
            localname = dom.localfilename.replace("\\","_").replace("/","_").replace(":","_").split(".")[0]
        else:
            localname = _pythonname(namespace)
        def itemsName(e):
            return "%s%sEnumItems" % (localname, e.name)
        for enum in enums:
            ret.append("\n//\n// Enum %s::%s\n//" % (namespace, enum.name) )
            ret.append("static const hkInternalClassEnumItem %s[] =\n{" % itemsName(enum) )
            for i in enum.item:
                value = str(i.value) 
                value += "u" if i.value >= (1<<31) else ""
                if i.initializer and i.initializer != str(i.value):
                    value += "/*%s*/" % i.initializer
                ret.append("\t{%s, \"%s\"}," % (value, i.name))
            ret.append("};")
        if len(enums):
            ret.append('static const hkInternalClassEnum %sEnums[] = {' % localname)
            for i,e in _enumerate(enums):
                comma = (",","")[i+1==len(enums)]
                ret.append('\t{"%s", %s, %i, HK_NULL, 0 }%s' % (e.name, itemsName(e), len(e.item), comma) )
            ret.append('};')
            for i,e in _enumerate(enums):
                ret.append('extern const hkClassEnum* %s%sEnum = reinterpret_cast<const hkClassEnum*>(&%sEnums[%i]);' % (namespace.replace(":",""), e.name, localname, i))

    def genFile(file):
        filename = dom.localfilename.replace("\\","/")
        if not collectAll:
            ret.append("\n// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.")
            ret.append("// Generated from '%s'" % filename)
            ret.append("#include <%s>" % (pchfile or dom.pchfile or "Common/Base/hkBase.h"))
            ret.append("#include <Common/Base/Reflection/hkClass.h>")
            ret.append("#include <Common/Base/Reflection/hkInternalClassMember.h>")
            ret.append("#include <Common/Base/Reflection/hkTypeInfo.h>")
            ret.append("#include <%s>" % filename.replace(".hkclass", ".h") )
            ret.append("" if not file.overridedestination else 'HK_REFLECTION_CLASSFILE_HEADER("%s");\n' % filename )
        # should really use xml file decl order here
        ret.append("")
        if len(file.enum) or len(file.struct):
            ret.append("//\n// Global\n//")
        genEnums(file.enum + file.taggedunion, "")
        allClasses = file._class[:]
        for u in file.taggedunion:
            ret.append("// %s" % u.name)
            klass = hkcToDom.Class()
            klass.name = u.name
            klass.enum.append( u )
            klass.reflected = True
            def genMember(i):
                m = hkcToDom.Member()
                m.name = i.name
                m.type = "class %s*" % i.klass
                m.overrideoffset = "0"
                return m
            klass.member += [ genMember(i) for i in u.item if i.klass!="HK_NULL" ]
            allClasses.append( klass )
        genAllExterns(allClasses)
        for c in allClasses:
            genClass(c, "")

    def genDocument(doc):
        """Generate the skeleton.
        """
        if not collectAll:
            platform = re.sub("[^\s]*SPU[^\s]*", "", doc.file.platform) #HACK
            ret.append(_tkbms % ((platform or 'ALL'), (doc.file.product or 'PHYSICS'), (doc.file.visibility or 'PUBLIC')) )
        genFile(doc.file)
        ret.append("")

    genDocument(dom)
    return "\n".join(ret.lines)

def usage():
    import os
    import sys
    print 'Usage: %s <filename.hkclass>\n' % os.path.basename(__file__)
    print 'Generate a source file from the specified xml class description'
    print 'Options:'
    print '  -h --help    print this help text'

def main():
    def processfile(inputfilename, debug=0):
        """Convenience function to create a class from an xml class file.
        """
        if inputfilename.endswith(".h"):
            dom = headerToDom.headerToDom(inputfilename)
        else:
            dom = hkcToDom.hkcToDom(inputfilename)
        return domToClass(dom, debug)
    args = sys.argv[1:]
    if ("-h" in args) or ("--help" in args):
        usage()
        sys.exit(0)
    if len(args) < 1:
        usage()
        sys.exit(1)
    for arg in args:
        try:
            processfile(arg, 1)
        except Exception, e:
            print "Error parsing '%s'\n%s" % (arg, e)
            break

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
