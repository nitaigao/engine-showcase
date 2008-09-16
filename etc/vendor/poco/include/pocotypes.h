/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC. All Rights Reserved.	    */
/*									    */
/*  This code is the property of Pocomatic Software LLC., and is	    */
/*  protected  by  copyright under the laws of the United States.	    */
/*  This  program is confidential, proprietary, and  a	trade secret,	    */
/*  not  to  be disclosed  without written authorization from Pocomatic     */
/*  Software, Inc.  Any use,  duplication,  or disclosure of this program   */
/*  by other than  Pocomatic Software Inc. and its assigned licensees	    */
/*  is strictly forbidden by law.					    */
/*									    */
/*  Author: Ke Jin <kejin@pocomatic.com>				    */
/*									    */
/****************************************************************************/
#ifndef _pocotypes_h_
# define _pocotypes_h_

//
// This header file is for export/import capsule 
// classes/functions on windows. 
//

typedef unsigned long POCO_ULong;

#if defined(WIN32)
#  if defined(BUILD_POCOCAPSULE_DLL)
#    define _POCO_CAPSULE_EXPORT __declspec(dllexport)
#  else
#    define _POCO_CAPSULE_EXPORT __declspec(dllimport)
#  endif
#else
#  define _POCO_CAPSULE_EXPORT
#endif


#endif
