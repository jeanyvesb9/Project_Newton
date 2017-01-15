#ifndef MACWINDOW_H
#define MACWINDOW_H

class AutoReleasePool
{
public:
    AutoReleasePool();
    ~AutoReleasePool();
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
