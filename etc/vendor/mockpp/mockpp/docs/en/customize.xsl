<?xml version='1.0'?>

<!-- Common custumisation options

     $Id: customize.xsl 1015 2005-12-03 18:13:00Z ewald-arnold $
-->

<xsl:stylesheet  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  version="1.0">

<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/html/chunk.xsl"/>

<!--
<xsl:template name="user.footer.content">
  <HR/><center><P class="copyright">&#x00A9; 2002  Megacorp, Inc.</P>
  <br />
  </center>
</xsl:template>

user.header.navigation  Called before standard navigational header.

header.navigation   The standard navigational header.

user.header.content  Called after standard navigational header but before any other content.

user.footer.content  Called after the chunk content but before the standard navigational footer.

footer.navigation  The standard navigational footer.

user.footer.navigation  Called after the standard navigational footer
-->

<xsl:param name="html.stylesheet"                        select="'common/mockpp.css'"/>
<xsl:param name="admon.graphics"                         select="1"/>
<xsl:param name="linenumbering.extension"                select="1"/>
<xsl:param name="chapter.autolabel"                      select="1"/>
<xsl:param name="appendix.autolabel"                     select="1"/>
<xsl:param name="section.autolabel"                      select="1"/>
<!-- xsl:param name="section.autolabel.max.depth"            select="4"/ -->
<xsl:param name="section.label.includes.component.label" select="1"/>
<xsl:param name="callout.graphics.number.limit"          select="'15'"/>
<xsl:param name="use.id.as.filename"                     select="'1'"/>
<xsl:param name="use.extensions"                         select="'0'"/>

</xsl:stylesheet>

