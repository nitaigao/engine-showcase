"""
Use this script to update ../hkgFontExtraPNG.cpp after hkg_font_demoextra.png has been updated.
"""

import sys

pngFile = "hkg_font_demoextra.png"
pngData = open(pngFile, 'rb').read()

print "const unsigned int hkgFontExtraPNGSize = %s;" % str(len(pngData))
print "const unsigned char hkgFontExtraPNGData [] = { "

for byteIndex in range(len(pngData)):
    print str( '%s0x%02X' % ( [',',' '][byteIndex==0], ord(pngData[byteIndex]) ) ).lower(),
    if (byteIndex % 10 == 9):
        print ''
print ' };'
