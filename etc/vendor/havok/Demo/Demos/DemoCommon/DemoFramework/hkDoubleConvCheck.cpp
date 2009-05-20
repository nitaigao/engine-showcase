/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Demos/DemoCommon/DemoFramework/hkDoubleConvCheck.h>

static hkBool g_checkDoubles = false;
static hkBool g_checkDoubleStack[255];
static int  g_checkDoubleStackIndex = 0;

void HK_CALL pushDoubleConversionCheck(hkBool enable)
{
	g_checkDoubleStack[g_checkDoubleStackIndex] = g_checkDoubles;
	g_checkDoubleStackIndex++;
	HK_ASSERT2(0x6c9871d4, g_checkDoubleStackIndex < 255, "Double check overflow");
	
	g_checkDoubles = enable;
}
	
void HK_CALL popDoubleConversionCheck()
{
	g_checkDoubleStackIndex--;
	HK_ASSERT2(0x7511c51f, g_checkDoubleStackIndex >= 0, "Double check underflow");
	
	g_checkDoubles = g_checkDoubleStack[g_checkDoubleStackIndex];
}

	
#if defined(HK_COMPILER_MWERKS) && defined(HK_ARCH_PS2) && defined(HK_DEBUG)

/*
** libgcc support for software floating point.
** Copyright (C) 1991 by Pipeline Associates, Inc.  All rights reserved.
** Permission is granted to do *anything* you want with this file,
** commercial or otherwise, provided this message remains intact.  So there!
** I would appreciate receiving any updates/patches/changes that anyone
** makes, and am willing to be the repository for said changes (am I
** making a big mistake?).
*/

/* the following deal with IEEE single-precision numbers */
#define EXCESS		126
#define SIGNBIT		0x80000000
#define HIDDEN		(1 << 23)
#define SIGN(fp)	((fp) & SIGNBIT)
#define EXP(fp)		(((fp) >> 23) & 0xFF)
#define MANT(fp)	(((fp) & 0x7FFFFF) | HIDDEN)
#define PACK(s,e,m)	((s) | ((e) << 23) | (m))

/* the following deal with IEEE double-precision numbers */
#define EXCESSD		1022
#define HIDDEND		(1 << 20)
#define EXPD(fp)	(((fp.l.upper) >> 20) & 0x7FF)
#define SIGND(fp)	((fp.l.upper) & SIGNBIT)
#define MANTD(fp)	(((((fp.l.upper) & 0xFFFFF) | HIDDEND) << 10) | \
				(fp.l.lower >> 22))
#define HIDDEND_LL	((long long)1 << 52)
#define MANTD_LL(fp)	((fp.ll & (HIDDEND_LL-1)) | HIDDEND_LL)
#define PACKD_LL(s,e,m)	(((long long)((s)+((e)<<20))<<32)|(m))

union double_long {
    double d;
    struct {
      unsigned int lower;
      unsigned int upper;
    } l;
    long ll;
};

union float_long
  {
    float f;
    unsigned int l;
  };


/* convert float to double */
static double gccextendsfdf2 (float a1)
{
  register union float_long fl1;
  register union double_long dl;
  register int exp;

  fl1.f = a1;

  if (!fl1.l)
    {
      dl.l.upper = 0; dl.l.lower = 0;
      return (dl.d);
    }

  dl.l.upper = SIGN (fl1.l);
  exp = (int)EXP (fl1.l) - EXCESS + EXCESSD;
  dl.l.upper |= (unsigned)(exp << 20);
  dl.l.upper |= (MANT (fl1.l) & ~HIDDEN) >> 3;
  dl.l.lower = MANT (fl1.l) << 29;

  return (dl.d);
}

/* convert double to float */
static float gcctruncdfsf2 (double a1)
{
  register int exp;
  register long mant;
  register union float_long fl;
  register union double_long dl1;

  dl1.d = a1;

  if (!dl1.l.upper && !dl1.l.lower)
    return (float)(0);

  exp = (int)EXPD (dl1);
  exp -= EXCESSD;
  exp += EXCESS;

  /* shift double mantissa 6 bits so we can round */
  mant = MANTD (dl1) >> 6;

  /* now round and shift down */
  mant += 1;
  mant >>= 1;

  /* did the round overflow? */
  if (mant & 0xFE000000)
    {
      mant >>= 1;
      exp++;
    }

  mant &= ~HIDDEN;

  /* pack up and go home */
  fl.l = PACK (SIGND (dl1), exp, (unsigned int)mant);
  return (fl.f);
}


//////////////////////////////////////////////////////////////////
//
// Our implementations of fptodp and dptofp
//
//////////////////////////////////////////////////////////////////

extern "C"
{	

	double fptodp(float x) 
	{
		if (g_checkDoubles)
		{ 
			HK_ASSERT2(0x37ffba58, false, "fptodp called\n"); 
		}
		return gccextendsfdf2(x);
	}
	
	float dptofp(double x) 
	{ 
		if (g_checkDoubles)
		{ 
			HK_ASSERT2(0x4f0c55c5, false, "dptofp called\n"); 
		}
		return gcctruncdfsf2(x);
	}
}

#endif

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
