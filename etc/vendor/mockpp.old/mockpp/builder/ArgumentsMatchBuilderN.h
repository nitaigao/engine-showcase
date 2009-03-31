/** @file
    @internal NOT INTENDED FOR PUBLIC INCLUSION
    @brief    Generated with gen_argumentsmatchbuilder_N.pl.

  $Id: ArgumentsMatchBuilderN.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sun Aug 22 2004
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

#ifndef MOCKPP_ARGUMENTSMATCHBUILDER_N_H
#define MOCKPP_ARGUMENTSMATCHBUILDER_N_H

//#include <mockpp/builder/ArgumentsMatchBuilder.h>


MOCKPP_NS_START


/** Build a matcher for an invocation with 1 argument.
  * @ingroup grp_chainer
  * @internal
  */
template <typename R,    // Returntype
          typename I>    // Invocation
class ArgumentsMatchBuilder1 : public MatchBuilder<R,I>
{
  public:

    typedef R ReturnType;           //!< internal shared member
    typedef I InvocationType;       //!< internal shared member
    typedef typename I::T1Type P1;  //!< internal shared member

    typedef ConstraintSet1<P1> ConstraintSetType;  //!< internal shorthand

  /** Adds another matcher.
    * @param matcher the matcher object
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& addMatcher( const MatcherHolder<I> &matcher ) = 0;

  /** Indicate a matcher for 1 arguments.
    * @ingroup grp_chainer
    * @param p1 pointer to the constraint for method parameter 1
    * @return the builder object
    */
    MatchBuilder<R, I>& with(  const ConstraintHolder<P1> &p1 )
    {
      return with_cs( ConstraintSetType( p1 ) );
    }

  protected:

  /** Indicate a matcher for a set of arguments.
    * @param constraintset   constraint set
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& with_cs( const ConstraintSetType &constraintset ) = 0;
};


/** Build a matcher for an invocation with 2 argument.
  * @ingroup grp_chainer
  * @internal
  */
template <typename R,    // Returntype
          typename I>    // Invocation
class ArgumentsMatchBuilder2 : public MatchBuilder<R,I>
{
  public:

    typedef R ReturnType;           //!< internal shared member
    typedef I InvocationType;       //!< internal shared member
    typedef typename I::T1Type P1;  //!< internal shared member
    typedef typename I::T2Type P2;  //!< internal shared member

    typedef ConstraintSet2<P1, P2> ConstraintSetType;  //!< internal shorthand

  /** Adds another matcher.
    * @param matcher the matcher object
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& addMatcher( const MatcherHolder<I> &matcher ) = 0;

  /** Indicate a matcher for 2 arguments.
    * @ingroup grp_chainer
    * @param p1 pointer to the constraint for method parameter 1
    * @param p2 pointer to the constraint for method parameter 2
    * @return the builder object
    */
    MatchBuilder<R, I>& with(  const ConstraintHolder<P1> &p1
                             , const ConstraintHolder<P2> &p2 )
    {
      return with_cs( ConstraintSetType( p1, p2 ) );
    }

  protected:

  /** Indicate a matcher for a set of arguments.
    * @param constraintset   constraint set
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& with_cs( const ConstraintSetType &constraintset ) = 0;
};


/** Build a matcher for an invocation with 3 argument.
  * @ingroup grp_chainer
  * @internal
  */
template <typename R,    // Returntype
          typename I>    // Invocation
class ArgumentsMatchBuilder3 : public MatchBuilder<R,I>
{
  public:

    typedef R ReturnType;           //!< internal shared member
    typedef I InvocationType;       //!< internal shared member
    typedef typename I::T1Type P1;  //!< internal shared member
    typedef typename I::T2Type P2;  //!< internal shared member
    typedef typename I::T3Type P3;  //!< internal shared member

    typedef ConstraintSet3<P1, P2, P3> ConstraintSetType;  //!< internal shorthand

  /** Adds another matcher.
    * @param matcher the matcher object
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& addMatcher( const MatcherHolder<I> &matcher ) = 0;

  /** Indicate a matcher for 3 arguments.
    * @ingroup grp_chainer
    * @param p1 pointer to the constraint for method parameter 1
    * @param p2 pointer to the constraint for method parameter 2
    * @param p3 pointer to the constraint for method parameter 3
    * @return the builder object
    */
    MatchBuilder<R, I>& with(  const ConstraintHolder<P1> &p1
                             , const ConstraintHolder<P2> &p2
                             , const ConstraintHolder<P3> &p3 )
    {
      return with_cs( ConstraintSetType( p1, p2, p3 ) );
    }

  protected:

  /** Indicate a matcher for a set of arguments.
    * @param constraintset   constraint set
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& with_cs( const ConstraintSetType &constraintset ) = 0;
};


/** Build a matcher for an invocation with 4 argument.
  * @ingroup grp_chainer
  * @internal
  */
template <typename R,    // Returntype
          typename I>    // Invocation
class ArgumentsMatchBuilder4 : public MatchBuilder<R,I>
{
  public:

    typedef R ReturnType;           //!< internal shared member
    typedef I InvocationType;       //!< internal shared member
    typedef typename I::T1Type P1;  //!< internal shared member
    typedef typename I::T2Type P2;  //!< internal shared member
    typedef typename I::T3Type P3;  //!< internal shared member
    typedef typename I::T4Type P4;  //!< internal shared member

    typedef ConstraintSet4<P1, P2, P3, P4> ConstraintSetType;  //!< internal shorthand

  /** Adds another matcher.
    * @param matcher the matcher object
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& addMatcher( const MatcherHolder<I> &matcher ) = 0;

  /** Indicate a matcher for 4 arguments.
    * @ingroup grp_chainer
    * @param p1 pointer to the constraint for method parameter 1
    * @param p2 pointer to the constraint for method parameter 2
    * @param p3 pointer to the constraint for method parameter 3
    * @param p4 pointer to the constraint for method parameter 4
    * @return the builder object
    */
    MatchBuilder<R, I>& with(  const ConstraintHolder<P1> &p1
                             , const ConstraintHolder<P2> &p2
                             , const ConstraintHolder<P3> &p3
                             , const ConstraintHolder<P4> &p4 )
    {
      return with_cs( ConstraintSetType( p1, p2, p3, p4 ) );
    }

  protected:

  /** Indicate a matcher for a set of arguments.
    * @param constraintset   constraint set
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& with_cs( const ConstraintSetType &constraintset ) = 0;
};


/** Build a matcher for an invocation with 5 argument.
  * @ingroup grp_chainer
  * @internal
  */
template <typename R,    // Returntype
          typename I>    // Invocation
class ArgumentsMatchBuilder5 : public MatchBuilder<R,I>
{
  public:

    typedef R ReturnType;           //!< internal shared member
    typedef I InvocationType;       //!< internal shared member
    typedef typename I::T1Type P1;  //!< internal shared member
    typedef typename I::T2Type P2;  //!< internal shared member
    typedef typename I::T3Type P3;  //!< internal shared member
    typedef typename I::T4Type P4;  //!< internal shared member
    typedef typename I::T5Type P5;  //!< internal shared member

    typedef ConstraintSet5<P1, P2, P3, P4, P5> ConstraintSetType;  //!< internal shorthand

  /** Adds another matcher.
    * @param matcher the matcher object
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& addMatcher( const MatcherHolder<I> &matcher ) = 0;

  /** Indicate a matcher for 5 arguments.
    * @ingroup grp_chainer
    * @param p1 pointer to the constraint for method parameter 1
    * @param p2 pointer to the constraint for method parameter 2
    * @param p3 pointer to the constraint for method parameter 3
    * @param p4 pointer to the constraint for method parameter 4
    * @param p5 pointer to the constraint for method parameter 5
    * @return the builder object
    */
    MatchBuilder<R, I>& with(  const ConstraintHolder<P1> &p1
                             , const ConstraintHolder<P2> &p2
                             , const ConstraintHolder<P3> &p3
                             , const ConstraintHolder<P4> &p4
                             , const ConstraintHolder<P5> &p5 )
    {
      return with_cs( ConstraintSetType( p1, p2, p3, p4, p5 ) );
    }

  protected:

  /** Indicate a matcher for a set of arguments.
    * @param constraintset   constraint set
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& with_cs( const ConstraintSetType &constraintset ) = 0;
};


/** Build a matcher for an invocation with 6 argument.
  * @ingroup grp_chainer
  * @internal
  */
template <typename R,    // Returntype
          typename I>    // Invocation
class ArgumentsMatchBuilder6 : public MatchBuilder<R,I>
{
  public:

    typedef R ReturnType;           //!< internal shared member
    typedef I InvocationType;       //!< internal shared member
    typedef typename I::T1Type P1;  //!< internal shared member
    typedef typename I::T2Type P2;  //!< internal shared member
    typedef typename I::T3Type P3;  //!< internal shared member
    typedef typename I::T4Type P4;  //!< internal shared member
    typedef typename I::T5Type P5;  //!< internal shared member
    typedef typename I::T6Type P6;  //!< internal shared member

    typedef ConstraintSet6<P1, P2, P3, P4, P5, P6> ConstraintSetType;  //!< internal shorthand

  /** Adds another matcher.
    * @param matcher the matcher object
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& addMatcher( const MatcherHolder<I> &matcher ) = 0;

  /** Indicate a matcher for 6 arguments.
    * @ingroup grp_chainer
    * @param p1 pointer to the constraint for method parameter 1
    * @param p2 pointer to the constraint for method parameter 2
    * @param p3 pointer to the constraint for method parameter 3
    * @param p4 pointer to the constraint for method parameter 4
    * @param p5 pointer to the constraint for method parameter 5
    * @param p6 pointer to the constraint for method parameter 6
    * @return the builder object
    */
    MatchBuilder<R, I>& with(  const ConstraintHolder<P1> &p1
                             , const ConstraintHolder<P2> &p2
                             , const ConstraintHolder<P3> &p3
                             , const ConstraintHolder<P4> &p4
                             , const ConstraintHolder<P5> &p5
                             , const ConstraintHolder<P6> &p6 )
    {
      return with_cs( ConstraintSetType( p1, p2, p3, p4, p5, p6 ) );
    }

  protected:

  /** Indicate a matcher for a set of arguments.
    * @param constraintset   constraint set
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& with_cs( const ConstraintSetType &constraintset ) = 0;
};



MOCKPP_NS_END


#endif // MOCKPP_ARGUMENTSMATCHBUILDER_N_H

