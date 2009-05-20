#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

import os
import re

def findSymbols(topLevel, neededSymbols, options):
    externsFromPath = {}
    pathFromSymbol = {}
    
    for where, dirs, files in os.walk(topLevel):
        for file in [f for f in files if f.endswith("%s.cpp"% options.version_suffix)]:
            path = os.path.join(where,file)
            txt = open(path).read()
            symbols = re.findall(r"^hkClass\s+(\w+)\s*\(\s*$(?m)", txt)
            for symbol in symbols:
                pathFromSymbol[symbol] = path
            externsFromPath[path] = re.findall(r"^extern\s+hkClass\s+(\w+)\s*;$(?m)", txt)

    donePaths = {}
    doneSymbols = {}
    while neededSymbols:
        symbol = neededSymbols.pop(0)
        try:
            path = pathFromSymbol[symbol]
        except KeyError:
            pass
        else:
            if donePaths.has_key(path):
                continue
            print path
            donePaths[path] = 1
            doneSymbols[symbol] = 1
            neededSymbols += [p for p in externsFromPath[path] if doneSymbols.get(p,0) == 0]

USAGE = """%prog [WHERE]...

"""

def main():
    import optparse
    parser = optparse.OptionParser(usage=USAGE)
    parser.add_option("--version-suffix", default="", help="Enable versioning mode")
    parser.add_option("--search-dir", default=".", help="Dir to search")
    options, args = parser.parse_args()
    assert options.version_suffix
    neededSymbols = [ "%sClass%s" % (a,options.version_suffix) for a in args ]
    findSymbols(options.search_dir, neededSymbols, options)

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
