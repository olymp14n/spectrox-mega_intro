#!/bin/bash

[ $# -ne 2 ] && {
    echo "Usage:";
    echo "  $0 <file> <name>"
    echo ""
    echo "Arguments:"
    echo "  file  Bitmap font metadata in fmt format."
    echo "  name  Name of the font strucs."
    exit 1;
}

file_name=$1
font_name=$2

# Glyphs
echo "fw_font_glyph _${font_name}_fontGlyphs[] = {"
cat ${file_name} | while read line
do
    if [[ "$line" =~ "char " ]]; then
        id=$(echo $line | perl -pe 's|(.*id=)(.*?)( .*)|\2|')
        x=$(echo $line | perl -pe 's|(.*x=)(.*?)( .*)|\2|')
        y=$(echo $line | perl -pe 's|(.*y=)(.*?)( .*)|\2|')
        width=$(echo $line | perl -pe 's|(.*width=)(.*?)( .*)|\2|')
        height=$(echo $line | perl -pe 's|(.*height=)(.*?)( .*)|\2|')
        xoffset=$(echo $line | perl -pe 's|(.*xoffset=)(.*?)( .*)|\2|')
        yoffset=$(echo $line | perl -pe 's|(.*yoffset=)(.*?)( .*)|\2|')
        xadvance=$(echo $line | perl -pe 's|(.*xadvance=)(.*?)( .*)|\2|')
        echo "    [$id] = {$x, $y, $width, $height, $xoffset, $yoffset, $xadvance},"
    fi
done
echo "};"
echo ""

# Face
echo "fw_font_face _${font_name}_fontFace = {"
cat ${file_name} | while read line
do
    if [[ "$line" =~ "common " ]]; then
        lineHeight=$(echo $line | perl -pe 's|(.*lineHeight=)(.*?)( .*)|\2|')
        scaleW=$(echo $line | perl -pe 's|(.*scaleW=)(.*?)( .*)|\2|')
        scaleH=$(echo $line | perl -pe 's|(.*scaleH=)(.*?)( .*)|\2|')
        echo "    .lineHeight = $lineHeight,"
        echo "    .textureWidth = $scaleW,"
        echo "    .textureHeight = $scaleH,"
        echo "    .glyphs = _${font_name}_fontGlyphs,"
        echo "    .numGlyphs = (sizeof(_${font_name}_fontGlyphs) / sizeof(_${font_name}_fontGlyphs[0])),"
    fi
done
echo "};"