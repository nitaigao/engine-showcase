#!/bin/sh

echo regression test

./xml2mockpp  --gen-visitable-method --file-name=$1  --dest-dir=$2/new-visit-meth/  --gen-class=MockppIdlTest

SRC="\".*/generator/mockpp2xml/"
sed -e s:"$SRC":\":g $2/new-visit-meth/MockppIdlTest_mock.h  >new-visit-MockppIdlTest_mock.h.tmp
sed -e s:"$SRC":\":g $2/orig-visit-meth/MockppIdlTest_mock.h >orig-visit-MockppIdlTest_mock.h.tmp
sed -e s:"$SRC":\":g $2/new-chain-meth/MockppIdlTest_mock.h  >new-chain-MockppIdlTest_mock.h.tmp
sed -e s:"$SRC":\":g $2/new-chain-meth/MockppIdlTest_mock.h  >orig-chain-MockppIdlTest_mock.h.tmp

# -----

echo diff new-visit-meth/MockppIdlTest_mock_user.cpp
diff $2/new-visit-meth/MockppIdlTest_mock_user.cpp $2/orig-visit-meth/MockppIdlTest_mock_user.cpp || return 1

echo diff new-visit-meth/MockppIdlTest_mock.cpp
diff $2/new-visit-meth/MockppIdlTest_mock.cpp      $2/orig-visit-meth/MockppIdlTest_mock.cpp      || return 1

echo diff new-visit-meth/MockppIdlTest_mock.h
diff new-visit-MockppIdlTest_mock.h.tmp        orig-visit-MockppIdlTest_mock.h.tmp                || return 1

# -----

./xml2mockpp  --gen-chainable-method --file-name=$1  --dest-dir=$2/new-chain-meth/  --gen-class=MockppIdlTest

echo diff new-chain-meth/MockppIdlTest_mock_user.cpp
diff $2/new-chain-meth/MockppIdlTest_mock_user.cpp $2/orig-chain-meth/MockppIdlTest_mock_user.cpp || return 1

echo diff new-chain-meth/MockppIdlTest_mock.cpp
diff $2/new-chain-meth/MockppIdlTest_mock.cpp      $2/orig-chain-meth/MockppIdlTest_mock.cpp      || return 1

echo diff new-chain-meth/MockppIdlTest_mock.h
diff new-chain-MockppIdlTest_mock.h.tmp        orig-chain-MockppIdlTest_mock.h.tmp                || return 1



