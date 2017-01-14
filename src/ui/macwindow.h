#ifndef MACWINDOW_H
#define MACWINDOW_H

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
    static void setWindowStyle(long win_id);
};

#endif // MACWINDOW_H
