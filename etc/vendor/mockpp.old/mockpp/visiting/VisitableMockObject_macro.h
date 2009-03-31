/** @file
    @internal NOT INTENDED FOR PUBLIC INCLUSION, USE @p VisitableMockObject.h INSTEAD
    @brief    Keep ugly macros away from header files

  $Id: VisitableMockObject_macro.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Fri Feb 07 2003
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


#ifndef MOCKPP_VISITABLEMOCKOBJECT_MACRO_H
#define MOCKPP_VISITABLEMOCKOBJECT_MACRO_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <mockpp/mockpp.h>

#include <memory>

////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_THROWABLE_INLINE true

//##########################################################################
//
//    IMPORTANT: update the test files if you add members for reset
//
//##########################################################################

#define MOCKPP_IMPL_RESET_VOID_BASE(methname) \
      objptr->methname ## Throwables.reset(); \
      objptr->methname ## DefaultThrowable.reset(); \
      objptr->methname ## ThrowablesInline = MOCKPP_THROWABLE_INLINE; \
      objptr->methname ## ThrowableInsteadReturn.clear()

#define MOCKPP_IMPL_RESET_BASE(methname) \
      MOCKPP_IMPL_RESET_VOID_BASE(methname); \
      objptr->methname ## ReturnValues.reset();    \
      objptr->methname ## haveDefaultReturnValue = false;  \
      objptr->methname ## DefaultReturnValueUsed = false

////////////////////////////////////////////////////////////////////////////

#define MOCKPP_IMPL_RESET_P1(methname) \
      objptr->methname ## ResponseValues.reset(); \
      objptr->methname ## Parameter1.reset();

#define MOCKPP_IMPL_RESET_P2(methname) \
      MOCKPP_IMPL_RESET_P1(methname); \
      objptr->methname ## Parameter2.reset()

#define MOCKPP_IMPL_RESET_P3(methname) \
      MOCKPP_IMPL_RESET_P2(methname); \
      objptr->methname ## Parameter3.reset()

#define MOCKPP_IMPL_RESET_P4(methname) \
      MOCKPP_IMPL_RESET_P3(methname); \
      objptr->methname ## Parameter4.reset()

#define MOCKPP_IMPL_RESET_P5(methname) \
      MOCKPP_IMPL_RESET_P4(methname); \
      objptr->methname ## Parameter5.reset()

////////////////////////////////////////////////////////////////////////////

#define MOCKPP_CONTROLLER_IMPL_RESET_0(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_BASE(methname); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESET_1(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_BASE(methname); \
      MOCKPP_IMPL_RESET_P1(methname); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESET_2(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_BASE(methname); \
      MOCKPP_IMPL_RESET_P2(methname); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESET_3(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_BASE(methname); \
      MOCKPP_IMPL_RESET_P3(methname); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESET_4(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_BASE(methname); \
      MOCKPP_IMPL_RESET_P4(methname); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESET_5(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_BASE(methname); \
      MOCKPP_IMPL_RESET_P5(methname); \
    }

////////////////////////////////////////////////////////////////////////////

#define MOCKPP_CONTROLLER_IMPL_VOID_RESET_0(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_VOID_BASE(methname); \
    }

#define MOCKPP_CONTROLLER_IMPL_VOID_RESET_1(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_VOID_BASE(methname); \
      MOCKPP_IMPL_RESET_P1(methname); \
    }

#define MOCKPP_CONTROLLER_IMPL_VOID_RESET_2(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_VOID_BASE(methname); \
      MOCKPP_IMPL_RESET_P2(methname); \
    }

#define MOCKPP_CONTROLLER_IMPL_VOID_RESET_3(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_VOID_BASE(methname); \
      MOCKPP_IMPL_RESET_P3(methname); \
    }

#define MOCKPP_CONTROLLER_IMPL_VOID_RESET_4(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_VOID_BASE(methname); \
      MOCKPP_IMPL_RESET_P4(methname); \
    }

#define MOCKPP_CONTROLLER_IMPL_VOID_RESET_5(methname) \
    void reset() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      MOCKPP_IMPL_RESET_VOID_BASE(methname); \
      MOCKPP_IMPL_RESET_P5(methname); \
    }


////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    virtual void unsetThrowablesInline() \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ThrowablesInline = false; \
    }

#define MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    void addThrowable(MOCKPP_NS::Throwable *t_) \
    { \
      MOCKPP_STD_NS::auto_ptr<MOCKPP_NS::Throwable> at (t_); \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## Throwables.push_back(at.release()); \
      objptr->methname ## ThrowableInsteadReturn.push_back(true); \
    } \
    \
    template <class T> \
    void addThrowable(const T &w, unsigned count = 1) \
    { \
      for ( ; count > 0; --count) \
        addThrowable(MOCKPP_NS::make_throwable(w)); \
    } \
    \
    void setDefaultThrowable(MOCKPP_NS::Throwable *t_) \
    { \
      MOCKPP_STD_NS::auto_ptr<MOCKPP_NS::Throwable> at (t_); \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## DefaultThrowable.take(at.release()); \
    }

////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CONTROLLER_IMPL_VERIFY(methname) \
    virtual void verify() \
    { \
      if (objptr->methname ## haveDefaultReturnValue) \
      { \
        MOCKPP_NS::String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 is unused.")); \
        fmt << MOCKPP_PCHAR(#methname) MOCKPP_PCHAR("DefaultReturnValueUsed"); \
        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, objptr->methname ## DefaultReturnValueUsed); \
      } \
      \
      MOCKPP_NS::Throwable *dt = objptr->methname ## DefaultThrowable.get(); \
      if (dt != 0) \
      { \
        MOCKPP_NS::String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 is unused.")); \
        fmt << MOCKPP_PCHAR(#methname) MOCKPP_PCHAR("DefaultThrowable"); \
        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, dt->hasThrown()); \
      }  \
    }


////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CONTROLLER_IMPL_VOID_VERIFY(methname) \
    virtual void verify() \
    { \
      MOCKPP_NS::Throwable *dt = objptr->methname ## DefaultThrowable.get(); \
      if (dt != 0) \
      { \
        MOCKPP_NS::String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 is unused.")); \
        fmt << MOCKPP_PCHAR(#methname) MOCKPP_PCHAR("DefaultThrowable"); \
        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, dt->hasThrown()); \
      }  \
    }


////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CONTROLLER_IMPL_RETURN(ret_type, methname) \
    void addReturnValue(const ret_type &rv, unsigned count = 1) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      for ( ; count > 0; --count) \
      { \
        objptr->methname ## ReturnValues.addObjectToReturn(rv); \
        objptr->methname ## ThrowableInsteadReturn.push_back(false); \
      } \
    } \
    \
    void setDefaultReturnValue(const ret_type &rv) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## haveDefaultReturnValue = true;  \
      objptr->methname ## DefaultReturnValue = rv; \
    }


////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CONTROLLER_IMPL_RESP_VAL1(ret_type, methname, type1) \
    void addResponseValue(const ret_type &rv, const type1 &p1, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(rv, (type1&)p1, count); \
    } \
    \
    void addResponseValue(const ret_type &rv, const MOCKPP_NS::ConstraintHolder< type1 > &p1, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(rv, p1, count); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESP_VAL2(ret_type, methname, type1, type2) \
    void addResponseValue(const ret_type &rv, const type1 &p1, const type2 &p2, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(rv, (type1&)p1, (type2&)p2, count); \
    } \
    \
    void addResponseValue(const ret_type &rv, const MOCKPP_NS::ConstraintHolder< type1 > &p1, \
                                              const MOCKPP_NS::ConstraintHolder< type2 > &p2, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(rv, p1, p2, count); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESP_VAL3(ret_type, methname, type1, type2, type3) \
    void addResponseValue(const ret_type &rv, const type1 &p1, const type2 &p2, const type3 &p3, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(rv, (type1&)p1, (type2&)p2, (type3&)p3, count); \
    } \
    \
    void addResponseValue(const ret_type &rv, const MOCKPP_NS::ConstraintHolder< type1 > &p1, \
                                              const MOCKPP_NS::ConstraintHolder< type2 > &p2, \
                                              const MOCKPP_NS::ConstraintHolder< type3 > &p3, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(rv, p1, p2, p3, count); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESP_VAL4(ret_type, methname, type1, type2, type3, type4) \
    void addResponseValue(const ret_type &rv, const type1 &p1, const type2 &p2, const type3 &p3, const type4 &p4, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(rv, (type1 &)p1, (type2 &)p2, (type3 &)p3, (type4 &)p4, count); \
    } \
    \
    void addResponseValue(const ret_type &rv, const MOCKPP_NS::ConstraintHolder< type1 > &p1, \
                                              const MOCKPP_NS::ConstraintHolder< type2 > &p2, \
                                              const MOCKPP_NS::ConstraintHolder< type3 > &p3, \
                                              const MOCKPP_NS::ConstraintHolder< type4 > &p4, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(rv, p1, p2, p3, p4, count); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESP_VAL5(ret_type, methname, type1, type2, type3, type4, type5) \
    void addResponseValue(const ret_type &rv, const type1 &p1, const type2 &p2, const type3 &p3, const type4 &p4, const type5 &p5, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(rv, (type1)p1, (type2)p2, (type3)p3, (type4)p4, (type5)p5, count); \
    } \
    \
    void addResponseValue(const ret_type &rv, const MOCKPP_NS::ConstraintHolder< type1 > &p1, \
                                              const MOCKPP_NS::ConstraintHolder< type2 > &p2, \
                                              const MOCKPP_NS::ConstraintHolder< type3 > &p3, \
                                              const MOCKPP_NS::ConstraintHolder< type4 > &p4, \
                                              const MOCKPP_NS::ConstraintHolder< type5 > &p5, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(rv, p1, p2, p3, p4, p5, count); \
    }


////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CONTROLLER_IMPL_RESP_THROW1(methname, type1) \
    void addResponseThrowable(MOCKPP_NS::Throwable *t, const type1 &p1, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(t, (type1&)p1, count); \
    } \
    \
    void addResponseThrowable(MOCKPP_NS::Throwable *t, const MOCKPP_NS::ConstraintHolder< type1 > &p1, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(t, p1, count); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESP_THROW2(methname, type1, type2) \
    void addResponseThrowable(MOCKPP_NS::Throwable *t, const type1 &p1, const type2 &p2, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(t, (type1&)p1, (type2&)p2, count); \
    } \
    \
    void addResponseThrowable(MOCKPP_NS::Throwable *t, const MOCKPP_NS::ConstraintHolder< type1 > &p1, \
                                                    const MOCKPP_NS::ConstraintHolder< type2 > &p2, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(t, p1, p2, count); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESP_THROW3(methname, type1, type2, type3) \
    void addResponseThrowable(MOCKPP_NS::Throwable *t, const type1 &p1, const type2 &p2, const type3 &p3, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(t, (type1 &)p1, (type2&)p2, (type3&)p3, count); \
    } \
    \
    void addResponseThrowable(MOCKPP_NS::Throwable *t, const MOCKPP_NS::ConstraintHolder< type1 > &p1, \
                                                    const MOCKPP_NS::ConstraintHolder< type2 > &p2, \
                                                    const MOCKPP_NS::ConstraintHolder< type3 > &p3, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(t, p1, p2, p3, count); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESP_THROW4(methname, type1, type2, type3, type4) \
    void addResponseThrowable(MOCKPP_NS::Throwable *t, const type1 &p1, const type2 &p2, const type3 &p3, const type4 &p4, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(t, (type1 &)p1, (type2 &)p2, (type3 &)p3, (type4 &)p4, count); \
    } \
    \
    void addResponseThrowable(MOCKPP_NS::Throwable *t, const MOCKPP_NS::ConstraintHolder< type1 > &p1, \
                                                    const MOCKPP_NS::ConstraintHolder< type2 > &p2, \
                                                    const MOCKPP_NS::ConstraintHolder< type3 > &p3, \
                                                    const MOCKPP_NS::ConstraintHolder< type4 > &p4, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(t, p1, p2, p3, p4, count); \
    }

#define MOCKPP_CONTROLLER_IMPL_RESP_THROW5(methname, type1, type2, type3, type4, type5) \
    void addResponseThrowable(MOCKPP_NS::Throwable *t, const type1 &p1, const type2 &p2, const type3 &p3, const type4 &p4, const type5 &p5, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(t, (type1)p1, (type2)p2, (type3)p3, (type4)p4, (type5)p5, count); \
    } \
    \
    void addResponseThrowable(MOCKPP_NS::Throwable *t, const MOCKPP_NS::ConstraintHolder< type1 > &p1, \
                                                    const MOCKPP_NS::ConstraintHolder< type2 > &p2, \
                                                    const MOCKPP_NS::ConstraintHolder< type3 > &p3, \
                                                    const MOCKPP_NS::ConstraintHolder< type4 > &p4, \
                                                    const MOCKPP_NS::ConstraintHolder< type5 > &p5, unsigned count = MOCKPP_UNLIMITED) \
    { \
      MOCKPP_ASSERT_FALSE(objptr->isActivated()); \
      objptr->methname ## ResponseValues.add(t, p1, p2, p3, p4, p5, count); \
    }


////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    friend class ControllerFor ## methname;  \
    public: \
      class ControllerFor ## methname : public MOCKPP_NS::VisitableMockObject::Controller \
      { \
          classname *objptr; \
        public: \
          ControllerFor ## methname(classname *cls) \
            : objptr(cls) \
        { \
          objptr->addController(this); \
        } \
        \
        virtual ~ControllerFor ## methname() \
        { \
          objptr->removeController(this); \
        }


#define MOCKPP_CONTROLLER_DECL_END  }


#define MOCKPP_VOID_CONTROLLER_DECL0(classname, methname) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_VOID_RESET_0(methname) \
    MOCKPP_CONTROLLER_IMPL_VOID_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END

#define MOCKPP_VOID_CONTROLLER_DECL1(classname, methname, type1) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_VOID_RESET_1(methname) \
    MOCKPP_CONTROLLER_IMPL_RESP_THROW1(methname, type1) \
    MOCKPP_CONTROLLER_IMPL_VOID_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END

#define MOCKPP_VOID_CONTROLLER_DECL2(classname, methname, type1, type2) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_VOID_RESET_2(methname) \
    MOCKPP_CONTROLLER_IMPL_RESP_THROW2(methname, type1, type2) \
    MOCKPP_CONTROLLER_IMPL_VOID_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END

#define MOCKPP_VOID_CONTROLLER_DECL3(classname, methname, type1, type2, type3) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_VOID_RESET_3(methname) \
    MOCKPP_CONTROLLER_IMPL_RESP_THROW3(methname, type1, type2, type3) \
    MOCKPP_CONTROLLER_IMPL_VOID_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END

#define MOCKPP_VOID_CONTROLLER_DECL4(classname, methname, type1, type2, type3, type4) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_VOID_RESET_4(methname) \
    MOCKPP_CONTROLLER_IMPL_RESP_THROW4(methname, type1, type2, type3, type4) \
    MOCKPP_CONTROLLER_IMPL_VOID_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END

#define MOCKPP_VOID_CONTROLLER_DECL5(classname, methname, type1, type2, type3, type4, type5) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_VOID_RESET_5(methname) \
    MOCKPP_CONTROLLER_IMPL_RESP_THROW5(methname, type1, type2, type3, type4, type5) \
    MOCKPP_CONTROLLER_IMPL_VOID_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END


////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CONTROLLER_DECL0(classname, ret_type, methname) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_RESET_0(methname) \
    MOCKPP_CONTROLLER_IMPL_RETURN(ret_type, methname) \
    MOCKPP_CONTROLLER_IMPL_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END

#define MOCKPP_CONTROLLER_DECL1(classname, ret_type, methname, type1) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_RETURN(ret_type, methname) \
    MOCKPP_CONTROLLER_IMPL_RESET_1(methname) \
    MOCKPP_CONTROLLER_IMPL_RESP_VAL1(ret_type, methname, type1) \
    MOCKPP_CONTROLLER_IMPL_RESP_THROW1(methname, type1) \
    MOCKPP_CONTROLLER_IMPL_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END

#define MOCKPP_CONTROLLER_DECL2(classname, ret_type, methname, type1, type2) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_RETURN(ret_type, methname) \
    MOCKPP_CONTROLLER_IMPL_RESET_2(methname) \
    MOCKPP_CONTROLLER_IMPL_RESP_VAL2(ret_type, methname, type1, type2) \
    MOCKPP_CONTROLLER_IMPL_RESP_THROW2(methname, type1, type2) \
    MOCKPP_CONTROLLER_IMPL_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END

#define MOCKPP_CONTROLLER_DECL3(classname, ret_type, methname, type1, type2, type3) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_RETURN(ret_type, methname) \
    MOCKPP_CONTROLLER_IMPL_RESET_3(methname) \
    MOCKPP_CONTROLLER_IMPL_RESP_VAL3(ret_type, methname, type1, type2, type3) \
    MOCKPP_CONTROLLER_IMPL_RESP_THROW3(methname, type1, type2, type3) \
    MOCKPP_CONTROLLER_IMPL_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END

#define MOCKPP_CONTROLLER_DECL4(classname, ret_type, methname, type1, type2, type3, type4) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_RETURN(ret_type, methname) \
    MOCKPP_CONTROLLER_IMPL_RESET_4(methname) \
    MOCKPP_CONTROLLER_IMPL_RESP_VAL4(ret_type, methname, type1, type2, type3, type4) \
    MOCKPP_CONTROLLER_IMPL_RESP_THROW4(methname, type1, type2, type3, type4) \
    MOCKPP_CONTROLLER_IMPL_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END

#define MOCKPP_CONTROLLER_DECL5(classname, ret_type, methname, type1, type2, type3, type4, type5) \
    MOCKPP_CONTROLLER_DECL_START(classname, methname) \
    MOCKPP_CONTROLLER_IMPL_HELPER(methname) \
    MOCKPP_CONTROLLER_IMPL_THROW(methname) \
    MOCKPP_CONTROLLER_IMPL_RETURN(ret_type, methname) \
    MOCKPP_CONTROLLER_IMPL_RESET_5(methname) \
    MOCKPP_CONTROLLER_IMPL_RESP_VAL5(ret_type, methname, type1, type2, type3, type4, type5) \
    MOCKPP_CONTROLLER_IMPL_RESP_THROW5(methname, type1, type2, type3, type4, type5) \
    MOCKPP_CONTROLLER_IMPL_VERIFY(methname) \
    MOCKPP_CONTROLLER_DECL_END


////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_DECLARE_DEFAULT_VARS(name) \
    MOCKPP_NS::ThrowableItem              name ## DefaultThrowable; \
    mutable MOCKPP_NS::ThrowableList      name ## Throwables; \
    bool                               name ## ThrowablesInline; \
    mutable MOCKPP_STL::vector<bool>          name ## ThrowableInsteadReturn

#define MOCKPP_DECLARE_RETURN_VARS(ret_type, name) \
    mutable MOCKPP_NS::ReturnObjectList< ret_type > name ## ReturnValues; \
    bool                                         name ## haveDefaultReturnValue; \
    mutable bool                                 name ## DefaultReturnValueUsed; \
    ret_type                                     name ## DefaultReturnValue


////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_DECLARE_PARAMETER_VARS1(name, type1) \
    mutable MOCKPP_NS::ConstraintList< type1 > name ## Parameter1

#define MOCKPP_DECLARE_PARAMETER_VARS2(name, type1, type2) \
    MOCKPP_DECLARE_PARAMETER_VARS1(name, type1); \
    mutable MOCKPP_NS::ConstraintList< type2 > name ## Parameter2

#define MOCKPP_DECLARE_PARAMETER_VARS3(name, type1, type2, type3) \
    MOCKPP_DECLARE_PARAMETER_VARS2(name, type1, type2); \
    mutable MOCKPP_NS::ConstraintList< type3 > name ## Parameter3

#define MOCKPP_DECLARE_PARAMETER_VARS4(name, type1, type2, type3, type4) \
    MOCKPP_DECLARE_PARAMETER_VARS3(name, type1, type2, type3); \
    mutable MOCKPP_NS::ConstraintList< type4 > name ## Parameter4

#define MOCKPP_DECLARE_PARAMETER_VARS5(name, type1, type2, type3, type4, type5) \
    MOCKPP_DECLARE_PARAMETER_VARS4(name, type1, type2, type3, type4); \
    mutable MOCKPP_NS::ConstraintList< type5 > name ## Parameter5

////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CONSTRUCT_BASE_VISITABLE_MEMBERS(name) \
    name ## Throwables(MOCKPP_PCHAR(#name) MOCKPP_PCHAR("Throwables"), this), \
    name ## ThrowablesInline(MOCKPP_THROWABLE_INLINE)

#define MOCKPP_CONSTRUCT_RETURN_VISITABLE_MEMBERS(name) \
    name ## ReturnValues(MOCKPP_PCHAR(#name) MOCKPP_PCHAR("/ReturnValues"), this), \
    name ## haveDefaultReturnValue(false), \
    name ## DefaultReturnValueUsed(false)


////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_CONSTRUCT_PARAMETER1_VISITABLE_MEMBERS(name) \
    name ## ResponseValues(MOCKPP_PCHAR(#name) MOCKPP_PCHAR("ResponseValues"), this), \
    name ## Parameter1(MOCKPP_PCHAR(#name) MOCKPP_PCHAR("Parameter1"), this)

#define MOCKPP_CONSTRUCT_PARAMETER2_VISITABLE_MEMBERS(name) \
    MOCKPP_CONSTRUCT_PARAMETER1_VISITABLE_MEMBERS(name),    \
    name ## Parameter2(MOCKPP_PCHAR(#name) MOCKPP_PCHAR("Parameter2"), this)

#define MOCKPP_CONSTRUCT_PARAMETER3_VISITABLE_MEMBERS(name) \
    MOCKPP_CONSTRUCT_PARAMETER2_VISITABLE_MEMBERS(name),    \
    name ## Parameter3(MOCKPP_PCHAR(#name) MOCKPP_PCHAR("Parameter3"), this)

#define MOCKPP_CONSTRUCT_PARAMETER4_VISITABLE_MEMBERS(name) \
    MOCKPP_CONSTRUCT_PARAMETER3_VISITABLE_MEMBERS(name),    \
    name ## Parameter4(MOCKPP_PCHAR(#name) MOCKPP_PCHAR("Parameter4"), this)

#define MOCKPP_CONSTRUCT_PARAMETER5_VISITABLE_MEMBERS(name) \
    MOCKPP_CONSTRUCT_PARAMETER4_VISITABLE_MEMBERS(name),    \
    name ## Parameter5(MOCKPP_PCHAR(#name) MOCKPP_PCHAR("Parameter5"), this)


////////////////////////////////////////////////////////////////////////////
//

#define MOCKPP_THROWER_IMPL(name) \
   bool do_throw = true; \
   if (name ## ThrowablesInline) \
   { \
     if (name ## ThrowableInsteadReturn.size() != 0) \
     { \
       do_throw = name ## ThrowableInsteadReturn[0]; \
       name ## ThrowableInsteadReturn.erase(name ## ThrowableInsteadReturn.begin()); \
     } \
   } \
   if (do_throw && name ## Throwables.hasMoreObjects() != 0) \
   { \
    MOCKPP_NS::Throwable *thr = name ## Throwables.nextThrowableObject(); \
    if (thr != 0) \
      thr->throw_me(); \
   } \
   \
   if (name ## DefaultThrowable.get() != 0) \
     name ## DefaultThrowable.get()->throw_me()

////////////////////////////////////////////////////////////////////////////
//

/** Implements the internal of a method with 0 parameters for a mock object.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  */
#define MOCKPP_VOID_VISITABLE0_IMPL(classname, m_name) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         return;\
      } \
      else \
      { \
         addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         MOCKPP_THROWER_IMPL(m_name ); \
         return;\
      } \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name ); \
    MOCKPP_VOID_CONTROLLER_DECL0(classname, m_name)


/** Implements a method with 1 parameter for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  */
#define MOCKPP_VOID_VISITABLE1_IMPL(classname, m_name, m_type1,  \
                                               a_name, v_type1) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         m_name ## Parameter1.addExpected((v_type1)param1); \
         return; \
      }    \
      else \
      {    \
         MOCKPP_TRY { \
           addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name);           \
           MOCKPP_NS::Throwable *t; \
           if (m_name ## ResponseValues.find(t, (v_type1)param1)) \
              t->throw_me(); \
           MOCKPP_THROWER_IMPL(m_name); \
         } MOCKPP_CATCH_ALL { \
           m_name ## Parameter1.balanceActual(); \
           MOCKPP_RETHROW; \
         } \
         m_name ## Parameter1.addActual((v_type1&)param1); \
         return; \
      }  \
    } \
    \
    void a_name (const MOCKPP_NS::ConstraintHolder< v_type1 >  &param1) /* @todo const? */ \
    { \
      addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + #m_name);  \
      m_name ## Parameter1.addExpected(param1); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name); \
    mutable MOCKPP_NS::ResponseThrowableVector1< v_type1 > m_name ## ResponseValues; \
    MOCKPP_DECLARE_PARAMETER_VARS1(m_name, v_type1); \
    MOCKPP_VOID_CONTROLLER_DECL1(classname, m_name, v_type1)


/** Implements a method with 2 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  */
#define MOCKPP_VOID_VISITABLE2_IMPL(classname, m_name, m_type1, m_type2,  \
                                               a_name, v_type1, v_type2) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         m_name ## Parameter1.addExpected((v_type1)param1); \
         m_name ## Parameter2.addExpected((v_type2)param2); \
         return; \
      }    \
      else \
      {    \
         MOCKPP_TRY { \
           addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name);           \
           MOCKPP_NS::Throwable *t; \
           if (m_name ## ResponseValues.find(t, (v_type1)param1, (v_type2)param2)) \
              t->throw_me(); \
           MOCKPP_THROWER_IMPL(m_name); \
         } MOCKPP_CATCH_ALL { \
           m_name ## Parameter1.balanceActual(); \
           m_name ## Parameter2.balanceActual(); \
           MOCKPP_RETHROW; \
         } \
         m_name ## Parameter1.addActual((v_type1&)param1); \
         m_name ## Parameter2.addActual((v_type2&)param2); \
         return; \
      }  \
    }     \
    \
    void a_name(const MOCKPP_NS::ConstraintHolder< v_type1 >  &param1, \
                const MOCKPP_NS::ConstraintHolder< v_type2 >  &param2) /* @todo const? */ \
    { \
      addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + #m_name);  \
      m_name ## Parameter1.addExpected(param1); \
      m_name ## Parameter2.addExpected(param2); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name); \
    mutable MOCKPP_NS::ResponseThrowableVector2< v_type1, v_type2 > m_name ##  ResponseValues; \
    MOCKPP_DECLARE_PARAMETER_VARS2(m_name, v_type1, v_type2); \
    MOCKPP_VOID_CONTROLLER_DECL2(classname, m_name, v_type1, v_type2)


/** Implements a method with 3 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
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
#define MOCKPP_VOID_VISITABLE3_IMPL(classname, m_name, m_type1, m_type2, m_type3,  \
                                               a_name, v_type1, v_type2, v_type3) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         m_name ## Parameter1.addExpected((v_type1)param1); \
         m_name ## Parameter2.addExpected((v_type2)param2); \
         m_name ## Parameter3.addExpected((v_type3)param3); \
         return; \
      }    \
      else \
      {    \
         MOCKPP_TRY { \
           addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name);           \
           MOCKPP_NS::Throwable *t; \
           if (m_name ## ResponseValues.find(t, (v_type1)param1, (v_type2)param2, (v_type3)param3)) \
              t->throw_me(); \
           MOCKPP_THROWER_IMPL(m_name); \
         } MOCKPP_CATCH_ALL { \
           m_name ## Parameter1.balanceActual(); \
           m_name ## Parameter2.balanceActual(); \
           m_name ## Parameter3.balanceActual(); \
           MOCKPP_RETHROW; \
         } \
         m_name ## Parameter1.addActual((v_type1&)param1); \
         m_name ## Parameter2.addActual((v_type2&)param2); \
         m_name ## Parameter3.addActual((v_type3&)param3); \
         return; \
      }  \
    }     \
    \
    void a_name(const MOCKPP_NS::ConstraintHolder< v_type1 >  &param1, \
                const MOCKPP_NS::ConstraintHolder< v_type2 >  &param2, \
                const MOCKPP_NS::ConstraintHolder< v_type3 >  &param3) /* @todo const? */ \
    { \
      addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + #m_name);  \
      m_name ## Parameter1.addExpected(param1); \
      m_name ## Parameter2.addExpected(param2); \
      m_name ## Parameter3.addExpected(param3); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name); \
    mutable MOCKPP_NS::ResponseThrowableVector3< v_type1, v_type2, v_type3 > m_name ##  ResponseValues; \
    MOCKPP_DECLARE_PARAMETER_VARS3(m_name, v_type1, v_type2, v_type3); \
    MOCKPP_VOID_CONTROLLER_DECL3(classname, m_name, v_type1, v_type2, v_type3)


/** Implements a method with 4 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
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
#define MOCKPP_VOID_VISITABLE4_IMPL(classname, m_name, m_type1, m_type2, m_type3, m_type4, \
                                               a_name, v_type1, v_type2, v_type3, v_type4) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         m_name ## Parameter1.addExpected((v_type1)param1); \
         m_name ## Parameter2.addExpected((v_type2)param2); \
         m_name ## Parameter3.addExpected((v_type3)param3); \
         m_name ## Parameter4.addExpected((v_type4)param4); \
         return; \
      }    \
      else \
      {    \
         MOCKPP_TRY { \
           addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name);  \
           MOCKPP_NS::Throwable *t; \
           if (m_name ## ResponseValues.find(t, (v_type1)param1, (v_type2)param2, (v_type3)param3, (v_type4)param4)) \
              t->throw_me(); \
           MOCKPP_THROWER_IMPL(m_name); \
         } MOCKPP_CATCH_ALL { \
           m_name ## Parameter1.balanceActual(); \
           m_name ## Parameter2.balanceActual(); \
           m_name ## Parameter3.balanceActual(); \
           m_name ## Parameter4.balanceActual(); \
           MOCKPP_RETHROW; \
         } \
         m_name ## Parameter1.addActual((v_type1&)param1); \
         m_name ## Parameter2.addActual((v_type2&)param2); \
         m_name ## Parameter3.addActual((v_type3&)param3); \
         m_name ## Parameter4.addActual((v_type4&)param4); \
         return; \
      }  \
    }     \
    \
    void a_name(const MOCKPP_NS::ConstraintHolder< v_type1 >  &param1, \
                const MOCKPP_NS::ConstraintHolder< v_type2 >  &param2, \
                const MOCKPP_NS::ConstraintHolder< v_type3 >  &param3, \
                const MOCKPP_NS::ConstraintHolder< v_type4 >  &param4) /* @todo const? */ \
    { \
      addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + #m_name);  \
      m_name ## Parameter1.addExpected(param1); \
      m_name ## Parameter2.addExpected(param2); \
      m_name ## Parameter3.addExpected(param3); \
      m_name ## Parameter4.addExpected(param4); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name); \
    mutable MOCKPP_NS::ResponseThrowableVector4< v_type1, v_type2, v_type3, v_type4 > m_name ## ResponseValues; \
    MOCKPP_DECLARE_PARAMETER_VARS4(m_name, v_type1, v_type2, v_type3, v_type4); \
    MOCKPP_VOID_CONTROLLER_DECL4(classname, m_name, v_type1, v_type2, v_type3, v_type4)


/** Implements a method with 5 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
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
#define MOCKPP_VOID_VISITABLE5_IMPL(classname, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                               a_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
    { \
      MOCKPP_STL::string func_name (#m_name); \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         m_name ## Parameter1.addExpected((v_type1)param1); \
         m_name ## Parameter2.addExpected((v_type2)param2); \
         m_name ## Parameter3.addExpected((v_type3)param3); \
         m_name ## Parameter4.addExpected((v_type4)param4); \
         m_name ## Parameter5.addExpected((v_type5)param5); \
         return; \
      }    \
      else \
      {    \
         MOCKPP_TRY { \
           addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name);           \
           MOCKPP_NS::Throwable *t; \
           if (m_name ## ResponseValues.find(t, (v_type1)param1, (v_type2)param2, (v_type3)param3, (v_type4)param4, (v_type5)param5)) \
              t->throw_me(); \
           MOCKPP_THROWER_IMPL(m_name); \
         } MOCKPP_CATCH_ALL { \
           m_name ## Parameter1.balanceActual(); \
           m_name ## Parameter2.balanceActual(); \
           m_name ## Parameter3.balanceActual(); \
           m_name ## Parameter4.balanceActual(); \
           m_name ## Parameter5.balanceActual(); \
           MOCKPP_RETHROW; \
         } \
         m_name ## Parameter1.addActual((v_type1&)param1); \
         m_name ## Parameter2.addActual((v_type2&)param2); \
         m_name ## Parameter3.addActual((v_type3&)param3); \
         m_name ## Parameter4.addActual((v_type4&)param4); \
         m_name ## Parameter5.addActual((v_type5&)param5); \
         return; \
      }  \
    }     \
    \
    void a_name(const MOCKPP_NS::ConstraintHolder< v_type1 >  &param1, \
                const MOCKPP_NS::ConstraintHolder< v_type2 >  &param2, \
                const MOCKPP_NS::ConstraintHolder< v_type3 >  &param3, \
                const MOCKPP_NS::ConstraintHolder< v_type4 >  &param4, \
                const MOCKPP_NS::ConstraintHolder< v_type5 >  &param5) /* @todo const? */ \
    { \
      addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + #m_name);  \
      m_name ## Parameter1.addExpected(param1); \
      m_name ## Parameter2.addExpected(param2); \
      m_name ## Parameter3.addExpected(param3); \
      m_name ## Parameter4.addExpected(param4); \
      m_name ## Parameter5.addExpected(param5); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name); \
    mutable MOCKPP_NS::ResponseThrowableVector5< v_type1, v_type2, v_type3, v_type4, v_type5 > m_name ## ResponseValues; \
    MOCKPP_DECLARE_PARAMETER_VARS5(m_name, v_type1, v_type2, v_type3, v_type4, v_type5); \
    MOCKPP_VOID_CONTROLLER_DECL5(classname, m_name, v_type1, v_type2, v_type3, v_type4, v_type5)


/** Implements a method with no parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param v_ret_type   the data type of the internal return value respesentation
  */
#define MOCKPP_VISITABLE0_IMPL(classname, m_ret_type, m_name, \
                                          v_ret_type) \
    { \
      MOCKPP_STL::string func_name = #m_name; \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         v_ret_type ret_val = v_ret_type(); \
         return ret_val; \
      } \
      else \
      { \
         addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         MOCKPP_THROWER_IMPL(m_name); \
         \
         if (m_name ## ReturnValues.hasMoreObjects()) \
           return m_name ## ReturnValues.nextReturnObject(); \
         \
         MOCKPP_ASSERT_TRUE(m_name ## haveDefaultReturnValue == true); \
         m_name ## DefaultReturnValueUsed = true; \
         return m_name ## DefaultReturnValue; \
      } \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name); \
    MOCKPP_DECLARE_RETURN_VARS(v_ret_type, m_name); \
    MOCKPP_CONTROLLER_DECL0(classname, v_ret_type, m_name)


/** Implements a method with 1 parameter for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  */
#define MOCKPP_VISITABLE1_IMPL(classname, m_ret_type, m_name, m_type1, \
                                          v_ret_type, a_name, v_type1) \
    { \
      MOCKPP_STL::string func_name = #m_name; \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         m_name ## Parameter1.addExpected((v_type1)param1); \
         v_ret_type ret_val = v_ret_type(); \
         return ret_val; \
      } \
      else \
      { \
         MOCKPP_TRY { \
           addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
           MOCKPP_NS::Throwable *t; \
           if (m_name ## ResponseValues.find(t, (v_type1)param1)) \
              t->throw_me(); \
           MOCKPP_THROWER_IMPL(m_name); \
         } MOCKPP_CATCH_ALL { \
           m_name ## Parameter1.balanceActual(); \
           MOCKPP_RETHROW; \
         } \
         m_name ## Parameter1.addActual((v_type1&)param1); \
         \
         v_ret_type ret_val; \
         if (m_name ## ResponseValues.find(ret_val, (v_type1)param1)) \
            return ret_val; \
         if (m_name ## ReturnValues.hasMoreObjects()) \
           return m_name ## ReturnValues.nextReturnObject(); \
         MOCKPP_ASSERT_TRUE(m_name ## haveDefaultReturnValue == true); \
         m_name ## DefaultReturnValueUsed = true; \
         return m_name ## DefaultReturnValue; \
      } \
    } \
    \
    m_ret_type a_name(const MOCKPP_NS::ConstraintHolder< v_type1 >  &param1) /* @todo const? */ \
    { \
      addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + #m_name);  \
      m_name ## Parameter1.addExpected(param1); \
      return v_ret_type(); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name); \
    MOCKPP_DECLARE_RETURN_VARS(v_ret_type, m_name); \
    mutable MOCKPP_NS::ResponseVector1< v_ret_type, v_type1 > m_name ## ResponseValues; \
    MOCKPP_DECLARE_PARAMETER_VARS1(m_name, v_type1); \
    MOCKPP_CONTROLLER_DECL1(classname, v_ret_type, m_name, v_type1)


/** Implements a method with 2 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the extended method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param a_name       the actual method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  */
#define MOCKPP_VISITABLE2_IMPL(classname, m_ret_type, m_name, m_type1, m_type2, \
                                          v_ret_type, a_name, v_type1, v_type2) \
    { \
      MOCKPP_STL::string func_name = #m_name; \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         m_name ## Parameter1.addExpected((v_type1)param1); \
         m_name ## Parameter2.addExpected((v_type2)param2); \
         v_ret_type ret_val = v_ret_type(); \
         return ret_val; \
      } \
      else \
      { \
         MOCKPP_TRY { \
           addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
           MOCKPP_NS::Throwable *t; \
           if (m_name ## ResponseValues.find(t, (v_type1)param1, (v_type2)param2)) \
              t->throw_me(); \
           MOCKPP_THROWER_IMPL(m_name); \
         } MOCKPP_CATCH_ALL { \
           m_name ## Parameter1.balanceActual(); \
           m_name ## Parameter2.balanceActual(); \
           MOCKPP_RETHROW; \
         } \
         \
         m_name ## Parameter1.addActual((v_type1&)param1); \
         m_name ## Parameter2.addActual((v_type2&)param2); \
         v_ret_type ret_val; \
         if (m_name ## ResponseValues.find(ret_val, (v_type1)param1, (v_type2)param2)) \
            return ret_val; \
         if (m_name ## ReturnValues.hasMoreObjects()) \
           return m_name ## ReturnValues.nextReturnObject(); \
         MOCKPP_ASSERT_TRUE(m_name ## haveDefaultReturnValue == true); \
         m_name ## DefaultReturnValueUsed = true; \
         return m_name ## DefaultReturnValue; \
      } \
    } \
    \
    m_ret_type a_name(const MOCKPP_NS::ConstraintHolder< v_type1 >  &param1, \
                      const MOCKPP_NS::ConstraintHolder< v_type2 >  &param2) /* @todo const? */ \
    { \
      addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + #m_name);  \
      m_name ## Parameter1.addExpected(param1); \
      m_name ## Parameter2.addExpected(param2); \
      return v_ret_type(); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name); \
    MOCKPP_DECLARE_RETURN_VARS(v_ret_type, m_name); \
    mutable MOCKPP_NS::ResponseVector2< v_ret_type, v_type1, v_type2 > m_name ## ResponseValues; \
    MOCKPP_DECLARE_PARAMETER_VARS2(m_name, v_type1, v_type2); \
    MOCKPP_CONTROLLER_DECL2(classname, v_ret_type, m_name, v_type1, v_type2)


/** Implements a method with 3 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
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
#define MOCKPP_VISITABLE3_IMPL(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, \
                                          v_ret_type, a_name, v_type1, v_type2, v_type3) \
    { \
      MOCKPP_STL::string func_name = #m_name; \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         m_name ## Parameter1.addExpected((v_type1)param1); \
         m_name ## Parameter2.addExpected((v_type2)param2); \
         m_name ## Parameter3.addExpected((v_type3)param3); \
         v_ret_type ret_val = v_ret_type(); \
         return ret_val; \
      } \
      else \
      { \
         MOCKPP_TRY { \
           addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
           MOCKPP_NS::Throwable *t; \
           if (m_name ## ResponseValues.find(t, (v_type1)param1, (v_type2)param2, (v_type3)param3)) \
              t->throw_me(); \
           MOCKPP_THROWER_IMPL(m_name); \
         } MOCKPP_CATCH_ALL { \
           m_name ## Parameter1.balanceActual(); \
           m_name ## Parameter2.balanceActual(); \
           m_name ## Parameter3.balanceActual(); \
           MOCKPP_RETHROW; \
         } \
         \
         m_name ## Parameter1.addActual((v_type1&)param1); \
         m_name ## Parameter2.addActual((v_type2&)param2); \
         m_name ## Parameter3.addActual((v_type3&)param3); \
         v_ret_type ret_val; \
         if (m_name ## ResponseValues.find(ret_val, (v_type1)param1, (v_type2)param2, (v_type3)param3)) \
            return ret_val; \
         if (m_name ## ReturnValues.hasMoreObjects()) \
           return m_name ## ReturnValues.nextReturnObject(); \
         MOCKPP_ASSERT_TRUE(m_name ## haveDefaultReturnValue == true); \
         m_name ## DefaultReturnValueUsed = true; \
         return m_name ## DefaultReturnValue; \
      } \
    } \
    \
    m_ret_type a_name(const MOCKPP_NS::ConstraintHolder< v_type1 >  &param1, \
                      const MOCKPP_NS::ConstraintHolder< v_type2 >  &param2, \
                      const MOCKPP_NS::ConstraintHolder< v_type3 >  &param3) /* @todo const? */ \
    { \
      addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + #m_name);  \
      m_name ## Parameter1.addExpected(param1); \
      m_name ## Parameter2.addExpected(param2); \
      m_name ## Parameter3.addExpected(param3); \
      return v_ret_type(); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name); \
    MOCKPP_DECLARE_RETURN_VARS(v_ret_type, m_name); \
    mutable MOCKPP_NS::ResponseVector3< v_ret_type, v_type1, v_type2, v_type3 > m_name ## ResponseValues; \
    MOCKPP_DECLARE_PARAMETER_VARS3(m_name, v_type1, v_type2, v_type3); \
    MOCKPP_CONTROLLER_DECL3(classname, v_ret_type, m_name, v_type1, v_type2, v_type3)

/** Implements a method with 4 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
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
#define MOCKPP_VISITABLE4_IMPL(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, \
                                          v_ret_type, a_name, v_type1, v_type2, v_type3, v_type4) \
    { \
      MOCKPP_STL::string func_name = #m_name; \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         m_name ## Parameter1.addExpected((v_type1)param1); \
         m_name ## Parameter2.addExpected((v_type2)param2); \
         m_name ## Parameter3.addExpected((v_type3)param3); \
         m_name ## Parameter4.addExpected((v_type4)param4); \
         v_ret_type ret_val = v_ret_type(); \
         return ret_val; \
      } \
      else \
      { \
         MOCKPP_TRY { \
           addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
           MOCKPP_NS::Throwable *t; \
           if (m_name ## ResponseValues.find(t, (v_type1)param1, (v_type2)param2, (v_type3)param3, (v_type4)param4)) \
              t->throw_me(); \
           MOCKPP_THROWER_IMPL(m_name); \
         } MOCKPP_CATCH_ALL { \
           m_name ## Parameter1.balanceActual(); \
           m_name ## Parameter2.balanceActual(); \
           m_name ## Parameter3.balanceActual(); \
           m_name ## Parameter4.balanceActual(); \
           MOCKPP_RETHROW; \
         } \
         \
         m_name ## Parameter1.addActual((v_type1&)param1); \
         m_name ## Parameter2.addActual((v_type2&)param2); \
         m_name ## Parameter3.addActual((v_type3&)param3); \
         m_name ## Parameter4.addActual((v_type4&)param4); \
         v_ret_type ret_val; \
         if (m_name ## ResponseValues.find(ret_val, (v_type1)param1, (v_type2)param2, (v_type3)param3, (v_type4)param4)) \
            return ret_val; \
         if (m_name ## ReturnValues.hasMoreObjects()) \
           return m_name ## ReturnValues.nextReturnObject(); \
         MOCKPP_ASSERT_TRUE(m_name ## haveDefaultReturnValue == true); \
         m_name ## DefaultReturnValueUsed = true; \
         return m_name ## DefaultReturnValue; \
      } \
    } \
    \
    m_ret_type a_name(const MOCKPP_NS::ConstraintHolder< v_type1 >  &param1, \
                      const MOCKPP_NS::ConstraintHolder< v_type2 >  &param2, \
                      const MOCKPP_NS::ConstraintHolder< v_type3 >  &param3, \
                      const MOCKPP_NS::ConstraintHolder< v_type4 >  &param4) /* @todo const? */ \
    { \
      addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + #m_name);  \
      m_name ## Parameter1.addExpected(param1); \
      m_name ## Parameter2.addExpected(param2); \
      m_name ## Parameter3.addExpected(param3); \
      m_name ## Parameter4.addExpected(param4); \
      return v_ret_type(); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name); \
    MOCKPP_DECLARE_RETURN_VARS(v_ret_type, m_name); \
    mutable MOCKPP_NS::ResponseVector4< v_ret_type, v_type1, v_type2, v_type3, v_type4 > m_name ## ResponseValues; \
    MOCKPP_DECLARE_PARAMETER_VARS4(m_name, v_type1, v_type2, v_type3, v_type4); \
    MOCKPP_CONTROLLER_DECL4(classname, v_ret_type, m_name, v_type1, v_type2, v_type3, v_type4)

/** Implements a method with 5 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
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
#define MOCKPP_VISITABLE5_IMPL(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                          v_ret_type, a_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
    { \
      MOCKPP_STL::string func_name = #m_name; \
      if (!isActivated() ) \
      { \
         addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
         m_name ## Parameter1.addExpected((v_type1)param1); \
         m_name ## Parameter2.addExpected((v_type2)param2); \
         m_name ## Parameter3.addExpected((v_type3)param3); \
         m_name ## Parameter4.addExpected((v_type4)param4); \
         m_name ## Parameter5.addExpected((v_type5)param5); \
         v_ret_type ret_val = v_ret_type(); \
         return ret_val; \
      } \
      else \
      { \
         MOCKPP_TRY { \
           addActualMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + func_name); \
           MOCKPP_NS::Throwable *t; \
           if (m_name ## ResponseValues.find(t, (v_type1)param1, (v_type2)param2, (v_type3)param3, (v_type4)param4, (v_type5)param5)) \
              t->throw_me(); \
           MOCKPP_THROWER_IMPL(m_name); \
         } MOCKPP_CATCH_ALL { \
           m_name ## Parameter1.balanceActual(); \
           m_name ## Parameter2.balanceActual(); \
           m_name ## Parameter3.balanceActual(); \
           m_name ## Parameter4.balanceActual(); \
           m_name ## Parameter5.balanceActual(); \
           MOCKPP_RETHROW; \
         } \
         \
         m_name ## Parameter1.addActual((v_type1&)param1); \
         m_name ## Parameter2.addActual((v_type2&)param2); \
         m_name ## Parameter3.addActual((v_type3&)param3); \
         m_name ## Parameter4.addActual((v_type4&)param4); \
         m_name ## Parameter5.addActual((v_type5&)param5); \
         v_ret_type ret_val; \
         if (m_name ## ResponseValues.find(ret_val, (v_type1)param1, (v_type2)param2, (v_type3)param3, (v_type4)param4, (v_type5)param5)) \
            return ret_val; \
         if (m_name ## ReturnValues.hasMoreObjects()) \
           return m_name ## ReturnValues.nextReturnObject(); \
         MOCKPP_ASSERT_TRUE(m_name ## haveDefaultReturnValue == true); \
         m_name ## DefaultReturnValueUsed = true; \
         return m_name ## DefaultReturnValue; \
      } \
    } \
    \
    m_ret_type a_name(const MOCKPP_NS::ConstraintHolder< v_type1 >  &param1, \
                      const MOCKPP_NS::ConstraintHolder< v_type2 >  &param2, \
                      const MOCKPP_NS::ConstraintHolder< v_type3 >  &param3, \
                      const MOCKPP_NS::ConstraintHolder< v_type4 >  &param4, \
                      const MOCKPP_NS::ConstraintHolder< v_type5 >  &param5) /* @todo const? */ \
    { \
      addExpectedMethod(MOCKPP_NS::getLatin1(getVerifiableName()) + "." + #m_name);  \
      m_name ## Parameter1.addExpected(param1); \
      m_name ## Parameter2.addExpected(param2); \
      m_name ## Parameter3.addExpected(param3); \
      m_name ## Parameter4.addExpected(param4); \
      m_name ## Parameter5.addExpected(param5); \
      return v_ret_type(); \
    } \
  MOCKPP_MEMBER_RESTRICTOR_PRIVATE: \
    MOCKPP_DECLARE_DEFAULT_VARS(m_name); \
    MOCKPP_DECLARE_RETURN_VARS(v_ret_type, m_name); \
    mutable MOCKPP_NS::ResponseVector5< v_ret_type, v_type1, v_type2, v_type3, v_type4, v_type5 > m_name ## ResponseValues; \
    MOCKPP_DECLARE_PARAMETER_VARS5(m_name, v_type1, v_type2, v_type3, v_type4, v_type5); \
    MOCKPP_CONTROLLER_DECL5(classname, v_ret_type, m_name, v_type1, v_type2, v_type3, v_type4, v_type5)


#endif // DOXYGEN_SHOULD_SKIP_THIS


#endif // MOCKPP_VISITABLEMOCKOBJECT_MACRO_H
