#!/bin/sh

p=$1

if test $# -eq 0; then
  p=6;
fi

if test $p -lt 6; then
  p=6;
fi

echo parameter: $p

( cd mockpp/chaining   ; ./gen_consecutive_N.pl           10 )

( cd mockpp/chaining   ; ./gen_invocation_N.pl            $p )
( cd mockpp/chaining   ; ./gen_chainablemethod_N.pl       $p )
( cd mockpp/chaining   ; ./gen_countchainable_N.pl        $p )
( cd mockpp/chaining   ; ./gen_returntrigger_N.pl         $p )
( cd mockpp/visiting   ; ./gen_responsevector_N.pl        $p )
( cd mockpp/visiting   ; ./gen_visitablemethod_N.pl       $p )
( cd mockpp/visiting   ; ./gen_countvisitable_N.pl        $p )
( cd mockpp/builder    ; ./gen_argumentsmatchbuilder_N.pl $p )
( cd mockpp/constraint ; ./gen_constraintset_N.pl         $p )
( cd mockpp/stub       ; ./gen_returntriggerstub_N.pl     $p )
( cd mockpp/stub       ; ./gen_triggerstub_N.pl           $p )
( cd mockpp            ; ./gen_countparams_N.pl           $p )

