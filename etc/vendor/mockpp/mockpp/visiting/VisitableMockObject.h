/** @file
    @brief MockObjects that can be visited under control.

  $Id: VisitableMockObject.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Fri Dec 18 2002
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

#ifndef MOCKPP_VISITABLEMOCKOBJECT_H
#define MOCKPP_VISITABLEMOCKOBJECT_H

#include <mockpp/mockpp.h> // always first

#include MOCKPP_VECTOR_H

#include <mockpp/MockObject.h>
#include <mockpp/Throwable.h>
#include <mockpp/ThrowableList.h>
#include <mockpp/ExpectationList.h>
#include <mockpp/ExpectationCounter.h>
#include <mockpp/ReturnObjectList.h>

#include <mockpp/constraint/ConstraintList.h>

#include <mockpp/visiting/VisitableMockObject_macro.h>
#include <mockpp/visiting/VisitableMockObject_template.h>


MOCKPP_NS_START


/** Base class for a generic object that can be used to replace a real world object for testing purposes.
  * It emulates the real world behaviour by feeding it values and exceptions.
  * \ingroup grp_advanced_mo
  */
class MOCKPP_API_DECL0 VisitableMockObjectBase
{
  public:

  /** Construct the item.
    * @param name    human readable description about the object
    * @param parent  parent verifiable
    */
    VisitableMockObjectBase(const String &name, VerifiableList *parent);

  /** Destruct the item.
    */
    virtual ~VisitableMockObjectBase();

  /** Changes from record mode to normal working mode so you can use the object
    */
    void activate();

  /** Switches exception handling back to pre-1.2 behaviour.
    * @see VisitableMockObject::Controller::unsetThrowablesInline()
    */
    void unsetThrowablesInline();

    class Controller;
    friend class Controller;

  /** Adds another expected call to a mock method.
    * @param name unique identifier for method
    */
    void addExpectedMethod(const MOCKPP_STL::string &name) const;

  /** Adds another actual call to a mock method.
    * @param name unique identifier for method
    */
    void addActualMethod(const MOCKPP_STL::string &name) const;

  /** Get information about working mode.
    * @return  false: object is in record mode
    * @return  true:  object is in real world working mode
    */
    bool isActivated() const;

  /** Adds another mock controller.
    * @param  pc  pointer to controller
    */
    void addController(Controller *pc);

  /** Removes a mock controller.
    * @param  pc  pointer to controller
    */
    void removeController(Controller *pc);

  /** Gets the number of attached controllers.
    * @return number of controllers
    */
    unsigned numController() const;

   protected:

  /** Verify that the expected and actual values are equal.
    * If it fails, an AssertionFailedError is thrown
    */
    void visitableVerify();

  /** Sets all internal objects to the state after construction.
    */
    void visitableReset();

  private:

    VisitableMockObjectBase (const VisitableMockObjectBase &);    // forbid
    VisitableMockObjectBase& operator=(VisitableMockObjectBase&);

    mutable ExpectationList<MOCKPP_STL::string> methodList;
    mutable bool                         activated;
    MOCKPP_STL::vector<Controller*>             controllers;
};


/** A generic object that can be used to replace a real world object for testing purposes.
  * It emulates the real world behaviour by feeding it values and exceptions.
  * \ingroup grp_advanced_mo
  */
class MOCKPP_API_DECL0 VisitableMockObject : public MockObject
                                           , public VisitableMockObjectBase
{
  public:

  /**
    * Construct the item.
    * @param name    human readable description about the object
    * @param parent  parent verifiable
    */
    VisitableMockObject(const String &name, VerifiableList *parent = 0);

  /** Destruct the item.
    */
    virtual ~VisitableMockObject();

  /**
    * Verify that the expected and actual values are equal.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify();

/**
    * Sets all internal objects to the state after construction.
    */
    virtual void reset();
};



/**
  * @defgroup grp_controller Controlling behaviour of a Visitable Mock Object
  */

/** Virtual base class for helper objects to easily set up the expectations.
  *
  * Due to the virtual nature of adding functionality with macros you will find
  * here the documentation of the api generated by the macros.
  * @ingroup grp_controller
  */
class MOCKPP_API_DECL0 VisitableMockObjectBase::Controller
{
  public:

  /** Destructs a controller for the according method.
    * Basically unregisters the controller with it's mock object.
    * @internal
    */
    virtual ~Controller() = 0;

  /** Clears all expectations and puts the object in record mode again.
    */
    virtual void reset() = 0;

  /** Switches exception handling back to pre-1.2 behaviour.
    * This affects behaviour created with calls to addThrowable() and addReturnValue().
    * Starting with version 1.2 return values and exceptions have equal priority
    * and are processed in the order they are listed in the sources.
    * Older versions threw exceptions if available before handling
    * potential return values.
    */
    virtual void unsetThrowablesInline() = 0;

  /**
    * Verify the conditions which are not in the verify() chain of the
    * visitable mock object. Located here instead of the object for traditional
    * reasons.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify() = 0;

#ifdef DOXYGEN_SHOULD_SKIP_THIS // just for documentation purposes

  /** Constructs a controller for the according method.
    * Basically registers the ccontroller with it's mock object.
    * @param  cls   the pointer to the parent mock object.
    * @internal
    */
    Controller (Your_VisitableMockObject *cls);

  /** Adds another throwable to the method.
    * The throwables are thrown one after the other when the method is
    * called after activating until all of them are used.
    * @param  t   a pointer to the throwable
    */
    void addThrowable (Throwable * t);

  /** Sets the default throwable.
    * Once the list of throwables is empty this one is thrown. This also
    * means that your method never processes the parameters passed to it.
    * @param  t   a pointer to the throwable
    */
    void setDefaultThrowable (Throwable * t);

  /** Adds another return value.
    * The values from this list are return one after the other unless there
    * are no other throwables or response values available.
    * @param  rv      the next return value
    * @param  count   the number of times this value shall be returned
    */
    void addReturnValue (const your_type &rv, unsigned count = 1);

  /** Sets the default return value.
    * This value is returned if there are no throwables available and if there
    * are no other possible return values in a list.
    * @param  rv      the default return value
    */
    void setDefaultReturnValue (const your_type &rv);

  /** Sets the default return value.
    * This value is returned if there are no throwables available and if there
    * are no other possible return values in a list.
    * @param  rv      the default return value
    */
    void setDefaultReturnValue (const your_type &rv);

  /** Adds another response value.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * @param  rv      the return value
    * @param  p1      the 1. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue (const your_type &rv,
                           const your_type1 &p1,
                           unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response throwable.
    * Response throwables are determined on the parameters you pass. This way the
    * object throws an object that is totally based on the input.
    * @param  t       the throwable
    * @param  p1      the 1. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable (Throwable *t,
                               const your_type1 &p1,
                               unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response value.
    * Response values are determined on the parameter constraints you pass. This way the
    * object returns a value that is totally based on the input.
    * @param  rv      the return value
    * @param  p1      the 1. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue(const ret_type &rv,
                          const MOCKPP_NS::ConstraintHolder<type1> &p1,
                          unsigned count = MOCKPP_UNLIMITED);

    /** Adds another response throwable.
    * Response throwables are determined on the parameter constraints you pass. This way the
    * object throws an object that is totally based on the input.
    * @param  t       the throwable
    * @param  p1      the 1. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable(MOCKPP_NS::Throwable *t,
                              const MOCKPP_NS::ConstraintHolder<type1> &p1,
                              unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response value.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * @param  rv      the return value
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue (const your_type &rv,
                           const your_type1 &p1, const your_type2 &p2,
                           unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response throwable.
    * Response throwables are determined on the parameters you pass. This way the
    * object throws an object that is totally based on the input.
    * @param  t       the throwable
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable (Throwable *t,
                               const your_type1 &p1, const your_type2 &p2,
                               unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response value.
    * Response values are determined on the parameter constraints you pass. This way the
    * object returns a value that is totally based on the input.
    * @param  rv      the return value
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue(const ret_type &rv,
                          const MOCKPP_NS::ConstraintHolder<type1> &p1,
                          const MOCKPP_NS::ConstraintHolder<type2> &p2,
                          unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response throwable.
    * Response throwables are determined on the parameter constraints you pass. This way the
    * object throws an object that is totally based on the input.
    * @param  t       the throwable
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable(MOCKPP_NS::Throwable *t,
                              const MOCKPP_NS::ConstraintHolder<type1> &p1,
                              const MOCKPP_NS::ConstraintHolder<type2> &p2,
                              unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response value.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * @param  rv      the return value
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue (const your_type &rv,
                           const your_type1 &p1, const your_type2 &p2, const your_type3 &p3,
                           unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response throwable.
    * Response throwables are determined on the parameters you pass. This way the
    * object throws an object that is totally based on the input.
    * @param  t       the throwable
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable (Throwable *t,
                               const your_type1 &p1, const your_type2 &p2, const your_type3 &p3,
                               unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response value.
    * Response values are determined on the parameter constraints you pass. This way the
    * object returns a value that is totally based on the input.
    * @param  rv      the return value
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue(const ret_type &rv,
                          const MOCKPP_NS::ConstraintHolder<type1> &p1,
                          const MOCKPP_NS::ConstraintHolder<type2> &p2,
                          const MOCKPP_NS::ConstraintHolder<type3> &p3,
                          unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response throwable.
    * Response throwables are determined on the parameter constraints you pass. This way the
    * object throws an object that is totally based on the input.
    * @param  t       the throwable
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable(MOCKPP_NS::Throwable *t,
                              const MOCKPP_NS::ConstraintHolder<type1> &p1,
                              const MOCKPP_NS::ConstraintHolder<type2> &p2,
                              const MOCKPP_NS::ConstraintHolder<type3> &p3,
                              unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response value.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * @param  rv      the return value
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  p4      the 4. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue (const your_type &rv,
                           const your_type1 &p1, const your_type2 &p2, const your_type3 &p3,
                           const your_type4 &p4,
                           unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response throwable.
    * Response throwables are determined on the parameters you pass. This way the
    * object throws an object that is totally based on the input.
    * @param  t       the throwable
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  p4      the 4. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable (Throwable *t,
                               const your_type1 &p1, const your_type2 &p2, const your_type3 &p3,
                               const your_type4 &p4,
                               unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response value.
    * Response values are determined on the parameter constraints you pass. This way the
    * object returns a value that is totally based on the input.
    * @param  rv      the return value
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  p4      the 4. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue(const ret_type &rv,
                          const MOCKPP_NS::ConstraintHolder<type1> &p1,
                          const MOCKPP_NS::ConstraintHolder<type2> &p2,
                          const MOCKPP_NS::ConstraintHolder<type3> &p3,
                          const MOCKPP_NS::ConstraintHolder<type4> &p4,
                          unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response throwable.
    * Response throwables are determined on the parameter constraints you pass. This way the
    * object throws an object that is totally based on the input.
    * @param  t       the throwable
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  p4      the 4. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable(MOCKPP_NS::Throwable *t,
                              const MOCKPP_NS::ConstraintHolder<type1> &p1,
                              const MOCKPP_NS::ConstraintHolder<type2> &p2,
                              const MOCKPP_NS::ConstraintHolder<type3> &p3,
                              const MOCKPP_NS::ConstraintHolder<type4> &p4,
                              unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response value.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * @param  rv      the return value
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  p4      the 4. parameter of the method
    * @param  p5      the 5. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue (const your_type &rv,
                           const your_type1 &p1, const your_type2 &p2, const your_type3 &p3,
                           const your_type4 &p4, const your_type5 &p5,
                           unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response throwable.
    * Response throwables are determined on the parameters you pass. This way the
    * object throws an object that is totally based on the input.
    * @param  t       the throwable
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  p4      the 4. parameter of the method
    * @param  p5      the 5. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable (Throwable *t,
                               const your_type1 &p1, const your_type2 &p2, const your_type3 &p3,
                               const your_type4 &p4, const your_type5 &p5,
                               unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response value.
    * Response values are determined on the parameter constraints you pass. This way the
    * object returns a value that is totally based on the input.
    * @param  rv      the return value
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  p4      the 4. parameter of the method
    * @param  p5      the 5. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue(const ret_type &rv,
                          const MOCKPP_NS::ConstraintHolder<type1> &p1,
                          const MOCKPP_NS::ConstraintHolder<type2> &p2,
                          const MOCKPP_NS::ConstraintHolder<type3> &p3,
                          const MOCKPP_NS::ConstraintHolder<type4> &p4,
                          const MOCKPP_NS::ConstraintHolder<type5> &p5,
                          unsigned count = MOCKPP_UNLIMITED);

  /** Adds another response throwable.
    * Response throwables are determined on the parameter constraints you pass. This way the
    * object throws an object that is totally based on the input.
    * @param  t       the throwable
    * @param  p1      the 1. parameter of the method
    * @param  p2      the 2. parameter of the method
    * @param  p3      the 3. parameter of the method
    * @param  p4      the 4. parameter of the method
    * @param  p5      the 5. parameter of the method
    * @param  count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable(MOCKPP_NS::Throwable *t,
                              const MOCKPP_NS::ConstraintHolder<type1> &p1,
                              const MOCKPP_NS::ConstraintHolder<type2> &p2,
                              const MOCKPP_NS::ConstraintHolder<type3> &p3,
                              const MOCKPP_NS::ConstraintHolder<type4> &p4,
                              const MOCKPP_NS::ConstraintHolder<type5> &p5,
                              unsigned count = MOCKPP_UNLIMITED);

#endif // DOXYGEN_SHOULD_SKIP_THIS

} ;


MOCKPP_NS_END


//////////////////////////////////////////////////////////////
//

/** Implements a controller for a method.
  * Serves also as a backwards compatibility macro.
  * @ingroup grp_controller
  * @see mockpp::VisitableMockObject::Controller
  * @param clsname  the visitable mock object class name
  * @param methname the method for which you create the controller
  */
#define MOCKPP_CONTROLLER_FOR(clsname, methname) \
    clsname::ControllerFor ## methname

/** Implements a controller for a method.
  * @ingroup grp_controller
  * @see mockpp::VisitableMockObject::Controller
  * @param clsname     the visitable mock object class name
  * @param m_methname  the method for which you create the controller
  * @param x_methname  optional extension for the internal names based on the method name
  */
#define MOCKPP_CONTROLLER_FOR_EXT(clsname, m_methname, x_methname) \
  MOCKPP_CONTROLLER_FOR(clsname, m_methname ## x_methname)

//////////////////////////////////////////////////////////////
//

/** Implements a method with 0 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  */
#define MOCKPP_VOID_VISITABLE0(classname, name) \
  public: \
    void name() \
  MOCKPP_VOID_VISITABLE0_IMPL(classname, name)

/** Implements a const method with 0 parameters for a mock object.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  */
#define MOCKPP_VOID_CONST_VISITABLE0(classname, name) \
  public: \
    void name() const \
  MOCKPP_VOID_VISITABLE0_IMPL(classname, name)


/** Implements a method with 0 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param x_name       optional extension for the internal variable names based on the method name
  */
#define MOCKPP_VOID_VISITABLE_EXT0(classname, m_name, \
                                              x_name) \
  public: \
    void m_name() \
  MOCKPP_VOID_VISITABLE0_IMPL(classname, m_name ## x_name)

/** Implements a const method with 0 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param x_name       optional extension for the internal variable names based on the method name
  */
#define MOCKPP_VOID_CONST_VISITABLE_EXT0(classname, m_name, \
                                                    x_name) \
  public: \
    void m_name() const \
  MOCKPP_VOID_VISITABLE0_IMPL(classname, m_name ## x_name)

////////////////////////////////////////////////////////////////////////////////

/** Implements a method with 1 parameter for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  */
#define MOCKPP_VOID_VISITABLE1(classname, name, type1) \
  public: \
    void name(const type1 &param1) \
  MOCKPP_VOID_VISITABLE1_IMPL(classname, name, const type1 &, \
                                         name, type1)

/** Implements a const method with 1 parameter for a mock object.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  */
#define MOCKPP_VOID_CONST_VISITABLE1(classname, name, type1) \
  public: \
    void name(const type1 &param1) const \
  MOCKPP_VOID_VISITABLE1_IMPL(classname, name, const type1 &, \
                                         name, type1)

/** Implements a method with 1 parameter for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  */
#define MOCKPP_VOID_VISITABLE_EXT1(classname, m_name, m_type1, \
                                              x_name, v_type1) \
  public: \
    void m_name(m_type1 param1) \
  MOCKPP_VOID_VISITABLE1_IMPL(classname, m_name ## x_name, m_type1, \
                                         m_name,           v_type1)

/** Implements a const method with 1 parameter for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  */
#define MOCKPP_VOID_CONST_VISITABLE_EXT1(classname, m_name, m_type1, \
                                                    x_name, v_type1) \
  public: \
    void m_name(m_type1 param1) const \
  MOCKPP_VOID_VISITABLE1_IMPL(classname, m_name ## x_name, m_type1, \
                                         m_name,           v_type1)

////////////////////////////////////////////////////////////////////////////////

/** Implements a method with 2 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  */
#define MOCKPP_VOID_VISITABLE2(classname, name, type1, type2) \
  public: \
    void name(const type1 &param1, const type2 &param2) \
  MOCKPP_VOID_VISITABLE2_IMPL(classname, name, const type1 &, const type2 &,  \
                                         name, type1,         type2)

/** Implements a const method with 2 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  */
#define MOCKPP_VOID_CONST_VISITABLE2(classname, name, type1, type2) \
  public: \
    void name(const type1 &param1, const type2 &param2) const \
  MOCKPP_VOID_VISITABLE2_IMPL(classname, name, const type1 &, const type2 &,  \
                                         name, type1,         type2)

/** Implements a method with 2 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  */
#define MOCKPP_VOID_VISITABLE_EXT2(classname, m_name, m_type1, m_type2,  \
                                              x_name, v_type1, v_type2) \
  public: \
    void m_name(m_type1 param1, m_type2 param2) \
  MOCKPP_VOID_VISITABLE2_IMPL(classname, m_name ## x_name, m_type1, m_type2,  \
                                         m_name,           v_type1, v_type2)

/** Implements a const method with 2 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  */
#define MOCKPP_VOID_CONST_VISITABLE_EXT2(classname, m_name, m_type1, m_type2, \
                                                    x_name, v_type1, v_type2) \
  public: \
    void m_name(m_type1 param1, m_type2 param2) const \
  MOCKPP_VOID_VISITABLE2_IMPL(classname, m_name ## x_name, m_type1, m_type2,  \
                                         m_name,           v_type1, v_type2)

////////////////////////////////////////////////////////////////////////////////

/** Implements a method with 3 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  */
#define MOCKPP_VOID_VISITABLE3(classname, name, type1, type2, type3) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3) \
  MOCKPP_VOID_VISITABLE3_IMPL(classname, name, const type1 &, const type2 &, const type3 &, \
                                         name, type1,         type2,         type3)

/** Implements a const method with 3 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  */
#define MOCKPP_VOID_CONST_VISITABLE3(classname, name, type1, type2, type3) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3) const \
  MOCKPP_VOID_VISITABLE3_IMPL(classname, name, const type1 &, const type2 &, const type3 &, \
                                         name, type1,         type2,         type3)

/** Implements a method with 3 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  */
#define MOCKPP_VOID_VISITABLE_EXT3(classname, m_name, m_type1, m_type2, m_type3,  \
                                              x_name, v_type1, v_type2, v_type3) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3) \
  MOCKPP_VOID_VISITABLE3_IMPL(classname, m_name ## x_name, m_type1, m_type2, m_type3, \
                                         m_name,           v_type1, v_type2, v_type3)

/** Implements a const method with 3 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  */
#define MOCKPP_VOID_CONST_VISITABLE_EXT3(classname, m_name, m_type1, m_type2, m_type3,  \
                                                    x_name, v_type1, v_type2, v_type3) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3) const \
  MOCKPP_VOID_VISITABLE3_IMPL(classname, m_name ## x_name, m_type1, m_type2, m_type3, \
                                         m_name,           v_type1, v_type2, v_type3)

////////////////////////////////////////////////////////////////////////////////

/** Implements a method with 4 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  */
#define MOCKPP_VOID_VISITABLE4(classname, name, type1, type2, type3, type4) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4) \
  MOCKPP_VOID_VISITABLE4_IMPL(classname, name, const type1 &, const type2 &, const type3 &, const type4 &, \
                                         name, type1,         type2,         type3,         type4)

/** Implements a const method with 4 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  */
#define MOCKPP_VOID_CONST_VISITABLE4(classname, name, type1, type2, type3, type4) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4) const\
  MOCKPP_VOID_VISITABLE4_IMPL(classname, name, const type1 &, const type2 &, const type3 &, const type4 &, \
                                         name, type1,         type2,         type3,         type4)

/** Implements a method with 4 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  */
#define MOCKPP_VOID_VISITABLE_EXT4(classname, m_name, m_type1, m_type2, m_type3, m_type4, \
                                              x_name, v_type1, v_type2, v_type3, v_type4) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4) \
  MOCKPP_VOID_VISITABLE4_IMPL(classname, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, \
                                         m_name,           v_type1, v_type2, v_type3, v_type4)

/** Implements a const method with 4 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  */
#define MOCKPP_VOID_CONST_VISITABLE_EXT4(classname, m_name, m_type1, m_type2, m_type3, m_type4, \
                                                    x_name, v_type1, v_type2, v_type3, v_type4) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4) const\
  MOCKPP_VOID_VISITABLE4_IMPL(classname, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, \
                                         m_name,           v_type1, v_type2, v_type3, v_type4)

////////////////////////////////////////////////////////////////////////////////

/** Implements a method with 5 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  * @param type5      the data type of the 5. parameter
  */
#define MOCKPP_VOID_VISITABLE5(classname, name, type1, type2, type3, type4, type5) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4, const type5 &param5) \
  MOCKPP_VOID_VISITABLE5_IMPL(classname, name, const type1 &, const type2 &, const type3 &, const type4 &, const type5 &, \
                                         name, type1,         type2,         type3,         type4,         type5)

/** Implements a const method with 5 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  * @param type5      the data type of the 5. parameter
  */
#define MOCKPP_VOID_CONST_VISITABLE5(classname, name, type1, type2, type3, type4, type5) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4, const type5 &param5) const \
  MOCKPP_VOID_VISITABLE5_IMPL(classname, name, const type1 &, const type2 &, const type3 &, const type4 &, const type5 &, \
                                         name, type1,         type2,         type3,         type4,         type5)

/** Implements a method with 5 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param m_type5      the data type of the 5. method parameter
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  * @param v_type5      the data type of the internal variable of 5. parameter
  */
#define MOCKPP_VOID_VISITABLE_EXT5(classname, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                              x_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4, m_type5 param5) \
  MOCKPP_VOID_VISITABLE5_IMPL(classname, m_name ##x_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                         m_name,          v_type1, v_type2, v_type3, v_type4, v_type5)

/** Implements a const method with 5 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param m_type5      the data type of the 5. method parameter
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  * @param v_type5      the data type of the internal variable of 5. parameter
  */
#define MOCKPP_VOID_CONST_VISITABLE_EXT5(classname, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                                    x_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4, m_type5 param5) const \
  MOCKPP_VOID_VISITABLE5_IMPL(classname, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                         m_name,           v_type1, v_type2, v_type3, v_type4, v_type5)

////////////////////////////////////////////////////////////////////////////////

/** Implements a method with no parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  */
#define MOCKPP_VISITABLE0(classname, ret_type, name) \
  public: \
    ret_type name() \
  MOCKPP_VISITABLE0_IMPL(classname, ret_type, name, \
                                    ret_type)

/** Implements a const method with no parameters for a mock object.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  */
#define MOCKPP_CONST_VISITABLE0(classname, ret_type, name) \
  public: \
    ret_type name() const \
  MOCKPP_VISITABLE0_IMPL(classname, ret_type, name, \
                                    ret_type)

/** Implements a method with no parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  */
#define MOCKPP_VISITABLE_EXT0(classname, m_ret_type, m_name, \
                                         v_ret_type, x_name) \
  public: \
    m_ret_type m_name() \
  MOCKPP_VISITABLE0_IMPL(classname, m_ret_type, m_name ## x_name, \
                                    v_ret_type)

/** Implements a const method with no parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  */
#define MOCKPP_CONST_VISITABLE_EXT0(classname, m_ret_type, m_name, \
                                               v_ret_type, x_name) \
  public: \
    m_ret_type m_name() const \
  MOCKPP_VISITABLE0_IMPL(classname, m_ret_type, m_name ## x_name, \
                                    v_ret_type)


////////////////////////////////////////////////////////////////////////////////

/** Implements a method with 1 parameter for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  */
#define MOCKPP_VISITABLE1(classname, ret_type, name, type1) \
  public: \
    ret_type name(const type1 &param1) \
  MOCKPP_VISITABLE1_IMPL(classname, ret_type, name, const type1 &, \
                                    ret_type, name, type1)

/** Implements a const method with 1 parameter for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  */
#define MOCKPP_CONST_VISITABLE1(classname, ret_type, name, type1) \
  public: \
    ret_type name(const type1 &param1) const \
  MOCKPP_VISITABLE1_IMPL(classname, ret_type, name, const type1 &, \
                                    ret_type, name, type1)

/** Implements a method with 1 parameter for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  */
#define MOCKPP_VISITABLE_EXT1(classname, m_ret_type, m_name, m_type1, \
                                         v_ret_type, x_name, v_type1) \
  public: \
    m_ret_type m_name(m_type1 param1) \
  MOCKPP_VISITABLE1_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, \
                                    v_ret_type, m_name,           v_type1)

/** Implements a const method with 1 parameter for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  */
#define MOCKPP_CONST_VISITABLE_EXT1(classname, m_ret_type, m_name, m_type1, \
                                               v_ret_type, x_name, v_type1) \
  public: \
    m_ret_type m_name(m_type1 param1) const \
  MOCKPP_VISITABLE1_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, \
                                    v_ret_type, m_name,           v_type1)

////////////////////////////////////////////////////////////////////////////////

/** Implements a method with 2 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  */
#define MOCKPP_VISITABLE2(classname, ret_type, name, type1, type2) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2) \
  MOCKPP_VISITABLE2_IMPL(classname, ret_type, name, const type1 &, const type2 &, \
                                    ret_type, name, type1,         type2)

/** Implements a const method with 2 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  */
#define MOCKPP_CONST_VISITABLE2(classname, ret_type, name, type1, type2) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2) const \
  MOCKPP_VISITABLE2_IMPL(classname, ret_type, name, const type1 &, const type2 &, \
                                    ret_type, name, type1,         type2)

/** Implements a method with 2 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  */
#define MOCKPP_VISITABLE_EXT2(classname, m_ret_type, m_name, m_type1, m_type2, \
                                         v_ret_type, x_name, v_type1, v_type2) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2) \
  MOCKPP_VISITABLE2_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, \
                                    v_ret_type, m_name,           v_type1, v_type2)

/** Implements a const method with 2 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  */
#define MOCKPP_CONST_VISITABLE_EXT2(classname, m_ret_type, m_name, m_type1, m_type2, \
                                               v_ret_type, x_name, v_type1, v_type2) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2) const \
  MOCKPP_VISITABLE2_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, \
                                    v_ret_type, m_name,           v_type1, v_type2)


////////////////////////////////////////////////////////////////////////////////

/** Implements a method with 3 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  */
#define MOCKPP_VISITABLE3(classname, ret_type, name, type1, type2, type3) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3) \
  MOCKPP_VISITABLE3_IMPL(classname, ret_type, name, const type1 &, const type2 &, const type3, \
                                    ret_type, name, type1,         type2,         type3)

/** Implements a const method with 3 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  */
#define MOCKPP_CONST_VISITABLE3(classname, ret_type, name, type1, type2, type3) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3) const \
  MOCKPP_VISITABLE3_IMPL(classname, ret_type, name, const type1 &, const type2 &, const type3, \
                                    ret_type, name, type1,         type2,         type3)

/** Implements a method with 3 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  */
#define MOCKPP_VISITABLE_EXT3(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, \
                                         v_ret_type, x_name, v_type1, v_type2, v_type3) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3) \
  MOCKPP_VISITABLE3_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, \
                                    v_ret_type, m_name,           v_type1, v_type2, v_type3)

/** Implements a const method with 3 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  */
#define MOCKPP_CONST_VISITABLE_EXT3(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, \
                                               v_ret_type, x_name, v_type1, v_type2, v_type3) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3) const \
  MOCKPP_VISITABLE3_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, \
                                    v_ret_type, m_name,           v_type1, v_type2, v_type3)


////////////////////////////////////////////////////////////////////////////////

/** Implements a method with 4 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  */
#define MOCKPP_VISITABLE4(classname, ret_type, name, type1, type2, type3, type4) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4) \
  MOCKPP_VISITABLE4_IMPL(classname, ret_type, name, const m_type1 &, const type2 &, const type3 &, const type4 &, \
                                    ret_type, name, type1,           type2,         type3,         type4)

/** Implements a const method with 4 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param ret_type   the data type of the return value
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  */
#define MOCKPP_CONST_VISITABLE4(classname, ret_type, name, type1, type2, type3, type4) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4) const \
  MOCKPP_VISITABLE4_IMPL(classname, ret_type, name, const type1 &, const type2 &, const type3 &, const type4 &, \
                                    ret_type, name, type1,         type2,         type3,         type4)

/** Implements a method with 4 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  */
#define MOCKPP_VISITABLE_EXT4(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, \
                                         v_ret_type, x_name, v_type1, v_type2, v_type3, v_type4) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4) \
  MOCKPP_VISITABLE4_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, \
                                    v_ret_type, m_name,           v_type1, v_type2, v_type3, v_type4)

/** Implements a const method with 4 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  */
#define MOCKPP_CONST_VISITABLE_EXT4(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, \
                                               v_ret_type, x_name, v_type1, v_type2, v_type3, v_type4) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4) const \
  MOCKPP_VISITABLE4_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, \
                                    v_ret_type, m_name,           v_type1, v_type2, v_type3, v_type4)


////////////////////////////////////////////////////////////////////////////////

/** Implements a method with 5 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  * @param type5      the data type of the 5. parameter
  */
#define MOCKPP_VISITABLE5(classname, ret_type, name, type1, type2, type3, type4, type5) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4, const type5 &param5) \
  MOCKPP_VISITABLE5_IMPL(classname, ret_type, name, const type1 &, const type2 &, const type3 &, const type4 &, const type5 &, \
                                    ret_type, name, type1,         type2,         type3,         type4,         type5)

/** Implements a const method with 5 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  * @param type5      the data type of the 5. parameter
  */
#define MOCKPP_CONST_VISITABLE5(classname, ret_type, name, type1, type2, type3, type4, type5) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4, const type5 &param5) const \
  MOCKPP_VISITABLE5_IMPL(classname, ret_type, name, const type1 &, const type2 &, const type3 &, const type4 &, const type5 &, \
                                    ret_type, name, type1,         type2,         type3,         type4,         type5)

/** Implements a method with 5 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param m_type5      the data type of the 5. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  * @param v_type5      the data type of the internal variable of 5. parameter
  */
#define MOCKPP_VISITABLE_EXT5(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                         v_ret_type, x_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4, m_type5 param5) \
  MOCKPP_VISITABLE5_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                    v_ret_type, m_name,           v_type1, v_type2, v_type3, v_type4, v_type5)

/** Implements a const method with 5 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param m_type2      the data type of the 2. method parameter
  * @param m_type3      the data type of the 3. method parameter
  * @param m_type4      the data type of the 4. method parameter
  * @param m_type5      the data type of the 5. method parameter
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  * @param v_type2      the data type of the internal variable of 2. parameter
  * @param v_type3      the data type of the internal variable of 3. parameter
  * @param v_type4      the data type of the internal variable of 4. parameter
  * @param v_type5      the data type of the internal variable of 5. parameter
  */
#define MOCKPP_CONST_VISITABLE_EXT5(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                               v_ret_type, x_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4, m_type5 param5) const \
  MOCKPP_VISITABLE5_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                    v_ret_type, m_name,           v_type1, v_type2, v_type3, v_type4, v_type5)

//////////////////////////////////////////////////////////////
//

/** Implements the initializers for the internal variables of a
  * void method with 0 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(name) \
    MOCKPP_CONSTRUCT_BASE_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with no parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT0(m_name, x_name)\
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * void method with 1 parameter.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE1(name) \
    MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(name), \
    MOCKPP_CONSTRUCT_PARAMETER1_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with 1 parameter.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT1(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE1(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * void method with 2 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE2(name) \
    MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(name), \
    MOCKPP_CONSTRUCT_PARAMETER2_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with 2 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT2(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE2(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * void method with 3 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE3(name) \
    MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(name), \
    MOCKPP_CONSTRUCT_PARAMETER3_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with 3 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT3(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE3(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * void method with 4 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE4(name) \
    MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(name), \
    MOCKPP_CONSTRUCT_PARAMETER4_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with 4 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT4(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE4(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * void method with 5 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE5(name) \
    MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(name), \
    MOCKPP_CONSTRUCT_PARAMETER5_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with 5 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE5(m_name ## x_name)

//////////////////////////////////////////////////////////////
//

/** Implements the initializers for the internal variables of a
  * method with 0 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(name) \
    MOCKPP_CONSTRUCT_BASE_VISITABLE_MEMBERS(name), \
    MOCKPP_CONSTRUCT_RETURN_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with no parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT0(m_name, x_name)\
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * method with 1 parameter.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE1(name) \
    MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(name), \
    MOCKPP_CONSTRUCT_PARAMETER1_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with 1 parameter.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT1(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE1(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * method with 2 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE2(name) \
    MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(name), \
    MOCKPP_CONSTRUCT_PARAMETER2_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with 2 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT2(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE2(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * method with 3 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE3(name) \
    MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(name), \
    MOCKPP_CONSTRUCT_PARAMETER3_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with 3 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT3(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE3(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * method with 4 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE4(name) \
    MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(name), \
    MOCKPP_CONSTRUCT_PARAMETER4_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with 4 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT4(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE4(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * method with 5 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE5(name) \
    MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(name), \
    MOCKPP_CONSTRUCT_PARAMETER5_VISITABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with 5 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE5(m_name ## x_name)


#endif // MOCKPP_VISITABLEMOCKOBJECT_H
