#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

## /usr/bin/env python

import re

HK_REFLECTION_RE = re.compile(r"^\s*HK_DECLARE_REFLECTION\s*\(\s*\)\s*;", re.M)

def hasReflectionDeclaration(txt):
	return HK_REFLECTION_RE.search(txt)

def mergeTags(tags1, tags2):
    tagslist1 = tags1.replace("+"," ").split()
    tagslist2 = tags2.replace("+"," ").split()
    if "ALL" in tagslist1 or "ALL" in tagslist2:
        return "ALL"
    tags = tagslist1
    for t2 in tagslist2:
        if not t2 in tags:
            tags.append(t2)
    return "+".join(tags)

HK_TKBMS_HEADER_RE = re.compile("// TKBMS v1.0.+?TKBMS v1.0", re.MULTILINE | re.DOTALL)
HK_TKBMS_TAGS_RE = re.compile("(\w[\w\d_]*)\s*:\s*(.+)")
def extract_tkbms(txt):
    try:
        tkbms = HK_TKBMS_HEADER_RE.search(txt).group(0)
    except AttributeError:
        return {}
    else:
        tags = {}
        for line in tkbms.split("\n"):
            match = HK_TKBMS_TAGS_RE.search(line)
            if match:
                (var,val) = match.group(1,2)
                tags[var.lower()] = val.strip()
        return tags
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
