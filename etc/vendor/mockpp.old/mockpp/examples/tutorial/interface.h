/***************************************************************************
          interface.h  -  interface class for file access

                             -------------------
    begin                : Sun 2 Jan 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: interface.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/

#include <mockpp/mockpp.h>

#include <string>

/** Interface class for a configuration file.
  * Each record consists of one line of ascii characters.
  * Each file contains exactly three lines.
  * Processed lines are written back with
  * "/processed" appended.
  */
class Interface
{
 public:

  /** Destroy the interface
    */
   virtual ~Interface()
   {}

  /** Opens the file
    * @param name  name of file to open
    */
    virtual void open(const MOCKPP_STL::string &name) = 0;

  /** Reads one record from the file
    * @return  the record (one complete line)
    */
    virtual MOCKPP_STL::string read() = 0;

  /** Writes a record to the file
    * @param  s   the record (one complete line)
    */
    virtual void write(const MOCKPP_STL::string &s) = 0;

  /** Does some calculating
    * @param  input ingoing data
    * @return outgoing result
    */
    virtual unsigned calculate(unsigned input) = 0;

  /** Closes the file
    */
    virtual void close() = 0;
};

