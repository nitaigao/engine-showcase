#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###
### This script is used to generate .NET class wrappers in managed C++ from an xml data
### binding.
### By default the script generates the hkHavokManagedClasses.h/.cpp files in the havok tree,
### unless different name is specified.
###
### This script can be either be executed directly or imported
###

_tkbms = """// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM   : WIN32
// PRODUCT    : %s
// VISIBILITY : %s
//
// ------------------------------------------------------TKBMS v1.0"""

import sys
import hkcToDom
import headerToDom
import domToClass

### managed class
classNamespace = "Havok"
classDeclSpec = "HK_MANAGED_CLASS"
classDeclAbstractSpec = "HK_MANAGED_ABSTRACT_CLASS"
classBaseSpec = "HK_MANAGED_CLASS_BASE"
classDefSpec = "_DEFINE"
classSpecDict = {   ### classSpecKey : (classSpecPostfix, classSpecParams)
                    "base" : ("","(%s,%s)"),                  ### HK_MANAGED_CLASS
                    "derived" : ("_DERIVED","(%s,%s,%s)"),    ### HK_MANAGED_CLASS_DERIVED
                    "derived_scoped" : ("_DERIVED_SCOPED","(%s,%s,%s,%s)"),    ### HK_MANAGED_CLASS_DERIVED_SCOPED
                    "scoped" : ("_SCOPED","(%s,%s,%s)")       ### HK_MANAGED_CLASS_SCOPED
                }

attrDefault = "/* no attributes */"
attrIgnore = None
attrExclude = -1
attrDeclSpec = "HK_MANAGED_ATTRIBUTE_CLASS"
attrBaseSpec = "HK_MANAGED_ATTRIBUTE_CLASS_BASE"
attrDefSpec = "_DEFINE"
attrSpecDict =  {   ### classSpecKey : (classSpecPostfix, classSpecParams)
                    "base" : ("","(%s)"),                  ### HK_MANAGED_ATTRIBUTE_CLASS
                    "derived" : ("_DERIVED","(%s,%s)"),    ### HK_MANAGED_ATTRIBUTE_CLASS_DERIVED
                    "derived_scoped" : ("_DERIVED_SCOPED","(%s,%s,%s)"),    ### HK_MANAGED_ATTRIBUTE_CLASS_DERIVED_SCOPED
                    "scoped" : ("_SCOPED","(%s,%s)")       ### HK_MANAGED_ATTRIBUTE_CLASS_SCOPED
                }

### managed class properties
classPropertySpec = "HK_PROPERTY"
classPropertyDefSpec = "_DEF"
classPropertySpecDict = { ### propertySpecKey : (propertySpecPostfix, propertySpecParams, propertyAttrs)
                            "plain" : ("","(%s,%s,%s)"),
                            "enum" : ("_ENUM", "(%s,%s,%s,%s)"),
                            "flags" : ("_FLAG", "(%s,%s,%s,%s)"),
                            "cstring" : ("_STRING","(%s,%s)"),
                            "plain_havok" : ("_PROXY","(%s,%s,%s)"),
                            "variant" : ("_VARIANT","(%s,%s)"),
                            "struct" : ("_STRUCT","(%s,%s,%s)"), ### copy by value
                            "pointer_struct" : ("_CLASS","(%s,%s,%s)"), ### copy by reference
                            "zero" : ("_ZERO","(%s,%s)"),
                            "pointer" : ("_PTR","(%s,%s,%s)"),
                            "simplearray" : ("_SIMPLEARRAY","(%s,%s,%s)"),
                            "simplearray_cstring" : ("_SIMPLEARRAY_STRING","(%s,%s)"),
                            "simplearray_plain_havok" : ("_SIMPLEARRAY_PROXY","(%s,%s,%s)"),
                            "simplearray_variant" : ("_SIMPLEARRAY_VARIANT","(%s,%s)"),
                            "simplearray_struct" : ("_SIMPLEARRAY_STRUCT","(%s,%s,%s)"),
                            "simplearray_pointer" : ("_SIMPLEARRAY_CLASS","(%s,%s,%s)"),
                            "array" : ("_ARRAY","(%s,%s,%s)"),
                            "array_cstring" : ("_ARRAY_STRING","(%s,%s)"),
                            "array_plain_havok" : ("_ARRAY_PROXY","(%s,%s,%s)"),
                            "array_variant" : ("_ARRAY_VARIANT","(%s,%s)"),
                            "array_struct" : ("_ARRAY_STRUCT","(%s,%s,%s)"),
                            "array_pointer" : ("_ARRAY_CLASS","(%s,%s,%s)"),
                            "pointer_plain" : ("_NOT_SPECIFIED","(%s,%s,%s)"),
                            "pointer_plain_havok" : ("_NOT_SPECIFIED","(%s,%s,%s)"),
                            "pointer_variant" : ("_NOT_SPECIFIED","(%s,%s,%s)"),
                            "none" : ("_NOT_SPECIFIED","(%s,%s,%s)")
                         }

typeToClassPropertySpecKeyDict = {
                                    "TYPE_VOID" : "none",
                                    "TYPE_BOOL" : "plain",
                                    "TYPE_CHAR" : "plain",
                                    "TYPE_INT8" : "plain",
                                    "TYPE_UINT8" : "plain",
                                    "TYPE_INT16" : "plain",
                                    "TYPE_UINT16" : "plain",
                                    "TYPE_INT32" : "plain",
                                    "TYPE_UINT32" : "plain",
                                    "TYPE_INT64" : "plain",
                                    "TYPE_UINT64" : "plain",
                                    "TYPE_REAL" : "plain", ### floating
                                    "TYPE_VECTOR4" : "plain_havok",
                                    "TYPE_QUATERNION" : "plain_havok",
                                    "TYPE_MATRIX3" : "plain_havok",
                                    "TYPE_ROTATION" : "plain_havok",
                                    "TYPE_QSTRANSFORM" : "plain_havok",
                                    "TYPE_MATRIX4" : "plain_havok",
                                    "TYPE_TRANSFORM" : "plain_havok",
                                    "TYPE_ZERO" : "zero", ### serialize as zero
                                    ### complex types
                                    "TYPE_POINTER" : "pointer", ### generic pointer, see member flags for more info
                                    "TYPE_FUNCTIONPOINTER" : "none",
                                    "TYPE_ARRAY" : "array", ### array
                                    "TYPE_INPLACEARRAY" : "array", ### inplace array
                                    "TYPE_ENUM" : "enum", ### enumerated values
                                    "TYPE_STRUCT" : "struct", ### simple struct
                                    "TYPE_SIMPLEARRAY" : "simplearray", ### simple array (ptr(typed) and size only)
                                    "TYPE_HOMOGENEOUSARRAY" : "none", ### simple array of homogenous types, so is a class id followed by a void* ptr and size 
                                    "TYPE_CSTRING" : "cstring", ### c-string, array of chars terminated by 0
                                    "TYPE_ULONG" : "plain",
                                    "TYPE_VARIANT" : "variant", ### void* and hkClass*
                                    "TYPE_FLAGS" : "flags", ### named bitfield
                                }

classEnumDeclSpec = ("HK_MANAGED_ENUM", "(%s)")
classEnumValueDeclSpec = ("_VALUE","(%s,%s)")

classFlagDeclSpec = ("HK_MANAGED_FLAG", "(%s)")
classFlagValueDeclSpec = ("_VALUE","(%s,%s)")

classCtorDeclSpec = "HK_MANAGED_CLASS_CTOR"
classCtorStartDefSpec = "_DEFINE_START"
classCtorEndDefSpec = "_DEFINE_END"
classCtorSpecDict = {   ### classCtorSpecKey : (classCtorSpecPostfix, classCtorSpecParams)
                    "base" : ("","(%s,(%s))"),                  ### HK_MANAGED_CLASS_CTOR
                    "derived" : ("_DERIVED","(%s,(%s),%s)"),    ### HK_MANAGED_CLASS_CTOR_DERIVED
                    "derived_scoped" : ("_DERIVED_SCOPED","(%s,%s,(%s),%s)"),    ### HK_MANAGED_CLASS_CTOR_DERIVED_SCOPED
                    "scoped" : ("_SCOPED","(%s,%s,(%s))"),       ### HK_MANAGED_CLASS_CTOR_SCOPED
                    "end" : ("","()")       ### HK_MANAGED_CLASS_CTOR_SCOPED
                }

def getInternalSpecKey(havokType, havokSubtype):
    specKey1 = typeToClassPropertySpecKeyDict[ havokType.replace("hkClassMember","").replace(":","") ]
    specKey2 = typeToClassPropertySpecKeyDict[ havokSubtype.replace("hkClassMember","").replace(":","") ]
    if specKey2=="none":
        specKey2=""
    else:
        if specKey1=="pointer" or specKey2 in [ "pointer", "cstring", "variant", "struct", "plain_havok" ]:
            specKey2="_"+specKey2
        else:
            specKey2=""
    return "".join([specKey1,specKey2])

    __slots__ = ("level")
    def __init__(self, level, msg):
        self.level = level
        UserWarning.__init__(self, msg)

class FormatedSourceCode:
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

def getScopedName(className, namespace=""):
    return ("%s::%s" % (namespace,className)).lstrip(":")

def getScopeNoName(classFullName):
    if classFullName.count("::"):
        return "::".join( classFullName.split("::")[:-1] )
    return ""

def getMapOfScopedClasses(startClass, scopedClassFromNameMap_IO, namespace=""):
    fullscope = getScopedName(startClass.name, namespace)
    for klass in startClass._class:
        getMapOfScopedClasses(klass, scopedClassFromNameMap_IO, fullscope)
    scopedClassFromNameMap_IO[fullscope] = startClass
    return scopedClassFromNameMap_IO

def getMapOfScopedEnums(startClass, enumFromFullNameMap_IO, namespace=""):
    #print startClass.name
    fullClassScope = getScopedName(startClass.name, namespace)
    for e in startClass.enum:
        fullscope = getScopedName(e.name, fullClassScope)
        enumFromFullNameMap_IO[fullscope] = e
        #print "\t",fullscope
    for klass in startClass._class:
        getMapOfScopedEnums(klass, enumFromFullNameMap_IO, fullClassScope)
    return enumFromFullNameMap_IO

def isLocalScopeType(fullTypeName, fullNamespace):
    return fullTypeName.split("::")[0] == fullNamespace.split("::")[0]

def getClassEntriesList(typeName, classList):
    retList = []
    partTypeNameList = typeName.split("::")
    lenTypeNameList = len(partTypeNameList)
    for cl in classList:
        l = cl.split("::")
        delta = len(l) - lenTypeNameList
        if delta >= 0:
            ret = [1 for i in range(lenTypeNameList-1, -1, -1) if partTypeNameList[i]==l[i+delta]]
            if len(ret)==lenTypeNameList:
                retList.append(cl)
    return retList

def getFullTypeName(typeName, className, namespace, classList):
    if typeName.startswith("::"):
        #print "GLOBAL"
        globalType = typeName.lstrip(":")
        if globalType in classList:
            return globalType
        else:
            print "UNKNOWN GLOBAL", typeName
            return typeName
    else:
        #print "GET ALL"
        scoped = getScopedName(typeName, getScopedName(className, namespace))
        if scoped in classList:
            return scoped
        scoped = getScopedName(typeName, className)
        if scoped in classList:
            return scoped
        tList = getClassEntriesList(typeName, classList)
        if len(tList):
            scope = getScopedName(typeName, className)
            #print "GET WITH CLASS SCOPE", scope, tList
            cList = getClassEntriesList(scope, tList)
            if len(cList) == 1:
                return scope
            else:
                if len(cList):
                    scope = getScopedName(typeName, getScopedName(className, namespace))
                    #print "GET WITH NAMESPACE::CLASS SCOPE", scope, cList
                    cList = getClassEntriesList(scope, cList)
                    if len([1 for c in cList if c==scope]) == 1:
                        return scope
                #print "GET WITH NAMESPACE/PARTS SCOPES", tList
                classMap = dict([(n,i) for i,n in enumerate(tList)])
                subname = namespace.split("::")
                while 1:
                    tmpType = getScopedName(typeName, "::".join(subname))
                    if classMap.has_key(tmpType):
                        return tmpType
                    if len(subname):
                        subname = subname[:-1]
                    else:
                        break
                print "ERROR", cList, scope
                return typeName
        else:
            print "UNKNOWN", typeName
    return typeName

class ManagedMemberInfo:
    """Class to describe memeber of a managed class.
    """
    __slots__ = (
        "name", "dataType", "havokType", "havokSubtype",
        "description", "visibility", "size", "typeSize", "externalDataType", "attributes", "hasDefault" )
    def __init__(self, domMember, ownerClass, namespace, allKnownClassNamesList):
        self.updateWithDomMember(domMember, ownerClass, namespace, allKnownClassNamesList)
    def updateWithDomMember(self, domMember, ownerClass, namespace, allKnownClassNamesList):
        """Generate a single member initializer.
           The fields of the initializer have different values depending
           on the type of the member.
        """
        def getStructClass(mem):
            pc = domToClass._getPointedClass(mem)
            ret = "HK_NULL"
            if pc:
                if pc.startswith("::"):
                    ret = pc[2:]
                else:
                    ret = pc
            return ret
        ###
        self.visibility = domMember.visibility
        self.description = domMember.description
        self.name = "m_%s" % domMember.name
        self.dataType = domToClass.removeFromDeclarationSpaced( domMember.type, ("mutable","const") )
        self.externalDataType = False
        self.isStruct = False
        self.hasDefault = domMember.default != None
        self.havokType = "TYPE_ZERO"
        self.havokSubtype = "hkClassMember::TYPE_VOID"
        self.typeSize = "sizeof(%s)" % self.dataType
        self.attributes = domMember.attributes ### the reference is needed to generate attributes

        scopedClasses = [ klass for klass in ownerClass._class ]
        menum = "HK_NULL"

        if not domMember.serialized:
            return

        if domMember.overridetype and domToClass._typecode(domMember.overridetype) == "hkClassMember::TYPE_ZERO":
            return

        # extract real type and subtype
        self.havokType = domToClass._typecode(self.dataType)
        self.size = 0

        # Get cstyle array size
        if self.dataType.find("[") != -1:
            self.dataType, rest = self.dataType.split("[")
            self.size = rest.split("]")[0]

        if self.havokType in ["hkClassMember::TYPE_ARRAY", "hkClassMember::TYPE_INPLACEARRAY", "hkClassMember::TYPE_SIMPLEARRAY", "hkClassMember::TYPE_ZERO" ]:
            self.havokSubtype = domToClass._infocode( self.dataType )
            if (self.havokSubtype == "hkClassMember::TYPE_POINTER" or self.havokSubtype == "hkClassMember::TYPE_STRUCT") and (domMember.flags.find("POINTER_VOIDSTAR") == -1):
                memClass = domToClass._get_array_type( self.dataType )
                self.dataType = getStructClass( domToClass.removeFromDeclarationSpaced(memClass, ("const","class","struct")) )
                self.isStruct = True
            else:
                self.dataType = domToClass._get_array_type( self.dataType )
        elif self.havokType == "hkClassMember::TYPE_POINTER" or self.havokType == "hkClassMember::TYPE_STRUCT":
            if self.havokType == "hkClassMember::TYPE_POINTER":
                self.havokSubtype = domToClass._get_type_from_pointer( domToClass.removeFromDeclarationSpaced(self.dataType, ("mutable","const")) )
            if (self.havokType == "hkClassMember::TYPE_STRUCT" or self.havokSubtype == "hkClassMember::TYPE_STRUCT"):
                self.dataType = getStructClass( domToClass.removeFromDeclarationSpaced(self.dataType, ("mutable","const","class","struct")) )
                self.isStruct = True
        elif self.havokType in [ "hkClassMember::TYPE_ENUM", "hkClassMember::TYPE_FLAGS" ]:
            #print self.dataType
            etype, esize = [x.strip() for x in self.dataType.split("<")[1].split(">")[0].split(",")]
            self.havokSubtype = domToClass._typecode( esize )
            self.dataType = getFullTypeName(etype.replace("enum ",""), ownerClass.name, namespace, allKnownClassNamesList)
            #print "->", self.havokType, self.havokSubtype
            self.typeSize = int(self.havokSubtype.replace("hkClassMember::TYPE_INT","").replace("hkClassMember::TYPE_UINT",""))
        # correct the member type
        if self.dataType == "hkBool":
            self.dataType = "bool"
        ### if this is a structure we need to specify fully qualified name for it
        if self.isStruct:
            dataType = self.dataType
            self.dataType = getFullTypeName(dataType, ownerClass.name, namespace, allKnownClassNamesList)
    def isPointerType(self):
        return self.havokType == "hkClassMember::TYPE_POINTER" or self.havokSubtype == "hkClassMember::TYPE_POINTER"

def _genCustomAttributes(className, attrOwnerName, attributes, output):
    attrs = []
    for attrname,val in attributes.items():
        attrklassname = attrname.replace(".","")
        output.append( "[%s(%s::typeid,%s,%s)]"%(attrklassname,className,('"%s"'%attrOwnerName) if attrOwnerName else 'nullptr','"%s"'%attrname) )
        attrs.append( (attrname, None, attrklassname) )
    return attrs

def genClassAttributes(domClass):
    attrCodeLines = []
    attrs = _genCustomAttributes(domClass.name, None, domClass.attributes, attrCodeLines)
    return " ".join(attrCodeLines) if len(attrs) else attrDefault

def genAttributes(className, memInfo, memIdx):
    attrCodeLines = []
    attrs = _genCustomAttributes(className, memInfo.name, memInfo.attributes, attrCodeLines)
    if memInfo.hasDefault:
        attrCodeLines.append("[hkDefaultAttribute(%s::typeid,%s)]"%(className,memIdx))
        attrs.append("hkDefaultAttribute")
    if memInfo.visibility == "public":
        attrCodeLines.append("[hkPublicAttribute()]")
        attrs.append("hkPublicAttribute")
    return " ".join(attrCodeLines) if len(attrs) else attrDefault

def getListOfParentNames(startClass, listOut):
    if startClass.parent:
        listOut.append(startClass.parent)
    for klass in startClass._class:
        if klass.reflected:
            getListOfParentNames(klass, listOut)
    return listOut

def getListOfMemberTypes(klass, namespace, classFullNameList, listOut):
    for mem in klass.member:
        if mem.reflected:
            memInfo = ManagedMemberInfo(mem, klass, namespace, classFullNameList)
            if memInfo.isStruct or memInfo.havokType in ["hkClassMember::TYPE_ENUM", "hkClassMember::TYPE_FLAGS"]:
                listOut.append(memInfo.dataType)
    for k in klass._class:
        if k.reflected:
            getListOfMemberTypes(k, getScopedName(klass.name, namespace), classFullNameList, listOut)
    return listOut

HK_CLASS_NOT_READY = 0
HK_CLASS_DONE = 1
HK_CLASS_IN_PROGRESS = 2
HK_CLASS_HIGH_PRIORITY = 3

class ManagedEnumInfo(hkcToDom.Object):
    __slots__ = ("domEnum", "status", "beingready", "namespace", "dependList")
    def __init__(self, domEnum, namespace, classFullNameList):
        hkcToDom.Object.__init__(self)
        self.domEnum = domEnum ### hkcToDom.Class
        self.namespace = namespace
        self.status = HK_CLASS_NOT_READY
        self.dependList = []

class ManagedClassInfo(hkcToDom.Object):
    __slots__ = ("domClass", "status", "beingready", "namespace", "dependList")
    def __init__(self, domClass, namespace, classFullNameList):
        hkcToDom.Object.__init__(self)
        self.domClass = domClass ### hkcToDom.Class
        self.namespace = namespace
        self.status = HK_CLASS_NOT_READY
        #self.members = [] ### list of ManagedMemberInfo objects
        self.updateDependencies(classFullNameList)
    def updateDependencies(self, classFullNameList):
        self.dependList = []
        ### build dependencies list based on parents
        classTypesList = []
        getListOfParentNames(self.domClass, classTypesList)
        ### build dependencies list based on member types
        memTypesList = []
        getListOfMemberTypes(self.domClass, self.namespace, classFullNameList, memTypesList)
        if len(memTypesList):
            for type in memTypesList:
            #    ### we depend only on types outside local scope
                classTypesList.append(type)
        ### optimize and set the dependcies list
        fullClassName = getScopedName(self.domClass.name, self.namespace)
        if len(classTypesList):
            classTypesList.sort()
            self.dependList = [""]
            for type in classTypesList:
                ### we depend only on types outside the namespace
                if not isLocalScopeType(type, fullClassName):
                    if self.dependList[-1] != type:
                        self.dependList.append(type)
            del self.dependList[0] ### remove first dummy item

def genFileHeadlines(content, file=None, filename=None, customLines=None):
    if file:
        content.append(_tkbms % ((file.product or 'BEHAVIOR'), (file.visibility or 'CLIENT')) )
    else:
        content.append(_tkbms % ('BEHAVIOR', 'CLIENT') )
    content.append("\n// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.")
    if filename:
        content.append("// Generated from '%s'\n" % filename )
    if customLines and len(customLines) > 0:
        for l in customLines:
            content.append(l)
    content.append("#include <HavokAssembly/hkHavokAssembly.h>\n")
    content.append("#pragma unmanaged")
    content.append("#include <HavokAssembly/hkHavokUnmanaged.h>\n")
    content.append("#pragma managed")
    content.append("#include <HavokAssembly/hkHavokManagedTypes.h>")
    content.append("#include <HavokAssembly/hkHavokManagedUtils.h>\n")

def domToManagedClass(dom, debug=0, collectAll=False, pchfile=""):
    """Generate a managed c++ file from the given input hkClass dom.
    """
    managedClassesDecl = FormatedSourceCode()
    managedClassesDef = FormatedSourceCode()
    ret = FormatedSourceCode()

    ### getClassSpecInfo returns (classSpecKey, classSpecArgs)
    def getClassSpecInfo(className, classParent=0, classScope=0, attributes=attrDefault):
        specKey = "base"
        args = [className]
        if classParent:
            specKey = "derived"
            args.append(classParent)
        if classScope:
            specKey = "scoped"
            args.insert(0, classScope)
        if classParent and classScope:
            specKey = "derived_scoped"
        if attributes not in [attrIgnore, attrExclude]:
            args.append(attributes)
        return (specKey, args)

    def getClassCtorSpecInfo(className, params, classParent=0, classScope=0):
        specKey = "base"
        args = [className,params]
        if classParent:
            specKey = "derived"
            args.append(classParent)
        if classScope:
            specKey = "scoped"
            args.insert(0, classScope)
        if classParent and classScope:
            specKey = "derived_scoped"
        return (specKey, args)

    ### shouldGenerateCtor is a temporary workaround, should be removed when properly implemented
    def shouldGenerateCtor(fullname, klass):
        return not ((klass.memory_declaration and klass.memory_declaration[0]!="public") or klass.defineattribute or klass.abstract)

    ### getPropertySpecInfo returns (propertySpecKey, propertySpecArgs)
    ### typeDesc = (class name, type, type), e.g. (hkShape, TYPE_POINTER, TYPE_STRUCT), (HK_NULL, TYPE_CSTRING, TYPE_VOID)
    ### it is assumed that the type description was not preprocessed with "+nosave" "override(type)" yet
    def getPropertySpecInfo(memInfo):
        args = [memInfo.dataType, memInfo.name]
        specKey = getInternalSpecKey(memInfo.havokType, memInfo.havokSubtype)
        if specKey.count("cstring") or specKey.count("variant") or specKey=="zero":
            del args[0]
        if specKey in [ "enum", "flags" ]:
            args.append(memInfo.typeSize)
        return (specKey, args)

    def getManagedClassPropertyDeclSpec(className, memInfo, propertyIndex):
        ### specify the spec
        specInfo = getPropertySpecInfo(memInfo)
        specResult = classPropertySpec
        specResult += "".join( [specItem for specItem in classPropertySpecDict[specInfo[0]]] )
        ### add class names base on spec
        specInfo[1].append(genAttributes(className,memInfo, propertyIndex))
        specResult = specResult % tuple(specInfo[1])
        specResult += ";"
        return specResult

    def getManagedClassPropertyDefSpec(className, memInfo, propertyIndex):
        ### specify the spec
        specInfo = getPropertySpecInfo(memInfo)
        specResult = classPropertySpec
        for i, specItem in enumerate(classPropertySpecDict[specInfo[0]]):
            ### build definition spec
            if i == 0:
                specResult += specItem + classPropertyDefSpec
            ### build params spec
            if i == 1:
                specItemParts = specItem.split(")")[:-1]
                ### add params for class name and declared member index
                specItemParts.insert(1, "%s)")
                specResult += ",".join( [item for item in specItemParts] )
        ### add class names base on spec
        args = [className]
        args.extend(specInfo[1])
        args.append(propertyIndex)
        specResult = specResult % tuple(args)
        specResult += ";"
        return specResult

    def getManagedEnumDeclSpec(specVar, enumName):
        return ("".join(specVar)) % enumName
    
    def getManagedEnumValueDeclSpec(specVar, valueSpecVar, enumValueName, enumValue):
        return (specVar[0] + "".join(valueSpecVar)) % (enumValueName, enumValue)

    ### common function to use for class and attribute declarations based on passed specs
    def getManagedDeclSpec(spec, specDict, className, classParent=0, classScope=0, attributes=attrDefault):
        ### get the spec
        specInfo = getClassSpecInfo(className, classParent, classScope, attributes)
        specResult = spec
        #specResult += "".join( [specItem for specItem in specDict[specInfo[0]]] )
        for specItem in specDict[specInfo[0]]:
            if attributes == attrExclude and specItem.startswith("("):
                ### remove last '%s' item from the format string
                specItem = "(" + ",".join(specItem.replace("(","").replace(")","").split(",")[:-1]) + ")"
            specResult += specItem
        ### add class names base on spec
        #print "getManagedDeclSpec:", specResult, specInfo[1]
        specResult = specResult % tuple(specInfo[1])
        return specResult

    ### common function to use for class and attribute definitions based on passed specs
    def getManagedDefSpec(spec, specDict, className, classParent=0, classScope=0, attributes=attrDefault):
        ### specify the spec
        specInfo = getClassSpecInfo(className, classParent, classScope, attributes)
        specResult = spec
        for specItem in specDict[specInfo[0]]:
            if attributes == attrExclude and specItem.startswith("("):
                ### remove last '%s' item from the format string
                specItem = "(" + ",".join(specItem.replace("(","").replace(")","").split(",")[:-1]) + ")"
            specResult += specItem
        ### add class names base on spec
        specResult = specResult % tuple(specInfo[1])
        return specResult

    def getManagedClassDeclSpec(classSpec, className, classParent=0, classScope=0, attributes=attrDefault):
        return getManagedDeclSpec(classSpec, classSpecDict, className, classParent, classScope, attributes)
    
    def getManagedClassDefSpec(className, classParent=0, classScope=0):
        return getManagedDefSpec(classDeclSpec + classDefSpec, classSpecDict, className, classParent, classScope, attrExclude)

    def getManagedAttrDeclSpec(attrSpec, className, classParent=0, classScope=0):
        return getManagedDeclSpec(attrSpec, attrSpecDict, className, classParent, classScope, attrIgnore)

    def getManagedAttrDefSpec(className, classParent=0, classScope=0):
        return getManagedDefSpec(attrDeclSpec + attrDefSpec, attrSpecDict, className, classParent, classScope, attrIgnore)

    def genManagedEnumDeclSpec(domEnum):
        managedEnumDeclStart = getManagedEnumDeclSpec(((domEnum.defineflags and classFlagDeclSpec) or classEnumDeclSpec),domEnum.name)
        managedEnumDeclBody = []
        value = 0
        for i in domEnum.item:
            if i.value != None:
                value = i.value
            managedEnumDeclBody.append(getManagedEnumValueDeclSpec(((domEnum.defineflags and classFlagDeclSpec) or classEnumDeclSpec), \
                                                                    ((domEnum.defineflags and classFlagValueDeclSpec) or classEnumValueDeclSpec), \
                                                                    i.name, value))
            try:
                value = int(value) + 1
            except ValueError:
                value = value + "+1"
        managedEnumDeclString = "%s\n{\n%s\n};\n"
        return managedEnumDeclString % (managedEnumDeclStart, ",\n".join(managedEnumDeclBody))

    def genManagedClassDeclarion(className, abstract, classBody, classParent=0, classScope=0, attributes=attrDefault):
        classStart = getManagedClassDeclSpec(classDeclSpec, className, classParent, classScope, attributes)
        if abstract:
            classStart = classStart.replace(classDeclSpec, classDeclAbstractSpec)
        classBase = getManagedClassDeclSpec(classBaseSpec, className, classParent, classScope, attrExclude)
        classBodyText = "\n".join(classBody)
        if len(classBodyText):
            classBodyText = "\n" + classBodyText + "\n"
        managedClassDeclString = "%s\n{\n%s;\n%s};\n" ### classStart, classBase, classBodyText
        return managedClassDeclString % (classStart, classBase, classBodyText)
    
    def genManagedClassDefinition(className, classBody, classParent=0, classScope=0):
        classDefs = getManagedClassDefSpec(className, classParent, classScope)
        managedClassDefString = "//%s\n%s;\n%s" ### className, classDefs, classBodyDefinitions
        classBodyText = "\n".join(classBody)
        if len(classBodyText):
            classBodyText = "\n" + classBodyText + "\n"
        return managedClassDefString % (className, classDefs, classBodyText)

    def genManagedAttributeDeclarion(className, attrBody, classParent=0, classScope=0):
        attrStart = getManagedAttrDeclSpec(attrDeclSpec, className, classParent, classScope)
        attrBase = getManagedAttrDeclSpec(attrBaseSpec, className, classParent, classScope)
        attrBodyText = "\n".join(attrBody)
        if len(attrBodyText):
            attrBodyText = "\n" + attrBodyText + "\n"
        managedAttrDeclString = "%s\n{\n%s;\n%s};\n" ### attrStart, attrBase, attrBodyText
        return managedAttrDeclString % (attrStart, attrBase, attrBodyText)
    
    def genManagedAttributeDefinition(className, attrBody, classParent=0, classScope=0):
        attrDefs = getManagedAttrDefSpec(className, classParent, classScope)
        managedAttrDefString = "//%s\n%s;\n%s" ### className, attrDefs, attrBodyDefinitions
        attrBodyText = "\n".join(attrBody)
        if len(attrBodyText):
            attrBodyText = "\n" + attrBodyText + "\n"
        return managedAttrDefString % (className, attrDefs, attrBodyText)

    def getManagedClassCtorDeclSpec(className, params, classParent=0, classScope=0):
        ### get the spec
        specInfo = getClassCtorSpecInfo(className, params, classParent, classScope)
        specResult = classCtorDeclSpec
        specResult += "".join( classCtorSpecDict[specInfo[0]] )
        ### add class names base on spec
        specResult = specResult % tuple(specInfo[1])
        return specResult+";"

    def getManagedClassCtorDefSpec(className, params, classParent=0, classScope=0):
        ### specify the spec
        specInfo = getClassCtorSpecInfo(className, params, classParent, classScope)
        specResult = classCtorDeclSpec + classCtorStartDefSpec
        for specItem in classCtorSpecDict[specInfo[0]]:
            specResult += specItem
        ### add class names base on spec
        specResult = specResult % tuple(specInfo[1])
        specResult += "\n%s%s%s;"
        specResult = specResult % (classCtorDeclSpec,classCtorEndDefSpec,classCtorSpecDict["end"][1])
        return specResult

    def getInternalMembersName(namespace, klass):
        hasNonPublicMembers = False in [m.visibility in ("","public") for m in klass.member]
        where = ("%s::%s" % (namespace, klass.name)).lstrip(":")
        if hasNonPublicMembers and not collectAll:
            return "%s::Members" % where
        else:
            return "%sClass_Members" % where.replace("::","_")

    def isDefaultCtor(klass, method):
        if klass.name == method.name:
            paramNum = len(method.parameter)
            return paramNum == 0 or paramNum == sum( [1 for p in method.parameter if p.default != None ] )
        return False

    def genEnums(enums):
        managedEnumList = []
        for enum in enums:
            managedEnumList.append(genManagedEnumDeclSpec(enum))
        return managedEnumList
        
    def genClass(klass, namespace, classFullNameList, classFromFullNameMap):
        """Generate a single class.
        """
        if not klass.reflected:
            return None, None

        namespace_klass_name = ("%s::%s" % (namespace, klass.name)).lstrip(":")
        klassBodyDecl = []
        klassBodyDef = []

        # nested enums and structs
        managedEnumsDecl = genEnums(klass.enum)
        if len(managedEnumsDecl):
            klassBodyDecl.append("".join(managedEnumsDecl))
        for s in klass._class:
            klassDecl, klassDef = genClass(s, namespace_klass_name, classFullNameList, classFromFullNameMap)
            if klassDecl:
                klassBodyDecl.append(klassDecl)
            if klassDef:
                klassBodyDef.append(klassDef)

        ### this is a temporary workaround for generating default ctors for behavior classes
        ### should be removed when properly implementated
        if shouldGenerateCtor(namespace_klass_name, klass):
            hasCtor = False
            for i,method in enumerate(klass.method):
                hasCtor = (klass.name == method.name) or hasCtor
                if hasCtor and method.visibility == "public" and isDefaultCtor(klass, method):
                    klassBodyDecl.append( getManagedClassCtorDeclSpec(method.name, "", klass.parent, namespace) )
                    klassBodyDef.append( getManagedClassCtorDefSpec(method.name, "", klass.parent, namespace) )
            if len(klass.method) == 0 or not hasCtor:
                klassBodyDecl.append( getManagedClassCtorDeclSpec(klass.name, "", klass.parent, namespace) )
                klassBodyDef.append( getManagedClassCtorDefSpec(klass.name, "", klass.parent, namespace) )

        for i,m in enumerate(klass.member):
            if m.reflected:
                memInfo = ManagedMemberInfo(m, klass, namespace, classFullNameList)
                if memInfo.isStruct and not memInfo.isPointerType():
                    memName = "%s::%s" % (klass.name, memInfo.name)
                    if not classFromFullNameMap.has_key(memInfo.dataType): ### class may be specified in different manifest
                        print >>sys.stderr, "WARNING! Please make sure that the class member '%(memName)s' is not virtual (reference counted).\n"\
                            "The embedded virtual reference counted class members are not supported in managed class."%locals()
                    else:
                        memClass = classFromFullNameMap[memInfo.dataType]
                        if memClass.vtable:
                            print >>sys.stderr, "WARNING! Ignore the class member '%(memName)s'.\n"\
                                "The embedded virtual reference counted class members are not supported in managed class."%locals()
                            #raise TypeError, "The the C++ class member '%(memName)s' declaration is not supported in managed class."%locals()
                            continue
                klassBodyDecl.append( getManagedClassPropertyDeclSpec(namespace_klass_name, memInfo, i) )
                klassBodyDef.append( getManagedClassPropertyDefSpec(namespace_klass_name, memInfo, i) )

        if klass.defineattribute:
            klassDecl = genManagedAttributeDeclarion(klass.name, klassBodyDecl, klass.parent, namespace)
            klassDef = genManagedAttributeDefinition(klass.name, klassBodyDef, klass.parent, namespace)
        else:
            klassDecl = genManagedClassDeclarion(klass.name, klass.abstract, klassBodyDecl, klass.parent, namespace, genClassAttributes(klass))
            klassDef = genManagedClassDefinition(klass.name, klassBodyDef, klass.parent, namespace)
        return klassDecl, klassDef

    def genFiles(file):

        def genClassInfoList(classesList, classFromFullNameMap, enumFullNameList):
            classInfoListOut = []
            fullNameList = classFromFullNameMap.keys()
            fullNameList.extend(enumFullNameList)
            for name in classesList:
                k = classFromFullNameMap[name]
                info = ManagedClassInfo(k, getScopeNoName(name), fullNameList)
                classInfoListOut.append(info)
            return classInfoListOut

        def genEnumInfoList(enumsList, enumFromFullNameMap):
            enumInfoListOut = []
            fullNameList = enumFromFullNameMap.keys()
            for name in enumsList:
                k = enumFromFullNameMap[name]
                info = ManagedEnumInfo(k, getScopeNoName(name), fullNameList)
                enumInfoListOut.append(info)
            return enumInfoListOut

        def extractClassName(managedName, classFromNameMap):
            if classFromNameMap.has_key(managedName):
                return managedName
            managedNameScope = getScopeNoName(managedName)
            if len(managedNameScope):
                return extractClassName(managedNameScope, classFromNameMap)
            return None
        #
        #PROCESS CLASS
        #check CLASS for dependencies recursively:
        #   for each CLASS dependency if dependency in not resolved
        #       PROCESS the dependency of CLASS
        #mark CLASS as resolved
        #generate CLASS
        #
        def processClass(classFromNameMap, managedClassInfo, classFullNameList, classFromFullNameMap):

            def splitDependList(dependList, classFromNameMap):
                listOfClasses = []
                listOfUnknownClasses = []
                for x in dependList:
                    className = extractClassName(x, classFromNameMap)
                    if className:
                        if classFromNameMap[className].status != HK_CLASS_DONE:
                            listOfClasses.append(className)
                    else:
                        listOfUnknownClasses.append(x)
                return listOfClasses, listOfUnknownClasses

            classDeclList = []
            classDefList = []
            classMissingList = []
            klass = managedClassInfo.domClass
            if managedClassInfo.status == HK_CLASS_NOT_READY:
                managedClassInfo.status = HK_CLASS_IN_PROGRESS
                dependList, missingList = splitDependList(managedClassInfo.dependList, classFromNameMap)
                for x in missingList:
                    if x not in classMissingList:
                        classMissingList.append(x)
                if len(missingList):
                    print "DEPEND:", classMissingList
                    print "Class",klass.name,"uses not defined classes:\n\t",missingList
                for dependKlassName in dependList:
                    if dependKlassName.split("::")[0] != klass.name: ### if not nested declaration of current class
                        if klass.parent == dependKlassName and classFromNameMap[dependKlassName].status == HK_CLASS_IN_PROGRESS:
                            classFromNameMap[dependKlassName].status = HK_CLASS_HIGH_PRIORITY
                            if klass.name not in classMissingList:
                                classMissingList.append(klass.name)
                        classDecl, classDef, missingClasses = processClass(classFromNameMap, classFromNameMap[dependKlassName], classFullNameList, classFromFullNameMap)
                        if len(classDecl):
                            classDeclList.append(classDecl)
                        if len(classDef):
                            classDefList.append(classDef)
                        if len(missingClasses):
                            for x in missingClasses.split("/n"):
                                if x not in classMissingList:
                                    classMissingList.append(x)
                    else:
                        ### update pending list, no need for forward declaration for local class
                        classFromNameMap[dependKlassName].status = HK_CLASS_DONE
                if classFromNameMap[klass.name].status != HK_CLASS_DONE:
                    classDecl, classDef = genClass(klass, "", classFullNameList, classFromFullNameMap)
                    classDeclList.append(classDecl)
                    classDefList.append(classDef)
                    classFromNameMap[klass.name].status = HK_CLASS_DONE
            else:
                if classFromNameMap[klass.name].status == HK_CLASS_IN_PROGRESS:
                    ### forward declaration
                    if klass.name not in classMissingList:
                        classMissingList.append(klass.name)
                elif managedClassInfo.status == HK_CLASS_HIGH_PRIORITY:
                    classDecl, classDef = genClass(klass, "", classFullNameList, classFromFullNameMap)
                    classDeclList.append(classDecl)
                    classDefList.append(classDef)
                    classFromNameMap[klass.name].status = HK_CLASS_DONE
            return "\n".join(classDeclList), "\n".join(classDefList), "\n".join(classMissingList)

        filename = dom.localfilename.replace("\\","/")
        if not collectAll:
            genFileHeadlines(managedClassesDecl, file, filename)
            genFileHeadlines(managedClassesDef, file, filename)

        # should really use xml file decl order here
        if len(file.enum) or len(file.struct):
            managedClassesDecl.append("//\n// Global\n//")
            managedClassesDef.append("//\n// Global\n//")

        enumFromFullNameMap = dict()
        enumFullNameList = []
        enumInfoFromClassNameMap = dict()
        classDataList = []
        classFromFullNameMap = dict()
        classFullNameList = []
        classInfoFromClassNameMap = dict()
        ### get list of all reflected classes and enums with fully qualified names
        for e in file.enum:
            enumFromFullNameMap[e.name] = e
        for klass in file._class:
            if klass.reflected:
                getMapOfScopedClasses(klass, classFromFullNameMap)
                getMapOfScopedEnums(klass, enumFromFullNameMap)

        if len(enumFromFullNameMap):
            ### generate list of global enums info
            enumFullNameList = enumFromFullNameMap.keys()
            enumFullNameList.sort()
            globalEnumNameList = [name for name in enumFullNameList if not name.lstrip(":").count("::")] # global enums
            enumInfoList = genEnumInfoList(globalEnumNameList, enumFromFullNameMap)
            enumInfoFromClassNameMap = dict(zip(globalEnumNameList, enumInfoList))

        if len(classFromFullNameMap):
            ### generate list of global classes info
            classFullNameList = classFromFullNameMap.keys()
            classFullNameList.sort()
            globalClassNameList = [name for name in classFullNameList if not name.lstrip(":").count("::")] # global classes
            classInfoList = genClassInfoList(globalClassNameList, classFromFullNameMap, enumFullNameList)
            classInfoFromClassNameMap = dict(zip(globalClassNameList, classInfoList))
            ### generate list of hkClasses
            managedClassesDecl.append("#pragma unmanaged")
            managedClassesDecl.append("\n".join( ["extern const hkClass %sClass;"%"".join( className.lstrip(":").split("::") ) for className in classFullNameList] ))
            managedClassesDecl.append("#pragma managed")

        managedClassesDecl.append("namespace %s\n{" % classNamespace)
        managedClassesDef.append("namespace %s\n{" % classNamespace)

        managedEnumsDecl = genEnums(file.enum)
        managedClassesDecl.append("".join(managedEnumsDecl))
        for e in file.enum:
            enumInfoFromClassNameMap[e.name].status = HK_CLASS_DONE

        ### extend the class name list with enums here
        if len(enumFullNameList):
            # merge the enums and classes list and map
            classFullNameList.extend(enumFullNameList)
            classFullNameList.sort()
            classInfoFromClassNameMap.update(enumInfoFromClassNameMap)

        for c in file._class:
            if c.reflected and c.defineattribute:
                classDecl, classDef, missingClasses = processClass(classInfoFromClassNameMap, classInfoFromClassNameMap[c.name], classFullNameList, classFromFullNameMap)
                if len(missingClasses):
                    for x in missingClasses.split("\n"):
                        managedClassesDecl.append("ref class %s;" % x)
                if len(classDecl):
                    managedClassesDecl.append(classDecl)
                if len(classDef):
                    managedClassesDef.append(classDef)

        for c in file._class:
            if c.reflected and not c.defineattribute:
                classDecl, classDef, missingClasses = processClass(classInfoFromClassNameMap, classInfoFromClassNameMap[c.name], classFullNameList, classFromFullNameMap)
                if len(missingClasses):
                    for x in missingClasses.split("\n"):
                        managedClassesDecl.append("ref class %s;" % x)
                if len(classDecl):
                    managedClassesDecl.append(classDecl)
                if len(classDef):
                    managedClassesDef.append(classDef)

        managedClassesDecl.append("}\n")
        managedClassesDef.append("}\n")

    genFiles(dom.file)
    return "\n".join(managedClassesDecl.lines), "\n".join(managedClassesDef.lines)

def usage():
    import os
    import sys
    print 'Usage: %s <filename.hkclass>\n' % os.path.basename(sys.argv[0])
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
        return domToManagedClass(dom, debug)
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
