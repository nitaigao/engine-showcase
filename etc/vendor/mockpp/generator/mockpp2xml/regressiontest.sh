#!/bin/sh

./mockpp2xml  $1 >$2.new

SRC="SOURCE\>.*/generator/mockpp2xml/"
sed -e s:"$SRC":SOURCE\>./:g $2      >$2.tmp
sed -e s:"$SRC":SOURCE\>./:g $2.new  >$2.new.tmp

diff $2.tmp $2.new.tmp


