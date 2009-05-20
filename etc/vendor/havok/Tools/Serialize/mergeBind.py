#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###
### Script to merge two data bindings. This script ensures that changes to the
### hkclass files are propagated each time the header is scanned with headerToDom.py
###

__all__ = ["mergeBind"]

import bindingToXml

def mergeBind(master, edited, context):
    """Merge changes from "edited" into "master"
    """
    rejects = []
    for slot in master.__slots__:
        mitem = getattr(master,slot)
        eitem = getattr(edited,slot)
        if type(mitem) in (type(""), type(u"")):
            if not mitem:
                setattr(master, slot, eitem)               
            elif mitem != eitem:
                rejects.append("<!-- +++CONFLICT+++\t(%s.%s)\n%s\n%s\n-->" % (context, slot, mitem, eitem))
        elif type(mitem) in (type(0), type(0.0)):
            if mitem != eitem:
                rejects.append("<!-- +++CONFLICT+++\t(%s.%s)\n%s\n%s\n-->" % (context, slot, mitem, eitem))
        elif type(mitem) == type([]):
            edict = {}
            for e in eitem:
                edict[e.name] = e
            for m in mitem:
                #print '\n\n\n slot:%s\n e: %s\n eitem: %s\n m: %s\n mitem: %s' % (slot, e,eitem,m,mitem)
                e = edict.get(m.name, None)
                if e:
                    eitem.remove(e)
                    rejects += mergeBind(m,e,'%s.%s' % (context, m.name))
            for e in eitem:
                rejects.append("<!-- +++UNUSED+++\t(%s.%s)\n%s\n-->" % (context, slot, bindingToXml.bindingToXml(slot,e)))
        elif mitem:
            rejects += mergeBind(mitem, eitem,'%s.%s' % (context, mitem.name))
        elif eitem:
            setattr(master, slot, eitem)
    return rejects

def main():
    import hkcToDom
    import headerToDom
    
    master = headerToDom.headerToDom( r'E:\DEV\serialize\havok\source\hkbase\class\hkClass.h' )
    edited = hkcToDom.hkcToDom( "edited.hkclass" )

    print >>open("master.hkclass","w"), bindingToXml.bindingToXml("file",master.file)
    rejectFileName = "masterXml.rej"
    print >>open(rejectFileName,"w"), '\n'.join(mergeBind(master.file, edited.file, master.name))
    print >>open("merged.hkclass","w"), bindingToXml.bindingToXml("file",master.file)

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
