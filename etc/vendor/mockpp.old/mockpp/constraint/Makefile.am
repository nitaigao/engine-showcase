INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

noinst_LTLIBRARIES = libconstraint.la

libconstraintinclude_HEADERS = IsAnything.h IsCloseTo.h IsEqual.h \
				IsGreaterThan.h 	IsInstanceOf.h IsLessThan.h IsNot.h IsNothing.h IsSame.h Or.h \
				StringContains.h StringStartsWith.h StringEndsWith.h And.h IsGreaterOrEqual.h \
	IsLessOrEqual.h Constraint.h ConstraintSet.h ConstraintSetN.h OutBound.h \
		TypelessConstraint.h ConstraintHolder.h ConstraintList.h TriggeredConstraint.h \
	TriggeredOutbound.h

EXTRA_DIST = \
   gen_constraintset_N.pl

CLEANFILES = *.~* *.~~* *~ *.old

libconstraintincludedir = $(includedir)/mockpp/constraint

libconstraint_la_SOURCES = IsAnything.cpp IsNothing.cpp ConstraintSet.cpp TypelessConstraint.cpp
