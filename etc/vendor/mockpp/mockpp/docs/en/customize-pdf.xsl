<?xml version='1.0'?>

<!-- Customisation for pdf-files

     $Id: customize-pdf.xsl 1400 2007-06-10 08:51:16Z ewald-arnold $
-->

<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  version="1.0">

  <xsl:import href="http://docbook.sourceforge.net/release/xsl/current/fo/docbook.xsl"/>

  <xsl:param name="draft.mode"                             select="'no'"/>
  <xsl:param name="admon.graphics"                         select="1"/>
  <xsl:param name="linenumbering.extension"                select="1"/>
  <xsl:param name="chapter.autolabel"                      select="1"/>
  <xsl:param name="appendix.autolabel"                     select="1"/>
  <xsl:param name="section.autolabel"                      select="1"/>
  <xsl:param name="section.label.includes.component.label" select="1"/>
  <xsl:param name="section.autolabel.max.depth"            select="2"/>

  <xsl:param name="callout.graphics"                       select="'0'"/>
  <xsl:param name="callout.graphics.number.limit"          select="'15'"/>
  <!-- xsl:param name="callout.unicode.number.limit"           select="'20'"/ -->
  <xsl:param name="callout.unicode"                        select="'1'"/>
  <!-- xsl:param name="callout.unicode.start.character"        select="'10102'"/ -->
  <!--  xsl:param name="callout.unicode.font"                   select="'Libertine'"/ -->

  <xsl:param name="use.id.as.filename"                     select="'1'"/>
  <xsl:param name="use.extensions"                         select="'0'"/>
  <!-- xsl:param name="title.margin.left"                      select="'0'"/ -->
  <xsl:param name="body.start.indent"                      select="'0'"/>
  <xsl:param name="toc.indent.width"                       select="'12'"/>

<!--
  <xsl:attribute-set name="monospace.verbatim.properties">
    <xsl:attribute name="border">0.5pt solid blue</xsl:attribute>
    <xsl:attribute name="background-color">#E0E0E0</xsl:attribute>
    <xsl:attribute name="padding">8pt</xsl:attribute>
  </xsl:attribute-set>
-->

  <xsl:template match="programlisting|screen">
    <fo:block white-space-collapse='false'
              white-space-treatment='preserve'
              linefeed-treatment='preserve'
              xsl:use-attribute-sets="monospace.verbatim.properties"
              background-color="#f0f0f0"
              border="0.5pt solid #e0e0E0"
              padding="3pt">
      <xsl:apply-templates/>

    </fo:block>
  </xsl:template>


  <xsl:template match="*" mode="admon.graphic.width">
    <xsl:text>24pt</xsl:text>
  </xsl:template>

</xsl:stylesheet>
