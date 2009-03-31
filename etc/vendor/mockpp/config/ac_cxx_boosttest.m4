
AC_DEFUN([EA_CHECK_BOOSTTEST],
[

AC_ARG_ENABLE(boostdir, [  --boost-dir             directory where the root of the Boost headers is located], [boostdir=NO])

boost_inc_dirs="/usr/include /usr/local/include /usr/include/boost-* /usr/local/include/boost-*"
if test x$boostdir != xNO; then
  boost_inc_dirs="$boost_inc_dirs $boostdir"
fi
boost_lib_dirs="/usr/lib /usr/lib64 /usr/local/lib"

if test "ac_cv_header_boost_TestSuite_h" != "yes"; then
  echo "checking for boost/any.hpp at $boost_inc_dirs"
  EA_FIND_FILE(boost/any.hpp, $boost_inc_dirs, boost_inc, dummy)
  if test x$boost_inc != xNO; then
    echo "*******************************************************"
    echo "found boost at $boost_inc"
    echo "*******************************************************"
    AC_DEFINE(HAVE_BOOSTTEST, 1, [Define if you have boost installed.])
  else
    echo "*******************************************************"
    echo "***  boost includes not found."
    echo "*******************************************************"
    AH_TEMPLATE(HAVE_BOOSTTEST, [Define if you have boost installed.])
  fi
fi

echo "checking for libboost_unit_test_framework*.so at $boost_lib_dirs"
EA_FIND_FILE(libboost_unit_test_framework*.so, $boost_lib_dirs, boost_lib, boost_name)
if test x$boost_lib != xNO; then
  echo "*******************************************************"
  echo "found boost test library as $boost_name at $boost_lib"
  echo "*******************************************************"
else
  echo "*******************************************************"
  echo "***  boost unit test framework not found."
  echo "*******************************************************"
fi

AC_ARG_ENABLE(boosttest, [  --enable-boosttest      use Boost.Test as framework, unless cxxtest selected (no) ], [enable_boosttest=yes])

if test x$boosttest_inc != xNO; then
  if test x$enable_boosttest = xyes; then
    x=${boost_name#lib}
    LIBBOOST="-l${x%.so}"
    EA_EXTRA_INC="$EA_EXTRA_INC -I$boost_inc"
    EA_EXTRA_LIB="$EA_EXTRA_LIB   -L$boost_lib"
    echo "*******************************************************"
    echo "**  using Boost.Test framework"
    echo "*******************************************************"
    AC_DEFINE(MOCKPP_USE_BOOSTTEST, 1, [Define to use Boost.Test as framework.])
  else
    echo "*******************************************************"
    echo "**  NOT using Boost.Test framework"
    echo "*******************************************************"
    AH_TEMPLATE(MOCKPP_USE_BOOSTTEST, [Define to use Boost.Test as framework.])
  fi
fi

AM_CONDITIONAL(BOOSTTEST, test x$enable_boosttest = xyes)
AC_SUBST(LIBBOOST)
])
