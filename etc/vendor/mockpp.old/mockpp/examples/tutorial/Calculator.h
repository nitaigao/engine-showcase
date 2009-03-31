/***************************************************************************
           Calculator.h  -  simple class for demonstration purposes

                             -------------------
    begin                : Sat 22 Jan 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: Calculator.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/


static int global_error = 0; // disturb tests

/** An exception class.
  * @internal
  */
class NetworkError {};

/** A simple class to test.
  * The class is constructed with a value which
  * serves as a base for different calculations.
  */
class Calculator
{
  public:

    /** Construct the object
      * @param in_base  the base value for all calculations
      */
    Calculator (int in_base)
      : base(in_base)
    {}

    /** Add a value
      * @param num   value to add
      * @result  result of base and input
      */
    int add(int num)
    {
      return base + num + global_error;
    }

    /** Subtract a value
      * @param num   value to subtract
      * @result  result of base and input
      */
    int sub(int num)
    {
      return base - num - global_error;
    }

 private:

   int base;
};

