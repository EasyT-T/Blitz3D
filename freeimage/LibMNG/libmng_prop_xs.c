/* ************************************************************************** */
/* *             For conditions of distribution and use,                    * */
/* *                see copyright notice in libmng.h                        * */
/* ************************************************************************** */
/* *                                                                        * */
/* * project   : libmng                                                     * */
/* * file      : libmng_prop_xs.c          copyright (c) 2000 G.Juyn        * */
/* * version   : 1.0.2                                                      * */
/* *                                                                        * */
/* * purpose   : property get/set interface (implementation)                * */
/* *                                                                        * */
/* * author    : G.Juyn                                                     * */
/* * web       : http://www.3-t.com                                         * */
/* * email     : mailto:info@3-t.com                                        * */
/* *                                                                        * */
/* * comment   : implementation of the property get/set functions           * */
/* *                                                                        * */
/* * changes   : 0.5.1 - 05/08/2000 - G.Juyn                                * */
/* *             - fixed calling convention                                 * */
/* *             - changed strict-ANSI stuff                                * */
/* *             0.5.1 - 05/11/2000 - G.Juyn                                * */
/* *             - added set_outputprofile2 & set_srgbprofile2              * */
/* *             0.5.1 - 05/12/2000 - G.Juyn                                * */
/* *             - changed trace to macro for callback error-reporting      * */
/* *                                                                        * */
/* *             0.5.2 - 05/23/2000 - G.Juyn                                * */
/* *             - changed inclusion of cms-routines                        * */
/* *             0.5.2 - 05/24/2000 - G.Juyn                                * */
/* *             - added support for get/set default zlib/IJG parms         * */
/* *             0.5.2 - 05/31/2000 - G.Juyn                                * */
/* *             - fixed up punctuation (contribution by Tim Rowley)        * */
/* *             0.5.2 - 06/05/2000 - G.Juyn                                * */
/* *             - added support for RGB8_A8 canvasstyle                    * */
/* *                                                                        * */
/* *             0.5.3 - 06/21/2000 - G.Juyn                                * */
/* *             - added get/set for speedtype to facilitate testing        * */
/* *             - added get for imagelevel during processtext callback     * */
/* *             0.5.3 - 06/26/2000 - G.Juyn                                * */
/* *             - changed userdata variable to mng_ptr                     * */
/* *             0.5.3 - 06/29/2000 - G.Juyn                                * */
/* *             - fixed incompatible return-types                          * */
/* *                                                                        * */
/* *             0.9.1 - 07/08/2000 - G.Juyn                                * */
/* *             - added get routines for internal display variables        * */
/* *             - added get/set routines for suspensionmode variable       * */
/* *             0.9.1 - 07/15/2000 - G.Juyn                                * */
/* *             - added get/set routines for sectionbreak variable         * */
/* *                                                                        * */
/* *             0.9.2 - 07/31/2000 - G.Juyn                                * */
/* *             - added status_xxxx functions                              * */
/* *             0.9.2 - 08/05/2000 - G.Juyn                                * */
/* *             - changed file-prefixes                                    * */
/* *                                                                        * */
/* *             0.9.3 - 10/10/2000 - G.Juyn                                * */
/* *             - added support for alpha-depth prediction                 * */
/* *             0.9.3 - 10/16/2000 - G.Juyn                                * */
/* *             - added functions to retrieve PNG/JNG specific header-info * */
/* *             0.9.3 - 10/20/2000 - G.Juyn                                * */
/* *             - added get/set for bKGD preference setting                * */
/* *             0.9.3 - 10/21/2000 - G.Juyn                                * */
/* *             - added get function for interlace/progressive display     * */
/* *                                                                        * */
/* *             1.0.1 - 04/21/2001 - G.Juyn (code by G.Kelly)              * */
/* *             - added BGRA8 canvas with premultiplied alpha              * */
/* *             1.0.1 - 05/02/2001 - G.Juyn                                * */
/* *             - added "default" sRGB generation (Thanks Marti!)          * */
/* *                                                                        * */
/* *             1.0.2 - 06/23/2001 - G.Juyn                                * */
/* *             - added optimization option for MNG-video playback         * */
/* *             1.0.2 - 06/25/2001 - G.Juyn                                * */
/* *             - added option to turn off progressive refresh             * */
/* *                                                                        * */
/* ************************************************************************** */

#include "libmng.h"
#include "libmng_data.h"
#include "libmng_error.h"
#include "libmng_trace.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "libmng_cms.h"

#if defined(__BORLANDC__) && defined(MNG_STRICT_ANSI)
#pragma option -A                      /* force ANSI-C */
#endif

/* ************************************************************************** */
/* *                                                                        * */
/* *  Property set functions                                                * */
/* *                                                                        * */
/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_userdata(mng_handle hHandle,
                                      mng_ptr pUserdata) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_USERDATA, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->pUserdata = pUserdata;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_USERDATA, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_canvasstyle(mng_handle hHandle,
                                         mng_uint32 iStyle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_CANVASSTYLE, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)

    switch (iStyle) {
        case MNG_CANVAS_RGB8    :
            break;
        case MNG_CANVAS_RGBA8   :
            break;
        case MNG_CANVAS_ARGB8   :
            break;
        case MNG_CANVAS_RGB8_A8 :
            break;
        case MNG_CANVAS_BGR8    :
            break;
        case MNG_CANVAS_BGRA8   :
            break;
        case MNG_CANVAS_BGRA8PM :
            break;
        case MNG_CANVAS_ABGR8   :
            break;
/*    case MNG_CANVAS_RGB16   : break; */
/*    case MNG_CANVAS_RGBA16  : break; */
/*    case MNG_CANVAS_ARGB16  : break; */
/*    case MNG_CANVAS_BGR16   : break; */
/*    case MNG_CANVAS_BGRA16  : break; */
/*    case MNG_CANVAS_ABGR16  : break; */
/*    case MNG_CANVAS_INDEX8  : break; */
/*    case MNG_CANVAS_INDEXA8 : break; */
/*    case MNG_CANVAS_AINDEX8 : break; */
/*    case MNG_CANVAS_GRAY8   : break; */
/*    case MNG_CANVAS_GRAY16  : break; */
/*    case MNG_CANVAS_GRAYA8  : break; */
/*    case MNG_CANVAS_GRAYA16 : break; */
/*    case MNG_CANVAS_AGRAY8  : break; */
/*    case MNG_CANVAS_AGRAY16 : break; */
/*    case MNG_CANVAS_DX15    : break; */
/*    case MNG_CANVAS_DX16    : break; */
        default                 : {
            MNG_ERROR (((mng_datap) hHandle), MNG_INVALIDCNVSTYLE)
        }
    }

    ((mng_datap) hHandle)->iCanvasstyle = iStyle;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_CANVASSTYLE, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_bkgdstyle(mng_handle hHandle,
                                       mng_uint32 iStyle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_BKGDSTYLE, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)

    switch (iStyle)                      /* alpha-modes not supported */
    {
        case MNG_CANVAS_RGB8    :
            break;
        case MNG_CANVAS_BGR8    :
            break;
/*    case MNG_CANVAS_RGB16   : break; */
/*    case MNG_CANVAS_BGR16   : break; */
/*    case MNG_CANVAS_INDEX8  : break; */
/*    case MNG_CANVAS_GRAY8   : break; */
/*    case MNG_CANVAS_GRAY16  : break; */
/*    case MNG_CANVAS_DX15    : break; */
/*    case MNG_CANVAS_DX16    : break; */
        default                 : MNG_ERROR (((mng_datap) hHandle), MNG_INVALIDCNVSTYLE)
    }

    ((mng_datap) hHandle)->iBkgdstyle = iStyle;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_BKGDSTYLE, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_bgcolor(mng_handle hHandle,
                                     mng_uint16 iRed,
                                     mng_uint16 iGreen,
                                     mng_uint16 iBlue) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_BGCOLOR, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iBGred = iRed;
    ((mng_datap) hHandle)->iBGgreen = iGreen;
    ((mng_datap) hHandle)->iBGblue = iBlue;
    ((mng_datap) hHandle)->bUseBKGD = MNG_FALSE;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_BGCOLOR, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_usebkgd(mng_handle hHandle,
                                     mng_bool bUseBKGD) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_USEBKGD, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->bUseBKGD = bUseBKGD;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_USEBKGD, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_storechunks(mng_handle hHandle,
                                         mng_bool bStorechunks) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_STORECHUNKS, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->bStorechunks = bStorechunks;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_STORECHUNKS, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_sectionbreaks(mng_handle hHandle,
                                           mng_bool bSectionbreaks) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SECTIONBREAKS, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->bSectionbreaks = bSectionbreaks;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SECTIONBREAKS, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_cacheplayback(mng_handle hHandle,
                                           mng_bool bCacheplayback) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_CACHEPLAYBACK, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)

    if (((mng_datap) hHandle)->bHasheader) MNG_ERROR (((mng_datap) hHandle), MNG_FUNCTIONINVALID)

    ((mng_datap) hHandle)->bCacheplayback = bCacheplayback;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_CACHEPLAYBACK, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_doprogressive(mng_handle hHandle,
                                           mng_bool bDoProgressive) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_DOPROGRESSIVE, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)

    ((mng_datap) hHandle)->bDoProgressive = bDoProgressive;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_DOPROGRESSIVE, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_retcode MNG_DECL mng_set_srgb(mng_handle hHandle,
                                  mng_bool bIssRGB) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SRGB, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->bIssRGB = bIssRGB;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SRGB, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_retcode MNG_DECL mng_set_outputprofile(mng_handle hHandle,
                                           mng_pchar zFilename) {
#ifdef MNG_INCLUDE_LCMS
    mng_datap pData;
#endif

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_OUTPUTPROFILE, MNG_LC_START)
#endif

#ifdef MNG_INCLUDE_LCMS
    MNG_VALIDHANDLE (hHandle)

    pData = (mng_datap)hHandle;          /* address the structure */

    if (pData->hProf2)                   /* previously defined ? */
      mnglcms_freeprofile (pData->hProf2);
                                         /* allocate new CMS profile handle */
    pData->hProf2 = mnglcms_createfileprofile (zFilename);

    if (!pData->hProf2)                  /* handle error ? */
      MNG_ERRORL (pData, MNG_LCMS_NOHANDLE)
#endif /* MNG_INCLUDE_LCMS */

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_OUTPUTPROFILE, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_retcode MNG_DECL mng_set_outputprofile2(mng_handle hHandle,
                                            mng_uint32 iProfilesize,
                                            mng_ptr pProfile) {
#ifdef MNG_INCLUDE_LCMS
    mng_datap pData;
#endif

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_OUTPUTPROFILE2, MNG_LC_START)
#endif

#ifdef MNG_INCLUDE_LCMS
    MNG_VALIDHANDLE (hHandle)

    pData = (mng_datap)hHandle;          /* address the structure */

    if (pData->hProf2)                   /* previously defined ? */
      mnglcms_freeprofile (pData->hProf2);
                                         /* allocate new CMS profile handle */
    pData->hProf2 = mnglcms_creatememprofile (iProfilesize, pProfile);

    if (!pData->hProf2)                  /* handle error ? */
      MNG_ERRORL (pData, MNG_LCMS_NOHANDLE)
#endif /* MNG_INCLUDE_LCMS */

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_OUTPUTPROFILE2, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_outputsrgb(mng_handle hHandle) {
#ifdef MNG_INCLUDE_LCMS
    mng_datap pData;
#endif

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_OUTPUTSRGB, MNG_LC_START)
#endif

#ifdef MNG_INCLUDE_LCMS
    MNG_VALIDHANDLE (hHandle)

    pData = (mng_datap)hHandle;          /* address the structure */

    if (pData->hProf2)                   /* previously defined ? */
      mnglcms_freeprofile (pData->hProf2);
                                         /* allocate new CMS profile handle */
    pData->hProf2 = mnglcms_createsrgbprofile ();

    if (!pData->hProf2)                  /* handle error ? */
      MNG_ERRORL (pData, MNG_LCMS_NOHANDLE)
#endif /* MNG_INCLUDE_LCMS */

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_OUTPUTSRGB, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_retcode MNG_DECL mng_set_srgbprofile(mng_handle hHandle,
                                         mng_pchar zFilename) {
#ifdef MNG_INCLUDE_LCMS
    mng_datap pData;
#endif

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SRGBPROFILE2, MNG_LC_START)
#endif

#ifdef MNG_INCLUDE_LCMS
    MNG_VALIDHANDLE (hHandle)

    pData = (mng_datap)hHandle;          /* address the structure */

    if (pData->hProf3)                   /* previously defined ? */
      mnglcms_freeprofile (pData->hProf3);
                                         /* allocate new CMS profile handle */
    pData->hProf3 = mnglcms_createfileprofile (zFilename);

    if (!pData->hProf3)                  /* handle error ? */
      MNG_ERRORL (pData, MNG_LCMS_NOHANDLE)
#endif /* MNG_INCLUDE_LCMS */

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SRGBPROFILE2, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_retcode MNG_DECL mng_set_srgbprofile2(mng_handle hHandle,
                                          mng_uint32 iProfilesize,
                                          mng_ptr pProfile) {
#ifdef MNG_INCLUDE_LCMS
    mng_datap pData;
#endif

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SRGBPROFILE, MNG_LC_START)
#endif

#ifdef MNG_INCLUDE_LCMS
    MNG_VALIDHANDLE (hHandle)

    pData = (mng_datap)hHandle;          /* address the structure */

    if (pData->hProf3)                   /* previously defined ? */
      mnglcms_freeprofile (pData->hProf3);
                                         /* allocate new CMS profile handle */
    pData->hProf3 = mnglcms_creatememprofile (iProfilesize, pProfile);

    if (!pData->hProf3)                  /* handle error ? */
      MNG_ERRORL (pData, MNG_LCMS_NOHANDLE)
#endif /* MNG_INCLUDE_LCMS */

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SRGBPROFILE, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_srgbimplicit(mng_handle hHandle) {
#ifdef MNG_INCLUDE_LCMS
    mng_datap pData;
#endif

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SRGBIMPLICIT, MNG_LC_START)
#endif

#ifdef MNG_INCLUDE_LCMS
    MNG_VALIDHANDLE (hHandle)

    pData = (mng_datap)hHandle;          /* address the structure */

    if (pData->hProf3)                   /* previously defined ? */
      mnglcms_freeprofile (pData->hProf3);
                                         /* allocate new CMS profile handle */
    pData->hProf3 = mnglcms_createsrgbprofile ();

    if (!pData->hProf3)                  /* handle error ? */
      MNG_ERRORL (pData, MNG_LCMS_NOHANDLE)
#endif /* MNG_INCLUDE_LCMS */

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SRGBIMPLICIT, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_viewgamma(mng_handle hHandle,
                                       mng_float dGamma) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_VIEWGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->dViewgamma = dGamma;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_VIEWGAMMA, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_displaygamma(mng_handle hHandle,
                                          mng_float dGamma) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_DISPLAYGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->dDisplaygamma = dGamma;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_DISPLAYGAMMA, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_dfltimggamma(mng_handle hHandle,
                                          mng_float dGamma) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_DFLTIMGGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->dDfltimggamma = dGamma;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_DFLTIMGGAMMA, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_viewgammaint(mng_handle hHandle,
                                          mng_uint32 iGamma) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_VIEWGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->dViewgamma = (mng_float) iGamma / 100000;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_VIEWGAMMA, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_displaygammaint(mng_handle hHandle,
                                             mng_uint32 iGamma) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_DISPLAYGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->dDisplaygamma = (mng_float) iGamma / 100000;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_DISPLAYGAMMA, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_dfltimggammaint(mng_handle hHandle,
                                             mng_uint32 iGamma) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_DFLTIMGGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->dDfltimggamma = (mng_float) iGamma / 100000;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_DFLTIMGGAMMA, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_maxcanvaswidth(mng_handle hHandle,
                                            mng_uint32 iMaxwidth) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_MAXCANVASWIDTH, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iMaxwidth = iMaxwidth;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_MAXCANVASWIDTH, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_maxcanvasheight(mng_handle hHandle,
                                             mng_uint32 iMaxheight) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_MAXCANVASHEIGHT, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iMaxheight = iMaxheight;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_MAXCANVASHEIGHT, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_set_maxcanvassize(mng_handle hHandle,
                                           mng_uint32 iMaxwidth,
                                           mng_uint32 iMaxheight) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_MAXCANVASSIZE, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iMaxwidth = iMaxwidth;
    ((mng_datap) hHandle)->iMaxheight = iMaxheight;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_MAXCANVASSIZE, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_retcode MNG_DECL mng_set_zlib_level(mng_handle hHandle,
                                        mng_int32 iZlevel) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_LEVEL, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iZlevel = iZlevel;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_LEVEL, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_retcode MNG_DECL mng_set_zlib_method(mng_handle hHandle,
                                         mng_int32 iZmethod) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_METHOD, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iZmethod = iZmethod;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_METHOD, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_retcode MNG_DECL mng_set_zlib_windowbits(mng_handle hHandle,
                                             mng_int32 iZwindowbits) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_WINDOWBITS, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iZwindowbits = iZwindowbits;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_WINDOWBITS, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_retcode MNG_DECL mng_set_zlib_memlevel(mng_handle hHandle,
                                           mng_int32 iZmemlevel) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_MEMLEVEL, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iZmemlevel = iZmemlevel;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_MEMLEVEL, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_retcode MNG_DECL mng_set_zlib_strategy(mng_handle hHandle,
                                           mng_int32 iZstrategy) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_STRATEGY, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iZstrategy = iZstrategy;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_STRATEGY, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_retcode MNG_DECL mng_set_zlib_maxidat(mng_handle hHandle,
                                          mng_uint32 iMaxIDAT) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_MAXIDAT, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iMaxIDAT = iMaxIDAT;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_ZLIB_MAXIDAT, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mng_retcode MNG_DECL mng_set_jpeg_dctmethod(mng_handle hHandle,
                                            mngjpeg_dctmethod eJPEGdctmethod) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_DCTMETHOD, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->eJPEGdctmethod = eJPEGdctmethod;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_DCTMETHOD, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mng_retcode MNG_DECL mng_set_jpeg_quality(mng_handle hHandle,
                                          mng_int32 iJPEGquality) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_QUALITY, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iJPEGquality = iJPEGquality;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_QUALITY, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mng_retcode MNG_DECL mng_set_jpeg_smoothing(mng_handle hHandle,
                                            mng_int32 iJPEGsmoothing) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_SMOOTHING, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iJPEGsmoothing = iJPEGsmoothing;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_SMOOTHING, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mng_retcode MNG_DECL mng_set_jpeg_progressive(mng_handle hHandle,
                                              mng_bool bJPEGprogressive) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_PROGRESSIVE, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->bJPEGcompressprogr = bJPEGprogressive;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_PROGRESSIVE, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mng_retcode MNG_DECL mng_set_jpeg_optimized(mng_handle hHandle,
                                            mng_bool bJPEGoptimized) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_OPTIMIZED, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->bJPEGcompressopt = bJPEGoptimized;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_OPTIMIZED, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mng_retcode MNG_DECL mng_set_jpeg_maxjdat(mng_handle hHandle,
                                          mng_uint32 iMaxJDAT) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_MAXJDAT, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iMaxJDAT = iMaxJDAT;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_JPEG_MAXJDAT, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_SUPPORT_READ

mng_retcode MNG_DECL mng_set_suspensionmode(mng_handle hHandle,
                                            mng_bool bSuspensionmode) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SUSPENSIONMODE, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)

    if (((mng_datap) hHandle)->bReading)  /* we must NOT be reading !!! */
    MNG_ERROR ((mng_datap) hHandle, MNG_FUNCTIONINVALID)

    ((mng_datap) hHandle)->bSuspensionmode = bSuspensionmode;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SUSPENSIONMODE, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_SUPPORT_READ */

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_retcode MNG_DECL mng_set_speed(mng_handle hHandle,
                                   mng_speedtype iSpeed) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SPEED, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    ((mng_datap) hHandle)->iSpeed = iSpeed;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_SET_SPEED, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */
/* *                                                                        * */
/* *  Property get functions                                                * */
/* *                                                                        * */
/* ************************************************************************** */

mng_ptr MNG_DECL
mng_get_userdata(mng_handle hHandle) {                            /* no tracing in here to prevent recursive calls */
    MNG_VALIDHANDLEX (hHandle)
    return ((mng_datap) hHandle)->pUserdata;
}

/* ************************************************************************** */

mng_imgtype MNG_DECL mng_get_sigtype(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_SIGTYPE, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return mng_it_unknown;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_SIGTYPE, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->eSigtype;
}

/* ************************************************************************** */

mng_imgtype MNG_DECL mng_get_imagetype(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_IMAGETYPE, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return mng_it_unknown;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_IMAGETYPE, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->eImagetype;
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_imagewidth(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_IMAGEWIDTH, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_IMAGEWIDTH, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iWidth;
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_imageheight(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_IMAGEWIDTH, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_IMAGEHEIGHT, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iHeight;
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_ticks(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_TICKS, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_TICKS, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iTicks;
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_framecount(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_FRAMECOUNT, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_FRAMECOUNT, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iFramecount;
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_layercount(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_LAYERCOUNT, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_LAYERCOUNT, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iLayercount;
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_playtime(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_PLAYTIME, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_PLAYTIME, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iPlaytime;
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_simplicity(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_SIMPLICITY, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_SIMPLICITY, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iSimplicity;
}

/* ************************************************************************** */

mng_uint8 MNG_DECL mng_get_bitdepth(mng_handle hHandle) {
    mng_uint8 iRslt;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_BITDEPTH, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

    if (((mng_datap) hHandle)->eImagetype == mng_it_png)
        iRslt = ((mng_datap) hHandle)->iBitdepth;
    else if (((mng_datap) hHandle)->eImagetype == mng_it_jng)
        iRslt = ((mng_datap) hHandle)->iJHDRimgbitdepth;
    else
        iRslt = 0;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_BITDEPTH, MNG_LC_END)
#endif

    return iRslt;
}

/* ************************************************************************** */

mng_uint8 MNG_DECL mng_get_colortype(mng_handle hHandle) {
    mng_uint8 iRslt;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_COLORTYPE, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

    if (((mng_datap) hHandle)->eImagetype == mng_it_png)
        iRslt = ((mng_datap) hHandle)->iColortype;
    else if (((mng_datap) hHandle)->eImagetype == mng_it_jng)
        iRslt = ((mng_datap) hHandle)->iJHDRcolortype;
    else
        iRslt = 0;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_COLORTYPE, MNG_LC_END)
#endif

    return iRslt;
}

/* ************************************************************************** */

mng_uint8 MNG_DECL mng_get_compression(mng_handle hHandle) {
    mng_uint8 iRslt;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_COMPRESSION, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

    if (((mng_datap) hHandle)->eImagetype == mng_it_png)
        iRslt = ((mng_datap) hHandle)->iCompression;
    else if (((mng_datap) hHandle)->eImagetype == mng_it_jng)
        iRslt = ((mng_datap) hHandle)->iJHDRimgcompression;
    else
        iRslt = 0;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_COMPRESSION, MNG_LC_END)
#endif

    return iRslt;
}

/* ************************************************************************** */

mng_uint8 MNG_DECL mng_get_filter(mng_handle hHandle) {
    mng_uint8 iRslt;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_FILTER, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

    if (((mng_datap) hHandle)->eImagetype == mng_it_png)
        iRslt = ((mng_datap) hHandle)->iFilter;
    else
        iRslt = 0;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_FILTER, MNG_LC_END)
#endif

    return iRslt;
}

/* ************************************************************************** */

mng_uint8 MNG_DECL mng_get_interlace(mng_handle hHandle) {
    mng_uint8 iRslt;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_INTERLACE, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

    if (((mng_datap) hHandle)->eImagetype == mng_it_png)
        iRslt = ((mng_datap) hHandle)->iInterlace;
    else if (((mng_datap) hHandle)->eImagetype == mng_it_jng)
        iRslt = ((mng_datap) hHandle)->iJHDRimginterlace;
    else
        iRslt = 0;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_INTERLACE, MNG_LC_END)
#endif

    return iRslt;
}

/* ************************************************************************** */

mng_uint8 MNG_DECL mng_get_alphabitdepth(mng_handle hHandle) {
    mng_uint8 iRslt;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ALPHABITDEPTH, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

    if (((mng_datap) hHandle)->eImagetype == mng_it_jng)
        iRslt = ((mng_datap) hHandle)->iJHDRalphabitdepth;
    else
        iRslt = 0;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ALPHABITDEPTH, MNG_LC_END)
#endif

    return iRslt;
}

/* ************************************************************************** */

mng_uint8 MNG_DECL mng_get_refreshpass(mng_handle hHandle) {
    mng_uint8 iRslt;
    mng_datap pData;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_REFRESHPASS, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

    pData = (mng_datap) hHandle;
    /* for PNG we know the exact pass */
    if ((pData->eImagetype == mng_it_png) && (pData->iPass >= 0))
        iRslt = pData->iPass;
#ifdef MNG_INCLUDE_JNG
    else                                 /* for JNG we'll fake it... */
    if ((pData->eImagetype == mng_it_jng) &&
        (pData->bJPEGhasheader) && (pData->bJPEGdecostarted) &&
        (pData->bJPEGprogressive)) {
        if (pData->pJPEGdinfo->input_scan_number <= 1)
            iRslt = 0;                       /* first pass (I think...) */
        else if (jpeg_input_complete(pData->pJPEGdinfo))
            iRslt = 7;                       /* input complete; aka final pass */
        else
            iRslt = 3;                       /* anything between 0 and 7 will do */

    }
#endif
    else
        iRslt = 0;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_REFRESHPASS, MNG_LC_END)
#endif

    return iRslt;
}

/* ************************************************************************** */

mng_uint8 MNG_DECL mng_get_alphacompression(mng_handle hHandle) {
    mng_uint8 iRslt;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ALPHACOMPRESSION, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

    if (((mng_datap) hHandle)->eImagetype == mng_it_jng)
        iRslt = ((mng_datap) hHandle)->iJHDRalphacompression;
    else
        iRslt = 0;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ALPHACOMPRESSION, MNG_LC_END)
#endif

    return iRslt;
}

/* ************************************************************************** */

mng_uint8 MNG_DECL mng_get_alphafilter(mng_handle hHandle) {
    mng_uint8 iRslt;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ALPHAFILTER, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

    if (((mng_datap) hHandle)->eImagetype == mng_it_jng)
        iRslt = ((mng_datap) hHandle)->iJHDRalphafilter;
    else
        iRslt = 0;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ALPHAFILTER, MNG_LC_END)
#endif

    return iRslt;
}

/* ************************************************************************** */

mng_uint8 MNG_DECL mng_get_alphainterlace(mng_handle hHandle) {
    mng_uint8 iRslt;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ALPHAINTERLACE, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

    if (((mng_datap) hHandle)->eImagetype == mng_it_jng)
        iRslt = ((mng_datap) hHandle)->iJHDRalphainterlace;
    else
        iRslt = 0;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ALPHAINTERLACE, MNG_LC_END)
#endif

    return iRslt;
}

/* ************************************************************************** */

mng_uint8 MNG_DECL mng_get_alphadepth(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ALPHADEPTH, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ALPHADEPTH, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iAlphadepth;
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_canvasstyle(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_CANVASSTYLE, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_CANVASSTYLE, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iCanvasstyle;
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_bkgdstyle(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_BKGDSTYLE, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_BKGDSTYLE, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iBkgdstyle;
}

/* ************************************************************************** */

mng_retcode MNG_DECL mng_get_bgcolor(mng_handle hHandle,
                                     mng_uint16 *iRed,
                                     mng_uint16 *iGreen,
                                     mng_uint16 *iBlue) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_GET_BGCOLOR, MNG_LC_START)
#endif

    MNG_VALIDHANDLE (hHandle)
    *iRed = ((mng_datap) hHandle)->iBGred;
    *iGreen = ((mng_datap) hHandle)->iBGgreen;
    *iBlue = ((mng_datap) hHandle)->iBGblue;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACE (((mng_datap)hHandle), MNG_FN_GET_BGCOLOR, MNG_LC_END)
#endif

    return MNG_NOERROR;
}

/* ************************************************************************** */

mng_bool MNG_DECL mng_get_usebkgd(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_USEBKGD, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_USEBKGD, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bUseBKGD;
}

/* ************************************************************************** */

mng_bool MNG_DECL mng_get_storechunks(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_STORECHUNKS, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_STORECHUNKS, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bStorechunks;
}

/* ************************************************************************** */

mng_bool MNG_DECL mng_get_sectionbreaks(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_SECTIONBREAKS, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_SECTIONBREAKS, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bSectionbreaks;
}

/* ************************************************************************** */

mng_bool MNG_DECL mng_get_cacheplayback(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_CACHEPLAYBACK, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_CACHEPLAYBACK, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bCacheplayback;
}

/* ************************************************************************** */

mng_bool MNG_DECL mng_get_doprogressive(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_DOPROGRESSIVE, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_DOPROGRESSIVE, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bDoProgressive;
}

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_bool MNG_DECL mng_get_srgb(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_SRGB, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEB (((mng_datap)hHandle), MNG_FN_GET_SRGB, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bIssRGB;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

mng_float MNG_DECL mng_get_viewgamma(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_VIEWGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_VIEWGAMMA, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->dViewgamma;
}

/* ************************************************************************** */

mng_float MNG_DECL mng_get_displaygamma(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_DISPLAYGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_DISPLAYGAMMA, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->dDisplaygamma;
}

/* ************************************************************************** */

mng_float MNG_DECL mng_get_dfltimggamma(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_DFLTIMGGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_DFLTIMGGAMMA, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->dDfltimggamma;
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_viewgammaint(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_VIEWGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_VIEWGAMMA, MNG_LC_END)
#endif

    return (mng_uint32)(((mng_datap) hHandle)->dViewgamma * 100000);
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_displaygammaint(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_DISPLAYGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_DISPLAYGAMMA, MNG_LC_END)
#endif

    return (mng_uint32)(((mng_datap) hHandle)->dDisplaygamma * 100000);
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_dfltimggammaint(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_DFLTIMGGAMMA, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_DFLTIMGGAMMA, MNG_LC_END)
#endif

    return (mng_uint32)(((mng_datap) hHandle)->dDfltimggamma * 100000);
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_maxcanvaswidth(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_MAXCANVASWIDTH, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_MAXCANVASWIDTH, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iMaxwidth;
}

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_maxcanvasheight(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_MAXCANVASHEIGHT, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_MAXCANVASHEIGHT, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iMaxheight;
}

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_int32 MNG_DECL mng_get_zlib_level(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_LEVEL, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_LEVEL, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iZlevel;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_int32 MNG_DECL mng_get_zlib_method(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_METHOD, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_METHOD, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iZmethod;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_int32 MNG_DECL mng_get_zlib_windowbits(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_WINDOWBITS, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_WINDOWBITS, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iZwindowbits;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_int32 MNG_DECL mng_get_zlib_memlevel(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_MEMLEVEL, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_MEMLEVEL, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iZmemlevel;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_int32 MNG_DECL mng_get_zlib_strategy(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_STRATEGY, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_STRATEGY, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iZstrategy;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_ZLIB

mng_uint32 MNG_DECL mng_get_zlib_maxidat(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_MAXIDAT, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_ZLIB_MAXIDAT, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iMaxIDAT;
}

#endif /* MNG_INCLUDE_ZLIB */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mngjpeg_dctmethod MNG_DECL mng_get_jpeg_dctmethod(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_DCTMETHOD, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return JDCT_ISLOW;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_DCTMETHOD, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->eJPEGdctmethod;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mng_int32 MNG_DECL mng_get_jpeg_quality(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_QUALITY, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_QUALITY, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iJPEGquality;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mng_int32 MNG_DECL mng_get_jpeg_smoothing(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_SMOOTHING, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_SMOOTHING, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iJPEGsmoothing;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mng_bool MNG_DECL mng_get_jpeg_progressive(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_PROGRESSIVE, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_PROGRESSIVE, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bJPEGcompressprogr;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mng_bool MNG_DECL mng_get_jpeg_optimized(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_OPTIMIZED, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_OPTIMIZED, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bJPEGcompressopt;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_INCLUDE_JNG

mng_uint32 MNG_DECL mng_get_jpeg_maxjdat(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_MAXJDAT, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_JPEG_MAXJDAT, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iMaxJDAT;
}

#endif /* MNG_INCLUDE_JNG */

/* ************************************************************************** */

#ifdef MNG_SUPPORT_READ

mng_bool MNG_DECL mng_get_suspensionmode(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_SUSPENSIONMODE, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return mng_st_normal;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_SUSPENSIONMODE, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bSuspensionmode;
}

#endif /* MNG_SUPPORT_READ */

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_speedtype MNG_DECL mng_get_speed(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_SPEED, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return mng_st_normal;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_SPEED, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iSpeed;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

mng_uint32 MNG_DECL mng_get_imagelevel(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_IMAGELEVEL, MNG_LC_START)
#endif

    MNG_VALIDHANDLEX (hHandle)

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_IMAGELEVEL, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iImagelevel;
}

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_uint32 MNG_DECL mng_get_starttime(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_STARTTIME, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return mng_st_normal;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_STARTTIME, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iStarttime;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_uint32 MNG_DECL mng_get_runtime(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_RUNTIME, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return mng_st_normal;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_RUNTIME, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iRuntime;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_uint32 MNG_DECL mng_get_currentframe(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_CURRENTFRAME, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return mng_st_normal;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_CURRENTFRAME, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iFrameseq;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_uint32 MNG_DECL mng_get_currentlayer(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_CURRENTLAYER, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return mng_st_normal;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_CURRENTLAYER, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iLayerseq;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_uint32 MNG_DECL mng_get_currentplaytime(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_CURRENTPLAYTIME, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return mng_st_normal;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_GET_CURRENTPLAYTIME, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->iFrametime;
}

#endif /* MNG_SUPPORT_DISPLAY */

/* ************************************************************************** */

mng_bool MNG_DECL mng_status_error(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_ERROR, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return MNG_FALSE;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_ERROR, MNG_LC_END)
#endif

    return (mng_bool) ((mng_datap) hHandle)->iErrorcode;
}

/* ************************************************************************** */

#ifdef MNG_SUPPORT_READ

mng_bool MNG_DECL mng_status_reading(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_READING, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return MNG_FALSE;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_READING, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bReading;
}

#endif

/* ************************************************************************** */

#ifdef MNG_SUPPORT_READ

mng_bool MNG_DECL mng_status_suspendbreak(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_SUSPENDBREAK, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return MNG_FALSE;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_SUSPENDBREAK, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bSuspended;
}

#endif

/* ************************************************************************** */

#ifdef MNG_SUPPORT_WRITE
mng_bool MNG_DECL mng_status_creating (mng_handle hHandle)
{
#ifdef MNG_SUPPORT_TRACE
  MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_CREATING, MNG_LC_START)
#endif

  if ((hHandle == 0) || (((mng_datap)hHandle)->iMagic != MNG_MAGIC))
    return MNG_FALSE;

#ifdef MNG_SUPPORT_TRACE
  MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_CREATING, MNG_LC_END)
#endif

  return ((mng_datap)hHandle)->bCreating;
}
#endif

/* ************************************************************************** */

#ifdef MNG_SUPPORT_WRITE
mng_bool MNG_DECL mng_status_writing (mng_handle hHandle)
{
#ifdef MNG_SUPPORT_TRACE
  MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_WRITING, MNG_LC_START)
#endif

  if ((hHandle == 0) || (((mng_datap)hHandle)->iMagic != MNG_MAGIC))
    return MNG_FALSE;

#ifdef MNG_SUPPORT_TRACE
  MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_WRITING, MNG_LC_END)
#endif

  return ((mng_datap)hHandle)->bWriting;
}
#endif

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_bool MNG_DECL mng_status_displaying(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_DISPLAYING, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return MNG_FALSE;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_DISPLAYING, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bDisplaying;
}

#endif

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_bool MNG_DECL mng_status_running(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_RUNNING, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return MNG_FALSE;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_RUNNING, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bRunning;
}

#endif

/* ************************************************************************** */

#ifdef MNG_SUPPORT_DISPLAY

mng_bool MNG_DECL mng_status_timerbreak(mng_handle hHandle) {
#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_TIMERBREAK, MNG_LC_START)
#endif

    if ((hHandle == 0) || (((mng_datap) hHandle)->iMagic != MNG_MAGIC))
        return MNG_FALSE;

#ifdef MNG_SUPPORT_TRACE
    MNG_TRACEX (((mng_datap)hHandle), MNG_FN_STATUS_TIMERBREAK, MNG_LC_END)
#endif

    return ((mng_datap) hHandle)->bTimerset;
}

#endif

/* ************************************************************************** */
/* * end of file                                                            * */
/* ************************************************************************** */

