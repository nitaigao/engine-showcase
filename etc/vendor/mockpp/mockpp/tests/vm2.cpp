#define MOCKPP_IMPORT_ABBREVIATED
#include <mockpp/mockpp.h> // always first

#include <mockpp/visiting/VisitableMockObject.h>
#include <mockpp/visiting/CountedVisitableMethod.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>


class VisitMock : public MOCKPP_NS::VisitableMockObject {
public:

	VisitMock():
		MOCKPP_NS::VisitableMockObject(MOCKPP_PCHAR("VisitMock"), 0),
		foo_mocker(MOCKPP_PCHAR("foo"), this)
	{
	}

	void foo(int& x)
	{
		return foo_mocker.forward(x);
	}

	void foo(const MOCKPP_NS::ConstraintHolder<int> &ch)
	{
		foo_mocker.forward(ch);
	}

	MOCKPP_NS::VisitableMockMethod<void, int> foo_mocker;
};


int main(int /*argc*/, char ** /*argv*/)
{
  printf ("1\n");
	VisitMock mock;

	mock.foo(logic_and<int>( eq<int>(0)
	                       , outBound<int>(42)
                         ));
  printf ("2\n");

	mock.activate();

	int val = 0;
  mock.foo(val);
  printf ("3\n");

  MOCKPP_ASSERT_EQUALS(42, val);
  printf ("4\n");

 	mock.verify();
  printf ("5\n");
}
