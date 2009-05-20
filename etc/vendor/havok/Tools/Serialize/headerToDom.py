#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###
### This script is used to extract an hkClass definition from a c++ header file.
###
### This script can be either be executed directly or imported
###

import re
import sys
import hkcToDom
import string
import util

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

extraTypesFile, globalTypeFromLocal = _readExtraTypes()

def applyAttr(target, attrname, value):
    try:
        curval = getattr( target, attrname )
    except AttributeError:
        assert attrname.rsplit(".",1)[-1][0] in string.uppercase, "Misspelled attribute. If '%s' is a custom identifier, it must begin with upper case." % attrname
        target.attributes[attrname] = value
    else:
        if curval == None:
            newval = value
        elif type(curval) == bool:
            newval = value.strip().lower() in ["1", "true"]
        else:
            newval = type(curval)(value)
        setattr( target, attrname, newval )

class TextMatcher:
    def __init__(self, name, rules):
        self.name = name
        self.rules = rules

    def run(self, txt):
        #print "*** %i '%s'"%( len(txt), txt[-10:])
        bestMatch = None
        bestRule = None
        for rule in self.rules:
            match = rule.search(txt)
            if match:
                if bestMatch==None or match.start() < bestMatch.start():
                    bestMatch = match
                    bestRule = rule
        if bestMatch == None:
            return None, None
        matchRange = bestMatch.span()
        skipped = txt[:matchRange[0]].strip()
        if len(skipped):
            print >>sys.stderr, "-- When parsing '%s' UNPARSED '%s' at '%s...'" % (self.name, skipped, txt[:100])
        return bestRule, bestMatch

def killwhitespace(s):
    return s.replace("\t","").replace("\n","").replace("\r","").replace(" ","")

class CppParser:
    
    re_commentAttribute = re.compile( r"//\s*(\+.+)" )
    re_docsComment = re.compile( "^(\s*///.*\n)+", re.M )
    re_cComment = re.compile( r"/\*[^*]*\*+([^/*][^*]*\*+)*/" )
    re_cppComment = re.compile( r"\s*//.*" )
    re_whitespace = re.compile( r"\s+" )
    re_whitespaceInArray = re.compile(r"\s*\[\s*(\S+)\s*\]")
    re_constInDecl = re.compile(r"\s*\bconst\b\s*")
    re_preproc = re.compile( r"\s*#\s*(pragma|error|undef|define|ifdef|ifndef|if|elif|else|endif).*")
    re_classAlign = re.compile( r"HK_CLASSALIGN(?:\d*)\s*\(([^,)]+)(?:,\s*\d+\s*)?\s*\)")
    re_align = re.compile( r"HK_ALIGN(\d*)\s*\(([^,\)]+)(?:,\s*(\d+)\s*)?\s*\)\s*;")
    re_substmacros = re.compile( r"\s*(HKP_SHAPE_VIRTUAL_CONST|HKP_SHAPE_VIRTUAL_THIS|HKP_SHAPE_VIRTUAL|HCL_SHAPE_VIRTUAL)\s*" ) ### macros to substitude
    re_refPtr = re.compile( r"hkRefPtr<\s*([^>]+)\s*>" )
    re_junkymacros = re.compile( r"\s*HK_(CPU_PTR|PAD_ON_SPU|THREAD_LOCAL)\((.*?)\)" ) ### leave group(2) in place
    re_memDeclaration = re.compile(r"\s*((HK_DECLARE_(CLASS|NONVIRTUAL_CLASS|SYSTEM)_ALLOCATOR)[^\s\(]*)", re.M)
    re_overideDestination = re.compile(r'^\s*HK_REFLECTION_CLASSFILE_DESTINATION\s*\(\s*"([^"]*)"\s*\)\s*;', re.M)

    re_classStart = re.compile( r"(?P<type>struct|class)\s+(?P<namespace>(::)?(\w+::)*)(?P<name>\w+)(\s*:(?!:)\s*(?P<parents>[^;{]+))?\s*(?P<decltype>[{;])" )
    re_enumStart = re.compile( r"enum\s*(?P<name>\w*)\s*(?P<decltype>[{;])" )
    re_unionStart = re.compile( r"union\s*(?P<name>\w*)\s*(?P<decltype>[{;])" )
    re_namespaceStart = re.compile( r"namespace\s*(?P<name>\w*)\s*(?P<decltype>[{;])" )
    re_taggedUnionStart = re.compile( r"hkTaggedUnion\s*(?P<name>\w*)\s*(?P<decltype>[{;])" )
    re_typedef = re.compile( r"typedef\s*[^;]+;" )
    re_attributesPlaceholder = re.compile( r"\001ATT([^\002]+)\002;" )
    re_attributes = re.compile(r"\+([\w\.]+)(?:\(([^\)]+)\))?")
    re_externVariable = re.compile( r"((extern|static)\s+)?(const\s+)?((class|struct)\s+)?(?P<type>[\w\*]+)\s+(?P<name>\w+)\s*(?P<value>=[^;]+)?;" )
    re_functionDefinition = re.compile( r"(?P<method>[\w~][^;\(]+)\s*\((?P<params>[^\)]*)\)\s*(?P<qualifiers>[^{;]*)(?P<decltype>[{;])" )
    re_spuriousSemicolon = re.compile( "^\s*;" )
    re_externCstart = re.compile( r"extern\s*\"C\"\s+[^{;]*(?P<decltype>[{;])" )
    re_hashInclude = re.compile( r'#\s*include\s*["<](?P<path>[^">]+)[>"]' )
    re_templateStart = re.compile( r"template\s*<" )
    re_enumItem  = re.compile( r"(?P<name>\w+)\s*(=\s*(?P<value>[^\s,]+))?,?" )
    re_taggedUnionItem  = re.compile( r"hkTaggedUnionItem\s*\(\s*(?P<name>\w+)\s*(=\s*(?P<value>[^\s,]+)\s*)?,\s*(?P<class>[^\s\)]+)\s*\)\s*,?" )
    re_accessControl = re.compile( r"(public|protected|private):" )
    re_classMethod = re.compile( r"(?P<method>[\w~][^;\(]+)\s*\((?P<params>[^\)]*)\)\s*(?P<qualifiers>[^\]{;]*)(?P<decltype>[{;])" )
    re_classMember = re.compile( r"(?P<member>\w[^{;\(]+);" )
    re_functionPointer = re.compile( r"\([^\*]*\*[^\)]+\)\s*\([^\)]*\)" )

    def __init__(self):
        self.debug = lambda x : sys.stdout.write(x+"\n")
        self.debug = lambda x : None
        self.globalMatcher = TextMatcher( "global", [
            self.re_overideDestination,
            self.re_templateStart,
            self.re_hashInclude,

            self.re_classStart,
            self.re_enumStart,
            self.re_unionStart,
            self.re_namespaceStart,
            self.re_taggedUnionStart,

            self.re_externCstart,
            self.re_typedef,

            self.re_externVariable,
            self.re_functionDefinition,
            self.re_attributesPlaceholder,
            self.re_spuriousSemicolon,
        ] )
        self.classMatcher = TextMatcher( "class", [
            self.re_accessControl,
            self.re_classStart,
            self.re_enumStart,
            self.re_unionStart,
            self.re_typedef,
            self.re_attributesPlaceholder,
            self.re_classMethod,
            self.re_classMember,
            self.re_spuriousSemicolon,
        ] )
        self.enumMatcher = TextMatcher( "enum", [
            self.re_attributesPlaceholder,
            self.re_enumItem,
        ] )
        self.taggedUnionMatcher = TextMatcher( "tagged", [
            self.re_taggedUnionItem,
        ] )
        self.current_filename = None

    def findEndBrace(self, txt, start, braces="{}"):
        cur = start
        lparens = 1
        while lparens:
            rp = txt.index(braces[1], cur)
            lparens -= 1
            lparens += txt[cur:rp].count(braces[0])
            cur = rp+1
        return cur

    def warn(self, match, txt):
        span = match.span()
        self.debug( "WARN (%s) (%s)" % (txt[:span[0]], txt[span[0]:span[1]]) )

    def location_of(self, what):
        if self.current_filename:
            txt = open(self.current_filename).read()
            try:
                if isinstance(what, basestring):
                    cidx = txt.find(what)
                else:
                    cidx = what.search(txt).start() + 1
            except:
                cidx = 0 # failsafe
            return "%s:%i" % (self.current_filename, txt[:cidx].count("\n") + 1)
        else:
            return "unknown_location:0"

    def parseAccessControl(self, match, txt):
        self.debug("ACCESS %s"% match.group())

    def _parseEnumOrTaggedUnion(self, match, txt, Defn, Item, Matcher):
        if match.group("decltype") == "{":
            self.debug("%s(%s)" % (Defn.__name__.upper(), match.group()))
            name = match.group("name")
            span = match.span()
            defnEnd = self.findEndBrace(txt, span[1] )
            if name:
                defn = Defn()
                defn.name = name
                body = txt[span[1]:defnEnd-1]
                seen_values = {}
                while 1:
                    item, match = Matcher.run( body )
                    if match == None:
                        break
                    elif item == self.re_attributesPlaceholder:
                        newbody, attrs = self.parseAttributes(match, body)
                        for k,v in attrs:
                            applyAttr(defn, k, v)
                    elif item in [ self.re_enumItem, self.re_taggedUnionItem ]:
                        i = Item()
                        i.name = match.group("name")
                        i.initializer = match.group("value")
                        try: i.klass = match.group("class")
                        except IndexError: pass
                        if i.initializer == None:
                            if len(defn.item):
                                i.value = defn.item[-1].value + 1
                            else:
                                i.value = 0
                        else:
                            try: i.value = int( eval( i.initializer, seen_values ) )
                            except (SyntaxError, NameError):
                                i.value = 0 ### This is only for non-reflected enums
                                print >>sys.stderr, "---------------UNPARSED '%s' in '%s'" % (i.initializer,body)
                        seen_values[i.name] = i.value
                        defn.item.append(i)
                        self.debug("%s(%s)"% (Item.__name__.upper(), hkcToDom.slot_values_as_keywords(i)) )
                    else:
                        self.debug("NO_ACTION %s %s"% (match.group(), item))
                    oldBodyLength = len(body)
                    body = body[match.span()[1]:]
                    if len(body) >= oldBodyLength:
                        raise ("*** Breaking from infinite loop ***\n" \
                            "While parsing '%s'\n" \
                            "'%s'" % (defn.name, body) )
                return txt[defnEnd:], defn
            return txt[defnEnd:], None
        return None, None

    def parseEnum(self, match, txt):
        return self._parseEnumOrTaggedUnion(match, txt, hkcToDom.Enum, hkcToDom.EnumItem, self.enumMatcher)

    def parseTaggedUnion(self, match, txt):
        return self._parseEnumOrTaggedUnion(match, txt, hkcToDom.TaggedUnion, hkcToDom.TaggedUnionItem, self.taggedUnionMatcher)

    def parseUnion(self, match, txt):
        if match.group("decltype") == "{":
            self.debug("UNION %s"% match.group())
            span = match.span()
            end = self.findEndBrace(txt, span[1] )
            return txt[end:], None
        return None, None

    def parseAttributes(self, match, txt):
        self.debug("ATTRIBUTES %s"% match.group(1))
        return None, self.re_attributes.findall(match.group())

    def parseExternC(self, match, txt):
        if match.group("decltype") == "{":
            span = match.span()
            end = self.findEndBrace(txt, span[1] )
            return txt[end:], None
        return None, None

    def parseNamespace(self, match, txt):
        if match.group("decltype") == "{":
            span = match.span()
            brace = self.findEndBrace(txt, span[1] ) - 1
            return txt[span[1]:brace] + txt[brace+1:], None # remove matching }
            #return txt[end:], None # ignore namespace block
        return None, None

    def parseFunction(self, match, txt):
        if match.group("decltype") == "{":
            span = match.span()
            end = self.findEndBrace(txt, span[1] )
            return txt[end:], None
        return None, None

    def parseClass(self, match, txt, declaredscope):
        if match.group("decltype") == "{":
            self.debug("CLASS (%s) (%s) (%s) (%s)" % (match.group("type"), match.group("namespace"), match.group("name"), match.group("parents")))
            span = match.span()
            classEnd = self.findEndBrace(txt, span[1] )
            klass = hkcToDom.Class()
            klass.scope = declaredscope
            klass.fromheader = 1
            klass.name = match.group("name")
            parents = match.group("parents")
            if parents:
                getparentname = lambda x : x[len(x) > 1 and 1 or 0]
                parents = [getparentname(x.split()).split("<")[0].strip() for x in parents.split(",")]
                klass.parent = parents[0]
                for iname in parents[1:]:
                    ic = hkcToDom.Interface()
                    ic.name = iname
                    klass.interface.append( ic )
            body = txt[span[1]:classEnd-1]
            if match.group("type") == "class":
                currentAccess = "private"
            else:
                currentAccess = "public"
            
            def appendMember(mname, mtype, mvis, msrc, mflags=""):
                assert len(mname)
                if len(mtype) == 0:
	                print >>sys.stderr, "Error: MISSING STRING AFTER '%s'" % (mname)
                assert len(mtype)
                if mname.startswith("m_"):
                    mname = mname[2:]
                # check for synthetic member types
                if len(klass.member) >= 1:
                    prev = klass.member[-1]
                    prev2 = klass.member[-2] if len(klass.member) >= 2 else None
                    if mname.startswith("num") and mname[3:].lower() == prev.name.lower():
                        errPrefix = "%s::%s " % (klass.name, mname)
                        if prev2 and prev2.name.startswith(prev.name) and prev2.name.endswith("Class"):
                            assert prev.type == "void*"
                            assert prev2.type == "hkClass*"
                            prev2.name = prev.name
                            prev2.type = "hkHomogeneousArray"
                            assert prev2.visibility == mvis and prev.visibility == mvis, errPrefix + "homogeneous parts have differing visibility"
                            assert prev2.flags == "" and prev.flags == "", errPrefix + "homogeneous array attributes should go on last member"
                            prev2.flags = mflags
                            prev2.sourcecode += prev.sourcecode + msrc
                            klass.member.pop()
                            return
                        elif prev.type.endswith("*"):
                            prev.type = "hkSimpleArray<%s>" % prev.type.rsplit("*",1)[0].rstrip()
                            assert prev.visibility == mvis, errPrefix + "simplearray parts have differing visibility"
                            assert prev.flags == "", errPrefix + "simplearray attributes should apply to last member"
                            prev.flags = mflags
                            prev.sourcecode += msrc
                            return
                    elif mname.endswith("Class") and prev.name==mname[:-5] \
                            and mtype=="hkClass*" and prev.type=="void*":
                        prev.type = "hkVariant"
                        return
                member = hkcToDom.Member()
                member.name = mname
                member.type = mtype
                member.visibility = mvis
                member.flags = mflags
                member.sourcecode = msrc
                klass.member.append(member)

            def appendMethod(mtxt, parameters, qualifiers, visibility):
                if 0 not in [ mtxt.find(ignore) == -1 for ignore in "HK_DECLARE HK_ALIGN HK_SPU_VIRTUAL_DECLSPEC".split() ]:
                    def extract(txt, reg):
                        out = re.sub(reg, "", txt)
                        return out, txt!=out
                    method = hkcToDom.Method()
                    method.description = "%s(%s)%s" % (mtxt,parameters,qualifiers)
                    method.visibility = visibility
                    mtxt, method.inline = extract(mtxt, r"\b(inline|HK_FORCE_INLINE)\b")
                    mtxt, method.virtual = extract(mtxt, r"\bvirtual\b")
                    mtxt, method.static = extract(mtxt, r"\bstatic\b")
                    mtxt, constness = extract(mtxt, r"\b(const|explicit)\b")
                    qualifiers, method.const = extract(qualifiers, r"\bconst\b")
                    qualifiers, method.purevirtual = extract(qualifiers, "=\s*0")
                    qualifiers, initializers = extract(qualifiers, r"([:,]\s*\w+\s*\([^\)]*\))+")
                    if qualifiers.replace(")","").strip() != "":
                        print >>sys.stderr, "---------------UNPARSED '%s' in '%s'" % (qualifiers,mtxt)
                    method.name = mtxt.split()[-1]
                    if killwhitespace(parameters):
                        for i, x in enumerate(parameters.split(",")):
                            param = hkcToDom.Parameter()
                            param.description = x
                            valuelist = x.split("=")
                            if len(valuelist) == 2:
                                param.default = valuelist[-1].lstrip()
                            plist = valuelist[0].split()
                            if len([v for v in plist if v != "const"]) > 1 \
                                and plist[-1] not in [">", "*", "&"]:
                                param.name = plist[-1]
                                param.type = " ".join(v for v in plist[:-1] if v != "const")
                            else:
                                param.type = " ".join(v for v in plist if v != "const")
                                param.name = "noname_"+str(i)
                            method.parameter.append(param)
                    if klass.name == method.name:
                        self.debug("CONSTRUCTOR(%s,%s)" % (method.name, parameters))
                        assert len(mtxt.split()) == 1
                        klass.method.append( method )
                    elif "~"+klass.name == method.name:
                        self.debug("DESTRUCTOR(%s)" % (method.name))
                        assert len(mtxt.split()) == 1
                        assert len(method.parameter) == 0
                    else:
                        method.returns = " ".join( mtxt.split()[:-1] )
                        self.debug("METHOD(%s,%s::%s,%s)" % (method.returns, klass.name, method.name, parameters))
                        klass.method.append( method )
            lastParsed = None
            while 1:
                item, match = self.classMatcher.run(body)
                if match == None:
                    break

                newbody = None
                if item == self.re_accessControl:
                    currentAccess = match.group(1)
                    item = lastParsed # don't affect lastParsed
                elif item == self.re_classStart:
                    newbody, k = self.parseClass(match, body, klass)
                    if k:
                        klass._class.append(k)
                elif item == self.re_enumStart:
                    newbody, enum = self.parseEnum(match, body)
                    if enum:
                        enum.scope = klass
                        klass.enum.append(enum)
                elif item == self.re_unionStart:
                    newbody, union = self.parseUnion(match, body)
                elif item == self.re_attributesPlaceholder:
                    newbody, attrs = self.parseAttributes(match, body)
                    if lastParsed == None:
                        target = klass
                    elif lastParsed == self.re_classMember:
                        if klass.member:
                            target = klass.member[-1]
                        else:
                            target = None
                    elif lastParsed == self.re_typedef:
                        target = None # ignore attrs on typedefs for now
                    else:
                        raise "Unhandled case for attributes"
                    if target:
                        nosaveflag = False # apply nosave at end
                        for k,v in attrs:
                            if k=="nosave":
                                target.serialized = False
                                nosaveflag = True
                            else:
                                applyAttr(target, k, v)
                        if nosaveflag:
                            t = target.overridetype or target.type
                            if t.endswith("*"):
                                target.overridetype = "void*"
                            else:
                                partial = re.match("(hkArray|hkSimpleArray|hkEnum|hkFlags)\s*<\s*([^>,]+)\s*(,[^>]+)?\s*>", t)
                                if partial:
                                    stem, c, sz = partial.groups()
                                    target.overridetype = stem + ("<void*" if c.strip().endswith("*") else "<void") + (sz or "") + ">"
                    item = lastParsed # don't affect lastParsed
                elif item == self.re_classMethod:
                    span = match.span()
                    # actually a function pointer
                    if self.re_functionPointer.search(match.group()):
                        mtype = "void*"
                        mname = match.group().split("(")[1].split("*")[1].split(")")[0].split()[-1]
                        self.debug("MEMBER (%s)" % match.group())
                        appendMember(mname, mtype, currentAccess, match.group() )
                    if util.hasReflectionDeclaration(match.group()):
                        self.debug(match.group())
                        klass.reflected = True
                    elif self.re_memDeclaration.search(match.group()):
                        klass.memory_declaration = (currentAccess, match.group())
                        self.debug("%s (%s)" % (match.group(), currentAccess))
                    else:
                        if not klass.abstract and killwhitespace(match.group("qualifiers")).find("=0") != -1:
                            self.debug("ABSTRACT")
                            klass.abstract = True
                        elif not klass.vtable and "virtual" in match.group().split():
                            self.debug("VIRTUAL")
                            klass.vtable = True
                        appendMethod(match.group("method"), match.group("params"), match.group("qualifiers"), currentAccess )
                    if match.group("decltype") == "{":
                        methodEnd = self.findEndBrace(body, span[1] )
                        newbody = body[methodEnd:]
                elif item == self.re_classMember:
                    bits = self.re_constInDecl.sub(" ", match.group().rstrip(";"))
                    bits = self.re_whitespaceInArray.sub(r"[\1]", bits).split()
                    if bits[0] not in ("friend", "static", "typedef"):
                        mtype, mname = " ".join(bits[:-1]), bits[-1]
                        k = klass #todo: should lookup parent first, but often don't have definition
                        while k: # look at enclosing scopes too
                            mtype = k.typedef.get(mtype, mtype)
                            k = k.scope
                        mflags = ""
                        if bits[0] == "enum":
                            print "%s:Implicitly sized enum is not portable (%s). Use hkEnum<> instead." % (self.location_of(re.compile("^\s*enum[^;]*"+bits[2],re.M)), " ".join(bits))
                        arrayBegin = mname.find("[")
                        if arrayBegin != -1:
                            mtype += mname[arrayBegin:]
                            mname = mname[:arrayBegin]
                        self.debug("MEMBER (%s)" % match.group())
                        appendMember(mname, mtype, currentAccess, match.group(), mflags)
                elif item == self.re_typedef:
                    bits = [b for b in match.group()[:-1].split(" ") if b]
                    klass.typedef[bits[-1]] = " ".join(bits[1:-1])
                elif item == self.re_spuriousSemicolon:
                    pass
                else:
                    self.debug("NO_ACTION %s %s"% (match.group(), item))
                lastParsed = item

                oldBodyLength = len(body)
                if newbody:
                    body = newbody
                else:
                    body = body[match.span()[1]:]
                if len(body) >= oldBodyLength:
                    raise ("*** Breaking from infinite loop ***\n" \
                        "While parsing '%s'\n" \
                        "'%s'" % (klass.name, body) )
            return txt[classEnd:], klass
        else:
            self.debug("FORWARD (%s) (%s) (%s) (%s)" % (match.group("type"), match.group("namespace"), match.group("name"), match.group("parents")))
        return None, None

    def parseTemplate(self, match, txt):
        span = match.span()
        end = self.findEndBrace(txt, span[1], "<>")
        tend = len(txt)
        decl = txt.find(";", end)
        if decl != -1 and decl < tend:
            tend = decl
        defn = txt.find("{", end)
        if defn != -1 and defn < tend:
            classdecl = txt[end:defn].split(":")[0] # get class declaration and cut inheritance declaration
            if classdecl.find("<") == -1 and classdecl.find(">") == -1:
                ### just skip "template" and let to process found class as normal
                #print "---CHECK:",txt[end:defn]
                return txt[end:], None
            self.debug("TEMPLATE %s" % txt[:defn])
            tend = self.findEndBrace(txt, defn+1 )
        else:
            self.debug("TEMPLATE %s" % txt[:decl])
        return txt[tend:], None

    def parseString(self, txt):
        document = hkcToDom.Document(self.current_filename)
        document.file = hkcToDom.File()
        
        for key, value in util.extract_tkbms(txt).items():
            setattr(document.file,key,value)

        txt = self.re_classAlign.sub(r"\1", txt) # HK_CLASSALIGN(class,8) -> class
        def do_align(match):
            return "%s; //+align(%s)\n" % (match.group(2), match.group(1) or match.group(3))
        txt = self.re_align.sub(do_align, txt) # HK_ALIGN(int foo,8); -> int foo; //+align(8)
        txt = re.sub("(?m)^\s*$\n","",txt) # delete blank lines
        def docstring_join(match):
            s,e = match.span()
            lines = match.string[s:e].split("\n")[:-1]
            lines = ( l[l.index("///")+3:].strip() for l in lines )
            return '//@hk.DocString("%s")\n' % " ".join(lines)
        # txt = self.re_docsComment.sub(docstring_join, txt) # join adjacent docstring lines
        txt = self.re_commentAttribute.sub(r"\001ATT\1\002;", txt) # save attributes, use unused char delimeters
        txt = self.re_cComment.sub("", txt) # remove c comments
        txt = self.re_cppComment.sub("", txt) # c++ comments too
        txt = txt.replace("\\\n", " ") # do preproc line joining
        txt = self.re_preproc.sub("", txt) # remove preprocessor
        txt = self.re_whitespace.sub(" ", txt) # and all whitespace
        def do_hkp_macros(match):
            substmacros = { "HCL_SHAPE_VIRTUAL": " virtual ",
                            "HKP_SHAPE_VIRTUAL": " virtual ",
                            "HKP_SHAPE_VIRTUAL_CONST": " const ",
                            "HKP_SHAPE_VIRTUAL_THIS" : " "
                        }
            return substmacros[match.group(1)]
        txt = self.re_refPtr.sub(r"\1*", txt) # hkRefPtr<T> -> T*
        txt = self.re_substmacros.sub(do_hkp_macros, txt) # substitude HKP_SHAPE_VIRTUAL, HKP_SHAPE_VIRTUAL_CONST and HKP_SHAPE_VIRTUAL_THIS macros
        txt = self.re_junkymacros.sub(r"\2 ", txt) # remove HK_CPU_PTR(\2), HK_PAD_ON_SPU(\2) and HK_THREAD_LOCAL(\2) macros

        while 1:
            item, match = self.globalMatcher.run(txt)
            if match == None:
                break

            newtxt = None
            if item == self.re_overideDestination:
                document.file.overridedestination = match.group(1)
                
            elif item == self.re_templateStart:
                newtxt, junk = self.parseTemplate(match, txt)

            elif item == self.re_hashInclude:
                self.debug("INCLUDE %s"% match.group("path"))
                document.file.includeheaders += "#include <%s>\n" % match.group("path")

            elif item == self.re_classStart:
                newtxt, klass = self.parseClass(match, txt, None)
                if klass:
                    document.file._class.append(klass)
            elif item == self.re_enumStart:
                newtxt, enum = self.parseEnum(match, txt)
                if enum:
                    document.file.enum.append(enum)
            elif item == self.re_unionStart:
                newtxt, union = self.parseUnion(match, txt)
            elif item == self.re_namespaceStart:
                newtxt, junk = self.parseNamespace(match, txt)
            elif item == self.re_taggedUnionStart:
                newtxt, union = self.parseTaggedUnion(match, txt)
                if union:
                    document.file.taggedunion.append(union)
            elif item == self.re_externCstart:
                newtxt, junk = self.parseExternC(match, txt)
            elif item == self.re_typedef:
                pass
            elif item == self.re_externVariable:
                pass
            elif item == self.re_functionDefinition:
                newtxt, junk = self.parseFunction(match, txt)
            elif item == self.re_attributesPlaceholder:
                pass # ignore attributes on e.g typedef or global
            elif item == self.re_spuriousSemicolon:
                pass
            else:
                print "NO_ACTION", match.group(), item
                print "***", txt

            oldTxtLength = len(txt)
            if newtxt:
                txt = newtxt
            else:
                txt = txt[match.span()[1]:]
            if len(txt) >= oldTxtLength:
                raise ("*** Breaking from infinite loop ***\n" \
                    "While parsing '%s'\n" \
                    "'%s'" % (self.current_filename, txt) )
        return document

    def parseFile(self, filename):
        self.debug("*************************\n*** %s\n*************************" % filename)
        
        self.current_filename = filename
        txt = open(filename).read()
        document = self.parseString(txt)
        self.current_filename = None
        return document

def headerToDom(filename):
    return CppParser().parseFile( filename )

def usage():
    import os
    print 'Usage: %s <filename.h>\n' % os.path.basename(sys.argv[0])
    print 'Generate a class dom from the specified header file'
    print 'Options:'
    print '  -h --help    print this help text'

def main():
    args = sys.argv[1:]
    if ("-h" in args) or ("--help" in args):
        usage()
        sys.exit(0)
    if len(args) < 1:
        usage()
        sys.exit(1)
    for arg in args:
        document = CppParser().parseFile( arg )
        print >>sys.stderr, document

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
