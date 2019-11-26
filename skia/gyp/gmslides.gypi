# include this gypi to include all the golden master slides.
{
  'include_dirs': [
    '../gm',
    # include dirs needed by particular GMs
    '../src/utils/debugger',
    '../src/images',
    '../src/lazy',
  ],
  'sources': [
    # base class for GMs
    '../gm/gm.cpp',
    '../gm/gm.h',

    '../gm/aaclip.cpp',
    '../gm/aarectmodes.cpp',
    '../gm/alphagradients.cpp',
    '../gm/arcofzorro.cpp',
    '../gm/arithmode.cpp',
    '../gm/beziereffects.cpp',
    '../gm/bicubicfilter.cpp',
    '../gm/bigblurs.cpp',
    '../gm/bigmatrix.cpp',
    '../gm/bigtext.cpp',
    '../gm/bitmapcopy.cpp',
    '../gm/bitmapmatrix.cpp',
    '../gm/bitmapfilters.cpp',
    '../gm/bitmappremul.cpp',
    '../gm/bitmaprect.cpp',
    '../gm/bitmaprecttest.cpp',
    '../gm/bitmapscroll.cpp',
    '../gm/bitmapshader.cpp',
    '../gm/bitmapsource.cpp',
    '../gm/bleed.cpp',
    '../gm/blurcircles.cpp',
    '../gm/blurs.cpp',
    '../gm/blurquickreject.cpp',
    '../gm/blurrect.cpp',
    '../gm/blurroundrect.cpp',
    '../gm/canvasstate.cpp',
    '../gm/circles.cpp',
    '../gm/circularclips.cpp',
    '../gm/clippedbitmapshaders.cpp',
    '../gm/coloremoji.cpp',
    '../gm/colorfilterimagefilter.cpp',
    '../gm/colorfilters.cpp',
    '../gm/colormatrix.cpp',
    '../gm/colortype.cpp',
    '../gm/complexclip.cpp',
    '../gm/complexclip2.cpp',
    '../gm/composeshader.cpp',
    #'../gm/conicpaths.cpp',
    '../gm/convexpaths.cpp',
    '../gm/convexpolyclip.cpp',
    '../gm/convexpolyeffect.cpp',
    '../gm/copyTo4444.cpp',
    '../gm/cubicpaths.cpp',
    '../gm/cmykjpeg.cpp',
    '../gm/degeneratesegments.cpp',
    '../gm/discard.cpp',
    '../gm/dashcubics.cpp',
    '../gm/dashing.cpp',
    '../gm/deviceproperties.cpp',
    '../gm/distantclip.cpp',
    '../gm/displacement.cpp',
    '../gm/downsamplebitmap.cpp',
    '../gm/drawbitmaprect.cpp',
    '../gm/drawlooper.cpp',
    '../gm/dropshadowimagefilter.cpp',
    '../gm/drrect.cpp',
    '../gm/etc1bitmap.cpp',
    '../gm/extractbitmap.cpp',
    '../gm/emptypath.cpp',
    '../gm/fatpathfill.cpp',
    '../gm/factory.cpp',
    '../gm/filltypes.cpp',
    '../gm/filltypespersp.cpp',
    '../gm/filterbitmap.cpp',
    '../gm/filterindiabox.cpp',
    '../gm/fontcache.cpp',
    '../gm/fontmgr.cpp',
    '../gm/fontscaler.cpp',
    '../gm/gammatext.cpp',
    '../gm/getpostextpath.cpp',
    '../gm/giantbitmap.cpp',
    '../gm/gradients.cpp',
    '../gm/gradients_2pt_conical.cpp',
    '../gm/gradients_no_texture.cpp',
    '../gm/gradientDirtyLaundry.cpp',
    '../gm/gradient_matrix.cpp',
    '../gm/gradtext.cpp',
    '../gm/hairlines.cpp',
    '../gm/hairmodes.cpp',
    '../gm/hittestpath.cpp',
    '../gm/imagealphathreshold.cpp',
    '../gm/imageblur.cpp',
    '../gm/imageblurtiled.cpp',
    '../gm/imagemagnifier.cpp',
    '../gm/imageresizetiled.cpp',
    '../gm/inversepaths.cpp',
    '../gm/lerpmode.cpp',
    '../gm/lighting.cpp',
    '../gm/lumafilter.cpp',
    '../gm/image.cpp',
    '../gm/imagefiltersbase.cpp',
    '../gm/imagefiltersclipped.cpp',
    '../gm/imagefilterscropped.cpp',
    '../gm/imagefilterscropexpand.cpp',
    '../gm/imagefiltersgraph.cpp',
    '../gm/imagefiltersscaled.cpp',
    '../gm/internal_links.cpp',
    '../gm/lcdtext.cpp',
    '../gm/linepaths.cpp',
    '../gm/matrixconvolution.cpp',
    '../gm/matriximagefilter.cpp',
    '../gm/megalooper.cpp',
    '../gm/mixedxfermodes.cpp',
    '../gm/modecolorfilters.cpp',
    '../gm/morphology.cpp',
    '../gm/nested.cpp',
    '../gm/ninepatchstretch.cpp',
    '../gm/nonclosedpaths.cpp',
    '../gm/offsetimagefilter.cpp',
    '../gm/optimizations.cpp',
    '../gm/ovals.cpp',
    '../gm/patheffects.cpp',
    '../gm/pathfill.cpp',
    '../gm/pathinterior.cpp',
    '../gm/pathopsinverse.cpp',
    '../gm/pathopsskpclip.cpp',
    '../gm/pathreverse.cpp',
    '../gm/peekpixels.cpp',
    '../gm/perlinnoise.cpp',
    '../gm/pictureimagefilter.cpp',
    '../gm/pictureshader.cpp',
    '../gm/points.cpp',
    '../gm/poly2poly.cpp',
    '../gm/polygons.cpp',
    '../gm/quadpaths.cpp',
    '../gm/rects.cpp',
    '../gm/resizeimagefilter.cpp',
    '../gm/rrect.cpp',
    '../gm/rrects.cpp',
    '../gm/roundrects.cpp',
    '../gm/samplerstress.cpp',
    # '../gm/scalebitmap.cpp',
    '../gm/shaderbounds.cpp',
    '../gm/selftest.cpp',
    '../gm/shadertext.cpp',
    '../gm/shadertext2.cpp',
    '../gm/shadertext3.cpp',
    '../gm/shadows.cpp',
    '../gm/shallowgradient.cpp',
    '../gm/simpleaaclip.cpp',
    '../gm/skbug1719.cpp',
    '../gm/stringart.cpp',
    '../gm/spritebitmap.cpp',
    '../gm/srcmode.cpp',
    '../gm/strokefill.cpp',
    '../gm/strokerect.cpp',
    '../gm/strokerects.cpp',
    '../gm/strokes.cpp',
    '../gm/stroketext.cpp',
    '../gm/tablecolorfilter.cpp',
    '../gm/texteffects.cpp',
    '../gm/testimagefilters.cpp',
    '../gm/texdata.cpp',
    '../gm/variedtext.cpp',
    '../gm/texturedomaineffect.cpp',
    '../gm/thinrects.cpp',
    '../gm/thinstrokedrects.cpp',
    '../gm/tileimagefilter.cpp',
    '../gm/tilemodes.cpp',
    '../gm/tilemodes_scaled.cpp',
    '../gm/tinybitmap.cpp',
    '../gm/twopointradial.cpp',
    '../gm/typeface.cpp',
    '../gm/vertices.cpp',
    '../gm/verttext.cpp',
    '../gm/verttext2.cpp',
    '../gm/verylargebitmap.cpp',
    '../gm/xfermodeimagefilter.cpp',
    '../gm/xfermodes.cpp',
    '../gm/xfermodes2.cpp',
    '../gm/xfermodes3.cpp',

    # Files needed by particular GMs
    '../src/utils/debugger/SkDrawCommand.h',
    '../src/utils/debugger/SkDrawCommand.cpp',
    '../src/utils/debugger/SkDebugCanvas.h',
    '../src/utils/debugger/SkDebugCanvas.cpp',
    '../src/utils/debugger/SkObjectParser.h',
    '../src/utils/debugger/SkObjectParser.cpp',

  ],
  'conditions': [
    # TODO: Several GMs are known to cause particular problems on Android, so
    # we disable them on Android.  See http://skbug.com/2326
    [ 'skia_os == "android"', {
      'sources!': [
        # TODO(borenet): Causes assertion failure on Nexus S.
        # See http://skbug.com/705
        '../gm/bitmapcopy.cpp',

        # SOME of the bitmaprect tests are disabled on Android; see
        # ../gm/bitmaprect.cpp

        # Fail for now until the appropriate freetype changes are submitted.
        '../gm/coloremoji.cpp',

        # We skip GPU tests in this GM; see
        # ../gm/deviceproperties.cpp

        # TODO(bsalomon): Hangs on Xoom and Nexus S. See http://skbug.com/637
        '../gm/drawbitmaprect.cpp',

        # TODO(epoger): Crashes on Nexus 10. See http://skbug.com/2313
        '../gm/imagefilterscropexpand.cpp',

        # TODO(borenet): Causes Nexus S to reboot. See http://skbug.com/665
        '../gm/shadertext.cpp',
        '../gm/shadertext2.cpp',
        '../gm/shadertext3.cpp',

        # TODO(reed): Allocates more memory than Android devices are capable of
        # fulfilling. See http://skbug.com/1978
        '../gm/verylargebitmap.cpp',
      ],

      'sources': [
        '../gm/androidfallback.cpp',
      ],
    }],
  ],
}
