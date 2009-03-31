/** @file
    @internal NOT INTENDED FOR PUBLIC INCLUSION, USE @p ChainableMockObject.h INSTEAD
    @brief    Keep ugly macros away from header files

  $Id: ChainableMockObject_macro.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Sep 28 2004
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 **/


#ifndef MOCKPP_CHAINABLEMOCKOBJECT_MACRO_H
#define MOCKPP_CHAINABLEMOCKOBJECT_MACRO_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <mockpp/mockpp.h>

////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name) \
  name ## CoreMock(MOCKPP_PCHAR(#name) MOCKPP_PCHAR("CoreMock"), this)

////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CHAINER_DECL_START(classname, methname, ArgCount, ReturnType, InvocationType) \
    friend class ChainerFor ## methname;  \
    public: \
      class ChainerFor ## methname : public MOCKPP_NS::ChainingMockBuilder<MOCKPP_NS::ArgumentsMatchBuilder ## ArgCount< ReturnType, InvocationType > >\
      { \
        \
        public: \
          \
          ChainerFor ## methname(classname *cls) \
            : MOCKPP_NS::ChainingMockBuilder<MOCKPP_NS::ArgumentsMatchBuilder ## ArgCount< ReturnType, InvocationType > >(&cls->methname ## CoreMock, cls, MOCKPP_PCHAR("ChainerFor") MOCKPP_PCHAR(# methname)) \
          { \
          } \
          \
          virtual ~ChainerFor ## methname() \
          { \
          }


#define MOCKPP_CHAINER_DECL_END  }


#define MOCKPP_CHAINER_DECL0(classname, methname, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_START(classname, methname, 0, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_END

#define MOCKPP_CHAINER_DECL1(classname, methname, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_START(classname, methname, 1, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_END

#define MOCKPP_CHAINER_DECL2(classname, methname, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_START(classname, methname, 2, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_END

#define MOCKPP_CHAINER_DECL3(classname, methname, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_START(classname, methname, 3, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_END

#define MOCKPP_CHAINER_DECL4(classname, methname, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_START(classname, methname, 4, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_END

#define MOCKPP_CHAINER_DECL5(classname, methname, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_START(classname, methname, 5, ReturnType, InvocationType) \
    MOCKPP_CHAINER_DECL_END

////////////////////////////////////////////////////////////////////////////

/** Implements a method with 0 parameters for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the extended method name
  * @param a_name       the actual method name
  */
#define  MOCKPP_VOID_CHAINABLE0_IMPL(classname, m_name, \
                                                a_name) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation0 inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name)); \
      m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation0 m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock<void, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL0(classname, m_name, void, m_name ## InvocationType)


/** Implements a method with 1 parameter for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  */
#define MOCKPP_VOID_CHAINABLE1_IMPL(classname, m_name, m_type1, \
                                               a_name, v_type1) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation1< v_type1 > inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name), (v_type1&)param1); \
      m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation1< v_type1 > m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock<void, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL1(classname, m_name, void, m_name ## InvocationType)


/** Implements a method with 2 parameters for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  */
#define MOCKPP_VOID_CHAINABLE2_IMPL(classname, m_name, m_type1, m_type2, \
                                               a_name, v_type1, v_type2) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation2< v_type1, v_type2 > inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name), (v_type1&)param1, (v_type2&)param2); \
      m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation2< v_type1, v_type2 > m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock<void, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL2(classname, m_name, void, m_name ## InvocationType)


/** Implements a method with 3 parameters for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  */
#define MOCKPP_VOID_CHAINABLE3_IMPL(classname, m_name, m_type1, m_type2, m_type3, \
                                               a_name, v_type1, v_type2, v_type3) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation3< v_type1, v_type2, v_type3 > inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name), (v_type1&)param1, (v_type2&)param2, (v_type3&)param3); \
      m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation3< v_type1, v_type2, v_type3 > m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock<void, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL3(classname, m_name, void, m_name ## InvocationType)


/** Implements a method with 4 parameters for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  */
#define MOCKPP_VOID_CHAINABLE4_IMPL(classname, m_name, m_type1, m_type2, m_type3, m_type4, \
                                               a_name, v_type1, v_type2, v_type3, v_type4) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation4< v_type1, v_type2, v_type3, v_type4 > inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name), (v_type1&)param1, (v_type2&)param2, (v_type3&)param3, (v_type4&)param4); \
      m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation4< v_type1, v_type2, v_type3, v_type4 > m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock<void, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL4(classname, m_name, void, m_name ## InvocationType)


/** Implements a method with 5 parameters for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param m_type5      the data type of the 5. method parameter
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  * @param v_type5      the data type of the internal variable of 5. parameter
  */
#define MOCKPP_VOID_CHAINABLE5_IMPL(classname, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                               a_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation5< v_type1, v_type2, v_type3, v_type4, v_type5 > inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name), (v_type1&)param1, (v_type2&)param2, (v_type3&)param3, (v_type4&)param4, (v_type5&)param5); \
      m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation5< v_type1, v_type2, v_type3, v_type4, v_type5 > m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock<void, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL5(classname, m_name, void, m_name ## InvocationType)

////////////////////////////////////////////////////////////////////////////

/** Implements a method with 0 parameters for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the extended method name
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param a_name       the actual method name
  */
#define  MOCKPP_CHAINABLE0_IMPL(classname, m_ret_type, m_name, \
                                           v_ret_type, a_name) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation0 inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name)); \
      return m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation0 m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock< m_ret_type, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL0(classname, m_name, m_ret_type, m_name ## InvocationType)


/** Implements a method with 1 parameter for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  */
#define MOCKPP_CHAINABLE1_IMPL(classname, m_ret_type, m_name, m_type1, \
                                          v_ret_type, a_name, v_type1) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation1< v_type1 > inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name), (v_type1&)param1); \
      return m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation1< v_type1 > m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock< m_ret_type, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL1(classname, m_name, m_ret_type, m_name ## InvocationType)


/** Implements a method with 2 parameters for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  */
#define MOCKPP_CHAINABLE2_IMPL(classname, m_ret_type, m_name, m_type1, m_type2, \
                                          v_ret_type, a_name, v_type1, v_type2) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation2< v_type1, v_type2 > inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name), (v_type1&)param1, (v_type2&)param2); \
      return m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation2< v_type1, v_type2 > m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock< m_ret_type, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL2(classname, m_name, m_ret_type, m_name ## InvocationType)


/** Implements a method with 3 parameters for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  */
#define MOCKPP_CHAINABLE3_IMPL(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, \
                                          v_ret_type, a_name, v_type1, v_type2, v_type3) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation3< v_type1, v_type2, v_type3 > inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name), (v_type1&)param1, (v_type2&)param2, (v_type3&)param3); \
      return m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation3< v_type1, v_type2, v_type3 > m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock< m_ret_type, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL3(classname, m_name, m_ret_type, m_name ## InvocationType)


/** Implements a method with 4 parameters for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  */
#define MOCKPP_CHAINABLE4_IMPL(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, \
                                          v_ret_type, a_name, v_type1, v_type2, v_type3, v_type4) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation4< v_type1, v_type2, v_type3, v_type4 > inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name), (v_type1&)param1, (v_type2&)param2, (v_type3&)param3, (v_type4&)param4); \
      return m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation4< v_type1, v_type2, v_type3, v_type4 > m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock< m_ret_type, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL4(classname, m_name, m_ret_type, m_name ## InvocationType)


/** Implements a method with 5 parameters for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param m_type5      the data type of the 5. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  * @param v_type5      the data type of the internal variable of 5. parameter
  */
#define MOCKPP_CHAINABLE5_IMPL(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                          v_ret_type, a_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      MOCKPP_NS::Invocation5< v_type1, v_type2, v_type3, v_type4, v_type5 > inv(getVerifiableName() + MOCKPP_PCHAR(".") MOCKPP_PCHAR(#m_name), (v_type1&)param1, (v_type2&)param2, (v_type3&)param3, (v_type4&)param4, (v_type5&)param5); \
      return m_name ## CoreMock.invoke(inv); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    typedef MOCKPP_NS::Invocation5< v_type1, v_type2, v_type3, v_type4, v_type5 > m_name ## InvocationType; \
    mutable MOCKPP_NS::CoreMock< m_ret_type, m_name ## InvocationType > m_name ## CoreMock; \
    MOCKPP_CHAINER_DECL5(classname, m_name, m_ret_type, m_name ## InvocationType)


#endif // DOXYGEN_SHOULD_SKIP_THIS


#endif // MOCKPP_CHAINABLEMOCKOBJECT_MACRO_H
