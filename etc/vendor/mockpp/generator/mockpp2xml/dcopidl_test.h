/* This is a comment */

#ifndef TEST_H
#define TEST_H

// some comment
#include <dcopobject.h>
#include <dcopref.h>

/* We never use it, but it's a test after all :)
 */
class Test2;

// this has to work too
#include <qstring.h>

typedef QMap<KIO::Job *, KIODownload> KIODownloadMap;
class Test3;

class Test : public MyNamespace::MyParentClass, virtual public DCOPObject,
             public QValueList<QString>
{
    K_DCOP

private mockpp_methods:
    struct Bar {
      int i;
    };
    class SomeForwardFooPrivate;
public mockpp_constructor:
    Test();
    ~Test();
private mockpp_methods:
    int getPageOfObj( int obj );
    void setCell( KSpreadTable *table,
		  const QPoint& point );

    SomeForwardFooPrivate *d;
    static const int AUDIO_CD = 1;
    static const int AUDIO_CD = 1 << 6;
k_dcop:
    virtual QString url();
    virtual DCOPRef firstView();
    virtual DCOPRef nextView();
    virtual int getNumObjects();
    virtual DCOPRef getObject( int num );
    virtual int getNumPages();
    virtual DCOPRef getPage( int num );
    int getPageOfObj( int obj );
    void setCell( const int& point = 3 );
    QValueList<DCOPRef> getWindows();

k_dcop_signals:
    void blah_signal();

private:
public mockpp_constructor:
    Test3 *doc();

    struct KIODownload;
    typedef QMap<KIO::Job *, KIODownload> KIODownloadMap;
};

class Test2 : public DCOPObject
{
    K_DCOP
private mockpp_methods:
k_dcop_signals:
    void fooSignal(int arg);
};

class Test3 : public DCOPObject
{
    K_DCOP
private mockpp_methods:
    void yadda();
};

#endif // end

/* Test for line numbers */


