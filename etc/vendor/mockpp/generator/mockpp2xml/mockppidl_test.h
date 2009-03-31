/* This is a comment */

#ifndef MOCKPP_IDL_TEST_H
#define MOCKPP_IDL_TEST_H

#include <string>

#define mockpp_constructor
#define mockpp_methods
#define K_DCOP
#define Q_OBJECT
#define k_dcop public
#define k_dcop_hidden public
#define k_dcop_signals public
#define slots
#define signals


class IdlInterface
{
    K_DCOP
  public:
    IdlInterface(const std::string &s)
    {}

    virtual ~IdlInterface()
    {}
};


class MockppIdlTest : public IdlInterface
{
    Q_OBJECT

public mockpp_constructor:
    MockppIdlTest()
      : IdlInterface("name")
    {
    }

public mockpp_constructor:
    MockppIdlTest(int i, const std::string &s)
      : IdlInterface("name")
    {
    }

    MockppIdlTest(int i, const std::string &s, long last)
      : IdlInterface("name")
    {
    }

public mockpp_methods:
    ~MockppIdlTest()
     {}

k_dcop:
k_dcop_signals:

private slots:
private signals:

private mockpp_methods:
    virtual std::string * first_url4() = 0;
    virtual std::string * first_url4(int i) = 0;
    virtual std::string * first_url4(long li = 2.0) = 0;
//mockpp_methods:
    virtual const void  * first_url4(float li = 1.0) = 0;
    virtual void * const first_url4(double li) = 0;

protected mockpp_methods:
    virtual std::string constTest() const = 0;
//mockpp_methods:
    virtual std::string & constRefTest() const = 0;

public:
    virtual void* *not_firstViewConst() const = 0;
    virtual void* *not_firstView() = 0;
    virtual void* not_nextView() = 0;

public mockpp_methods:
    virtual long** firstViewConst() const = 0;
    virtual void* firstView() = 0;
// mockpp_methods:
    virtual void* nextView1(int i, long l, std::string &s, const bool * b, char c) const = 0;
    virtual void* nextView2(int i, long l, std::string &s, bool * const b, char c) const = 0;

    void  getNumObjects(std::string s);
    const void* getObject(const std::string &s = std::string("iuuiu", 1));
    int   getNumPages(const std::wstring &s = L"asdf") throw(int, long);

    virtual const std::string * last_url4() = 0;
    virtual std::string ***** last_url4(int i) = 0;
    virtual std::string * last_url4(long li) throw(int, float) = 0;
};

typedef double Scalar;
inline static Scalar rad2deg( Scalar angle ) { return angle * (180./ 3.1415); }

#endif // end of line comment


