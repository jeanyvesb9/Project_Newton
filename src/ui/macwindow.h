#ifndef MACWINDOW_H
#define MACWINDOW_H

#ifdef MACX

class CocoaInitializer
{
public:
    CocoaInitializer();
    ~CocoaInitializer();
private:
    class Private;
    Private *p;
};

class MacWindow
{
public:
    static void initObjC_GC();
    static void setWindowStyle(long win_id);
};

#endif

#endif // MACWINDOW_H
