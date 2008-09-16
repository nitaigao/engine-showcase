/***************************************************************************
          consumer.cpp  -  configuration consumer class

                             -------------------
    begin                : Sun 2 Jan 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: consumer.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

#include "interface.h"
#include "consumer.h"

#include <mockpp/mockpp_dbc.h>

Consumer::Consumer(Interface *intf)
  : configfile(intf)
{
  MOCKPP_PRE(intf != 0);
}

void Consumer::load()
{
  configfile->open("file1.lst");
  config1 = configfile->read();
  config2 = configfile->read();
  config3 = configfile->read();
  configfile->close();
}

void Consumer::process()
{
  unsigned x = configfile->calculate(1);
  x += configfile->calculate(2);
  x =+ configfile->calculate(3);

  config1 += "/processed";
  config2 += "/processed";
  config3 += "/processed";
}

void Consumer::save()
{
  configfile->open("file1.lst");
  configfile->write(config1);
  configfile->write(config2);
  configfile->write(config3);
  configfile->close();
}
