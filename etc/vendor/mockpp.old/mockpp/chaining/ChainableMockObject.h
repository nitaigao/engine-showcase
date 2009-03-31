/** @file
    @brief MockObjects that can be chained under control.

  $Id: ChainableMockObject.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_CHAINABLEMOCKOBJECT_H
#define MOCKPP_CHAINABLEMOCKOBJECT_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/MockObject.h>

#include <mockpp/chaining/ChainableMockObject_macro.h>
#include <mockpp/chaining/Invocation.h>
#include <mockpp/chaining/CoreMock.h>
#include <mockpp/chaining/ChainingMockBuilder.h>

#include <mockpp/builder/ArgumentsMatchBuilder.h>
#include <mockpp/builder/InvocationMockerBuilder.h>
#include <mockpp/builder/BuilderNamespace.h>

#include <mockpp/matcher/TypelessMatcher.h>


MOCKPP_NS_START


/** Base for a generic object that can be used to replace a real world object
  * for testing purposes.
  * It emulates the real world behaviour by feeding it "chained expectations".
  * \ingroup grp_advanced_mo
  */
class MOCKPP_API_DECL0 ChainableMockObjectBase : public BuilderNamespace
{
    typedef MOCKPP_STL::map<String, MatchBuilderAdapterBase*>  TableType;
    typedef MOCKPP_STL::multimap<String, InvokedRecorder*>     PendingType;

  public:

  /** Construct the item.
    * @param name    human readable description about the object
    */
    ChainableMockObjectBase(const String &name);

  /** Destruct the item.
    */
    virtual ~ChainableMockObjectBase();

  /** Searches a name.
    * @internal
    * @param id   item's name
    * @return   if name was found: pointer to matcher
    * @return   if name was not found: 0
    */
    MatchBuilderAdapterBase *lookupID( const String &id ) const;

  /** Registers another match builder under its unique identifier.
    * @internal
    * @param   id       builder's identifier
    * @param   builder  pointer to builder object
    */
    void registerUniqueID( const String &id, MatchBuilderAdapterBase *builder );

  /** Stores a builder object.
    * @internal
    * Originally the builder objects were temporary objects. But due to the fact
    * that they may be needed for subsequent calls of after() or before() they must
    * remain accessible until the last expectation is set. For simplicity they
    * are destroyed with the mock object.
    * @param   builder  pointer to builder object
    */
    virtual void storeBuilder( AutoPointer<MatchBuilderAdapterBase> builder );

  /** Stores another recorder object for later use.
    * @internal
    * Pending recorders occur when an after() or before() call occurs before the
    * identifier is actually set. It is later resolved when the identifier is set.
    * @param   id        recorder's identifier
    * @param   recorder  pointer to recorder object
    */
    virtual void addPendingRecorder(const String &id, InvokedRecorder::AP recorder);

  /** Checks if there are any pending recorders left. If there are, an exception is
    * throw.
    * @internal
    */
    void checkPendingRecorders() const; // @todo make private

  protected:

  /** Verifies the object and the mock objects it contains.
    * If it fails, an AssertionFailedError is thrown
    */
    void chainableVerify();

  /** Frees all internal data.
    * @internal
    */
    virtual void chainableReset();

  private:

  /** Stores another match builder under its unique identifier.
    * @internal
    * @param   id       builder's identifier
    * @param   builder  pointer to builder object
    */
    void storeID( const String &id, MatchBuilderAdapterBase *builder );

    ChainableMockObjectBase (const ChainableMockObjectBase &);    // forbid
    ChainableMockObjectBase& operator=(ChainableMockObjectBase&);

    MOCKPP_STL::vector<MatchBuilderAdapterBase*>  allBuilders;
    TableType                              idTable;
    mutable PendingType                    pendingRecorders;
};


/** A generic object that can be used to replace a real world object for testing purposes.
  * It emulates the real world behaviour by feeding it "chained expectations".
  * \ingroup grp_advanced_mo
  */
class MOCKPP_API_DECL0 ChainableMockObject : public MockObject
                                           , public ChainableMockObjectBase
{
  public:

  /** Construct the item.
    * @param name    human readable description about the object
    * @param parent  parent verifiable
    */
    ChainableMockObject(const String &name, VerifiableList *parent = 0);

  /** Destruct the item.
    */
    virtual ~ChainableMockObject();

  /** Verifies the object and the mock objects it contains.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify();

  /** Frees all internal data.
    * @internal
    */
    virtual void reset();
};


MOCKPP_NS_END


//////////////////////////////////////////////////////////////
//

/**
  * @defgroup grp_chainer Chaining behaviour of a Chainable Mock Object
  */

/** Implements a chainer for a method.
  * Serves also as a backwards compatibility macro.
  * @ingroup grp_chainer
  * @param clsname     the chainable mock object class name
  * @param methname    the method for which you create the chainer
  */
#define MOCKPP_CHAINER_FOR(clsname, methname) \
    clsname::ChainerFor ## methname

/** Implements a chainer for a method.
  * @ingroup grp_chainer
  * @param clsname     the chainable mock object class name
  * @param m_methname  the method for which you create the chainer
  * @param x_methname  optional extension for the internal names based on the method name
  */
#define MOCKPP_CHAINER_FOR_EXT(clsname, m_methname, x_methname) \
  MOCKPP_CHAINER_FOR(clsname, m_methname ## x_methname)

//////////////////////////////////////////////////////////////
//

/** Implements a method with 0 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  */
#define MOCKPP_VOID_CHAINABLE0(classname, name) \
  public: \
    void name() \
  MOCKPP_VOID_CHAINABLE0_IMPL(classname, name,  \
                                         name)

/** Implements a const method with 0 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  */
#define MOCKPP_VOID_CONST_CHAINABLE0(classname, name) \
  public: \
    void name() const \
  MOCKPP_VOID_CHAINABLE0_IMPL(classname, name, \
                                         name)

/** Implements a method with 0 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param x_name       optional extension for the internal variable names based on the method name
  */
#define MOCKPP_VOID_CHAINABLE_EXT0(classname, m_name, \
                                              x_name) \
  public: \
    void m_name() \
  MOCKPP_VOID_CHAINABLE0_IMPL(classname, m_name ## x_name, \
                                         m_name)

/** Implements a method with 0 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param x_name       optional extension for the internal variable names based on the method name
  */
#define MOCKPP_VOID_CONST_CHAINABLE_EXT0(classname, m_name,  \
                                                    x_name) \
  public: \
    void m_name() const \
  MOCKPP_VOID_CHAINABLE0_IMPL(classname, m_name ## x_name,  \
                                         m_name)

//////////////////////////////////////////////////////////////


/** Implements a method with 1 parameter for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  */
#define MOCKPP_VOID_CHAINABLE1(classname, name, type1) \
  public: \
    void name(const type1 &param1) \
  MOCKPP_VOID_CHAINABLE1_IMPL(classname, name , const type1 &, \
                                         name,        type1)

/** Implements a const method with 1 parameter for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  */
#define MOCKPP_VOID_CONST_CHAINABLE1(classname, name, type1) \
  public: \
    void name(const type1 &param1) const \
  MOCKPP_VOID_CHAINABLE1_IMPL(classname, name, const type1 &, \
                                         name,       type1)

/** Implements a method with 1 parameter for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_name       the method name
  * @param m_type1      the data type of the 1. method parameter
  * @param x_name       optional extension for the internal variable names based on the method name
  * @param v_type1      the data type of the internal variable of 1. parameter
  */
#define MOCKPP_VOID_CHAINABLE_EXT1(classname, m_name, m_type1, \
                                              x_name, v_type1) \
  public: \
    void m_name(m_type1 param1) \
  MOCKPP_VOID_CHAINABLE1_IMPL(classname, m_name ## x_name, m_type1, \
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
#define MOCKPP_VOID_CONST_CHAINABLE_EXT1(classname, m_name, m_type1, \
                                                    x_name, v_type1) \
  public: \
    void m_name(m_type1 param1) const \
  MOCKPP_VOID_CHAINABLE1_IMPL(classname, m_name ## x_name, m_type1, \
                                         m_name,           v_type1)

////////////////////////////////////////////////////////////////////


/** Implements a method with 2 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  */
#define MOCKPP_VOID_CHAINABLE2(classname, name, type1, type2) \
  public: \
    void name(const type1 &param1, const type2 &param2) \
  MOCKPP_VOID_CHAINABLE2_IMPL(classname, name, const type1 &, const type2 &, \
                                         name,       type1,   type2)

/** Implements a const method with 2 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  */
#define MOCKPP_VOID_CONST_CHAINABLE2(classname, name, type1, type2) \
  public: \
    void name(const type1 &param1, const type2 &param2) const \
  MOCKPP_VOID_CHAINABLE2_IMPL(classname, name, const type1&, const type2&, \
                                         name,       type1,        type2)

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
#define MOCKPP_VOID_CHAINABLE_EXT2(classname, m_name, m_type1, m_type2, \
                                              x_name, v_type1, v_type2) \
  public: \
    void m_name(m_type1 param1, m_type2 param2) \
  MOCKPP_VOID_CHAINABLE2_IMPL(classname, m_name ## x_name, m_type1, m_type2, \
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
#define MOCKPP_VOID_CONST_CHAINABLE_EXT2(classname, m_name, m_type1, m_type2, \
                                                    x_name, v_type1, v_type2) \
  public: \
    void m_name(m_type1 param1, m_type2 param2) const \
  MOCKPP_VOID_CHAINABLE2_IMPL(classname, m_name ## x_name, m_type1, m_type2, \
                                         m_name,           v_type1, v_type2)


////////////////////////////////////////////////////////////////////


/** Implements a method with 3 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  */
#define MOCKPP_VOID_CHAINABLE3(classname, name, type1, type2, type3) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3) \
  MOCKPP_VOID_CHAINABLE3_IMPL(classname, name, const type1 &, const type2 &, const type3 &, \
                                         name,       type1,         type2,         type3)

/** Implements a const method with 3 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  */
#define MOCKPP_VOID_CONST_CHAINABLE3(classname, name, type1, type2, type3) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3) const \
  MOCKPP_VOID_CHAINABLE3_IMPL(classname, name, const type1 &, const type2 &, const type3 &, \
                                         name,       type1,   type2,         type3)

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
#define MOCKPP_VOID_CHAINABLE_EXT3(classname, m_name, m_type1, m_type2, m_type3, \
                                              x_name, v_type1, v_type2, v_type3) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3) \
  MOCKPP_VOID_CHAINABLE3_IMPL(classname, m_name ## x_name, m_type1, m_type2, m_type3, \
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
#define MOCKPP_VOID_CONST_CHAINABLE_EXT3(classname, m_name, m_type1, m_type2, m_type3, \
                                                    x_name, v_type1, v_type2, v_type3) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3) const \
  MOCKPP_VOID_CHAINABLE3_IMPL(classname, m_name ## x_name, m_type1, m_type2, m_type3, \
                                         m_name,           v_type1, v_type2, v_type3)


////////////////////////////////////////////////////////////////////


/** Implements a method with 4 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  */
#define MOCKPP_VOID_CHAINABLE4(classname, name, type1, type2, type3, type4) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4) \
  MOCKPP_VOID_CHAINABLE4_IMPL(classname, name, const type1 &, const type2 &, const type3 &, const type4 &, \
                                         name,       type1,         type2,         type3,         type4)

/** Implements a const method with 4 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  */
#define MOCKPP_VOID_CONST_CHAINABLE4(classname, name, type1, type2, type3, type4) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4) const \
  MOCKPP_VOID_CHAINABLE4_IMPL(classname, name, const type1 &, const type2 &, const type3 &, const type4 &, \
                                         name,        type1,        type2,         type3,         type4)

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
#define MOCKPP_VOID_CHAINABLE_EXT4(classname, m_name, m_type1, m_type2, m_type3, m_type4, \
                                              x_name, v_type1, v_type2, v_type3, v_type4) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4) \
  MOCKPP_VOID_CHAINABLE4_IMPL(classname, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, \
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
#define MOCKPP_VOID_CONST_CHAINABLE_EXT4(classname, m_name, m_type1, m_type2, m_type3, m_type4, \
                                                    x_name, v_type1, v_type2, v_type3, v_type4) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4) const \
  MOCKPP_VOID_CHAINABLE4_IMPL(classname, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, \
                                         m_name,           v_type1, v_type2, v_type3, v_type4)


////////////////////////////////////////////////////////////////////


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
#define MOCKPP_VOID_CHAINABLE5(classname, name, type1, type2, type3, type4, type5) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4, const type5 &param5) \
  MOCKPP_VOID_CHAINABLE5_IMPL(classname, name, const type1 &, const type2 &, const type3 &, const type4 &, const type5 &, \
                                         name,       type1,         type2,         type3,         type4,         type5)

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
#define MOCKPP_VOID_CONST_CHAINABLE5(classname, name, type1, type2, type3, type4, type5) \
  public: \
    void name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4, const type5 &param5) const \
  MOCKPP_VOID_CHAINABLE5_IMPL(classname, name, const type1 &, const type2 &, const type3 &, const type4 &, const type5 &, \
                                         name,       type1,         type2,         type3,         type4,         type5)

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
#define MOCKPP_VOID_CHAINABLE_EXT5(classname, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                              x_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4, m_type5 param5) \
  MOCKPP_VOID_CHAINABLE5_IMPL(classname, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                         m_name,           v_type1, v_type2, v_type3, v_type4, v_type5)

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
#define MOCKPP_VOID_CONST_CHAINABLE_EXT5(classname, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                                    x_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
  public: \
    void m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4, m_type5 param5) const \
  MOCKPP_VOID_CHAINABLE5_IMPL(classname, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                         m_name,           v_type1, v_type2, v_type3, v_type4, v_type5)

//////////////////////////////////////////////////////////////
//

/** Implements a method with 0 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  */
#define MOCKPP_CHAINABLE0(classname, ret_type, name) \
  public: \
    ret_type name() \
  MOCKPP_CHAINABLE0_IMPL(classname, ret_type, name,  \
                                    ret_type, name)

/** Implements a const method with 0 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  */
#define MOCKPP_CONST_CHAINABLE0(classname, ret_type, name) \
  public: \
    ret_type name() const \
  MOCKPP_CHAINABLE0_IMPL(classname, ret_type, name,  \
                                    ret_type, name)

/** Implements a method with 0 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  */
#define MOCKPP_CHAINABLE_EXT0(classname, m_ret_type, m_name,  \
                                         v_ret_type, x_name) \
  public: \
    m_ret_type m_name() \
  MOCKPP_CHAINABLE0_IMPL(classname, m_ret_type, m_name ## x_name,  \
                                    v_ret_type, m_name)

/** Implements a method with 0 parameters for a mock object.
  * The macro distinguishes between the methods parameter and the types for
  * the internal variables.
  * @param classname    the name of the mock class to which the method belongs
  * @param m_ret_type   the data type of the return value
  * @param m_name       the method name
  * @param v_ret_type   the data type of the internal return value respesentation
  * @param x_name       optional extension for the internal variable names based on the method name
  */
#define MOCKPP_CONST_CHAINABLE_EXT0(classname, m_ret_type, m_name,  \
                                               v_ret_type, x_name) \
  public: \
    m_ret_type m_name() const \
  MOCKPP_CHAINABLE0_IMPL(classname, m_ret_type, m_name ## x_name,  \
                                    v_ret_type, m_name)

//////////////////////////////////////////////////////////////


/** Implements a method with 1 parameter for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type     the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  */
#define MOCKPP_CHAINABLE1(classname, ret_type, name, type1) \
  public: \
    ret_type name(const type1 &param1) \
  MOCKPP_CHAINABLE1_IMPL(classname, ret_type, name , const type1 &, \
                                    ret_type, name,        type1)

/** Implements a const method with 1 parameter for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  */
#define MOCKPP_CONST_CHAINABLE1(classname, ret_type, name, type1) \
  public: \
    ret_type name(const type1 &param1) const \
  MOCKPP_CHAINABLE1_IMPL(classname, ret_type, name, const type1 &, \
                                    ret_type, name,       type1)

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
#define MOCKPP_CHAINABLE_EXT1(classname, m_ret_type, m_name, m_type1, \
                                         v_ret_type, x_name, v_type1) \
  public: \
    m_ret_type m_name(m_type1 param1) \
  MOCKPP_CHAINABLE1_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, \
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
#define MOCKPP_CONST_CHAINABLE_EXT1(classname, m_ret_type, m_name, m_type1, \
                                               v_ret_type, x_name, v_type1) \
  public: \
    m_ret_type m_name(m_type1 param1) const \
  MOCKPP_CHAINABLE1_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, \
                                    v_ret_type, m_name,           v_type1)

////////////////////////////////////////////////////////////////////


/** Implements a method with 2 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  */
#define MOCKPP_CHAINABLE2(classname, ret_type, name, type1, type2) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2) \
  MOCKPP_CHAINABLE2_IMPL(classname, ret_type, name, const type1 &, const type2 &, \
                                    ret_type, name,       type1,         type2)

/** Implements a const method with 2 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type   the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  */
#define MOCKPP_CONST_CHAINABLE2(classname, ret_type, name, type1, type2) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2) const \
  MOCKPP_CHAINABLE2_IMPL(classname, ret_type, name,  const type1 &, const type2 &, \
                                    ret_type, name,        type1,         type2)

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
#define MOCKPP_CHAINABLE_EXT2(classname, m_ret_type, m_name, m_type1, m_type2, \
                                         v_ret_type, x_name, v_type1, v_type2) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2) \
  MOCKPP_CHAINABLE2_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, \
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
#define MOCKPP_CONST_CHAINABLE_EXT2(classname, m_ret_type, m_name, m_type1, m_type2, \
                                               v_ret_type, x_name, v_type1, v_type2) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2) const \
  MOCKPP_CHAINABLE2_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, \
                                    v_ret_type, m_name,           v_type1, v_type2)


////////////////////////////////////////////////////////////////////


/** Implements a method with 3 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type     the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  */
#define MOCKPP_CHAINABLE3(classname, ret_type, name, type1, type2, type3) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3) \
  MOCKPP_CHAINABLE3_IMPL(classname, ret_type, name, const type1 &, const type2 &, const type3 &, \
                                    ret_type, name,       type1,         type2,         type3)

/** Implements a const method with 3 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type     the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  */
#define MOCKPP_CONST_CHAINABLE3(classname, ret_type, name, type1, type2, type3) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3) const \
  MOCKPP_CHAINABLE3_IMPL(classname, ret_type, name, const type1 &, const type2 &, const type3 &, \
                                    ret_type, name,       type1,         type2,         type3)

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
#define MOCKPP_CHAINABLE_EXT3(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, \
                                         v_ret_type, x_name, v_type1, v_type2, v_type3) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3) \
  MOCKPP_CHAINABLE3_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, \
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
#define MOCKPP_CONST_CHAINABLE_EXT3(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, \
                                               v_ret_type, x_name, v_type1, v_type2, v_type3) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3) const \
  MOCKPP_CHAINABLE3_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, \
                                    v_ret_type, m_name,           v_type1, v_type2, v_type3)


////////////////////////////////////////////////////////////////////


/** Implements a method with 4 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type     the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  */
#define MOCKPP_CHAINABLE4(classname, ret_type, name, type1, type2, type3, type4) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4) \
  MOCKPP_CHAINABLE4_IMPL(classname, ret_type, name, const type1 &, const type2 &, const type3 &, const type4 &, \
                                    ret_type, name,       type1,         type2,         type3,         type4)

/** Implements a const method with 4 parameters for a mock object.
  * Serves also as a backwards compatibility macro.
  * @param classname  the name of the mock class to which the method belongs
  * @param ret_type     the data type of the return value
  * @param name       the method name
  * @param type1      the data type of the 1. parameter
  * @param type2      the data type of the 2. parameter
  * @param type3      the data type of the 3. parameter
  * @param type4      the data type of the 4. parameter
  */
#define MOCKPP_CONST_CHAINABLE4(classname, ret_type, name, type1, type2, type3, type4) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4) const \
  MOCKPP_CHAINABLE4_IMPL(classname, ret_type, name, const type1 &, const type2 &, const type3 &, const type4 &, \
                                    ret_type, name,       type1,         type2,         type3,         type4)

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
#define MOCKPP_CHAINABLE_EXT4(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, \
                                         v_ret_type, x_name, v_type1, v_type2, v_type3, v_type4) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4) \
  MOCKPP_CHAINABLE4_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, \
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
#define MOCKPP_CONST_CHAINABLE_EXT4(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, \
                                               v_ret_type, x_name, v_type1, v_type2, v_type3, v_type4) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4) const \
  MOCKPP_CHAINABLE4_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, \
                                    v_ret_type, m_name,           v_type1, v_type2, v_type3, v_type4)


////////////////////////////////////////////////////////////////////


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
#define MOCKPP_CHAINABLE5(classname, ret_type, name, type1, type2, type3, type4, type5) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4, const type5 &param5) \
  MOCKPP_CHAINABLE5_IMPL(classname, ret_type, name, const type1 &, const type2 &, const type3 &, const type4 &, const type5 &, \
                                    ret_type, name,       type1,         type2,         type3,         type4,         type5)

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
#define MOCKPP_CONST_CHAINABLE5(classname, ret_type, name, type1, type2, type3, type4, type5) \
  public: \
    ret_type name(const type1 &param1, const type2 &param2, const type3 &param3, const type4 &param4, const type5 &param5) const \
  MOCKPP_CHAINABLE5_IMPL(classname, ret_type, name, const type1 &, const type2 &, const type3 &, const type4 &, const type5 &, \
                                    ret_type, name,       type1,         type2,         type3,         type4,         type5)

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
#define MOCKPP_CHAINABLE_EXT5(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                         v_ret_type, x_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4, m_type5 param5) \
  MOCKPP_CHAINABLE5_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
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
#define MOCKPP_CONST_CHAINABLE_EXT5(classname, m_ret_type, m_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                               v_ret_type, x_name, v_type1, v_type2, v_type3, v_type4, v_type5) \
  public: \
    m_ret_type m_name(m_type1 param1, m_type2 param2, m_type3 param3, m_type4 param4, m_type5 param5) const \
  MOCKPP_CHAINABLE5_IMPL(classname, m_ret_type, m_name ## x_name, m_type1, m_type2, m_type3, m_type4, m_type5, \
                                    v_ret_type, m_name,           v_type1, v_type2, v_type3, v_type4, v_type5)


//////////////////////////////////////////////////////////////
//

/** Implements the initializers for the internal variables of a
  * void method with 0 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE0(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with no parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE_EXT0(m_name, x_name)\
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE0(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * void method with 1 parameter.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE1(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with 1 parameter.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE_EXT1(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE1(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * void method with 2 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE2(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with 2 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE_EXT2(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE2(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * void method with 3 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE3(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with 3 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE_EXT3(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE3(m_name ## x_name)

/** Implements the initializers for the internal variables of a
  * void method with 4 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE4(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with 4 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE_EXT4(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE4(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * void method with 5 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE5(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * void method with 5 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE_EXT5(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE5(m_name ## x_name)

//////////////////////////////////////////////////////////////
//

/** Implements the initializers for the internal variables of a
  * method with 0 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with no parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT0(m_name, x_name)\
  MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(m_name ## x_name)

/** Implements the initializers for the internal variables of a
  * method with 1 parameter.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE1(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with 1 parameter.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT1(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE1(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * method with 2 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE2(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with 5 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT2(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE2(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * method with 3 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE3(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with 4 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT3(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE3(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * method with 4 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE4(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with 4 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT4(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE4(m_name ## x_name)


/** Implements the initializers for the internal variables of a
  * method with 5 parameters.
  * @param name       the method name
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE5(name) \
  MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS(name)

/** Implements the initializers for the internal variables of a
  * method with 5 parameters.
  * @param m_name       the method name
  * @param x_name       extension for internal naming
  */
#define MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT5(m_name, x_name) \
  MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE5(m_name ## x_name)


#endif // MOCKPP_CHAINABLEMOCKOBJECT_H

