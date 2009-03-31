dnl @synopsis AC_CXX_RTTI from CppUnit
dnl
dnl If the compiler supports Run-Time Type Identification (typeinfo
dnl header and typeid keyword), define HAVE_RTTI.
dnl
dnl @version $Id: ac_cxx_rtti.m4 996 2005-11-27 17:27:57Z ewald-arnold $
dnl @author Luc Maisonobe
dnl

AC_DEFUN([AC_CXX_RTTI],
[AC_CACHE_CHECK(whether the compiler supports Run-Time Type Identification,
ac_cv_cxx_rtti,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>],[
class Base { public :
             Base () {}
             virtual ~Base () {}
             virtual int f () { return 0; }
           };
class Derived : public Base { public :
                              Derived () {}
                              virtual ~Derived () {}
                              virtual int f () { return 1; }
                            };
Derived d;
Base *ptr = &d;
return typeid (*ptr) == typeid (Derived);
],
 ac_cv_cxx_rtti=yes, ac_cv_cxx_rtti=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_rtti" = yes; then
  AC_DEFINE(HAVE_RTTI,1,
            [define if the compiler supports Run-Time Type Identification])
fi
])


AC_DEFUN([EA_DISABLE_RTTI],[

AC_ARG_ENABLE(rtti, [  --disable-rtti          disable rtti support (no)], [ disable_rtti=yes ], [ disable_rtti=no ])

if test "x$disable_rtti" = xyes; then
  echo "*******************************************************"
  echo "**  RTTI is disabled"
  echo "*******************************************************"
  AC_DEFINE(MOCKPP_NO_RTTI, 1, [Define to disable rtti support.])
  CXXFLAGS="$CXXFLAGS -fno-rtti"
else
  echo "*******************************************************"
  echo "**  RTTI is enabled"
  echo "*******************************************************"
  AH_TEMPLATE([MOCKPP_NO_RTTI],
   [Define to disable rtti support.])
fi
# AC_SUBST(MOCKPP_NO_RTTI)
])

