dnl @synopsis AC_CXX_exception from CppUnit
dnl
dnl If the compiler supports Run-Time Type Identification (typeinfo
dnl header and typeid keyword), define HAVE_exception.
dnl
dnl @version $Id: ac_cxx_except.m4 998 2005-11-27 17:36:55Z ewald-arnold $
dnl @author Luc Maisonobe
dnl

AC_DEFUN([EA_DISABLE_EXCEPTIONS],[

AC_ARG_ENABLE(exceptions, [  --disable-exceptions    disable exception support (no)], [ disable_exceptions=yes ], [ disable_exceptions=no ])

if test "x$disable_exceptions" = xyes; then
  echo "*******************************************************"
  echo "**  EXCEPTIONS are disabled"
  echo "*******************************************************"
  AC_DEFINE(MOCKPP_NO_EXCEPTIONS, 1, [Define to disable exception support.])
  CXXFLAGS="$CXXFLAGS -fno-exceptions"
else
  echo "*******************************************************"
  echo "**  EXCEPTIONS are enabled"
  echo "*******************************************************"
  AH_TEMPLATE([MOCKPP_NO_EXCEPTIONS],
   [Define to disable exception support.])
fi
# AC_SUBST(MOCKPP_NO_EXCEPTION)
])

