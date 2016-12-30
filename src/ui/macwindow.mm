#ifdef MACX

#include "src/ui/macwindow.h"

#include <Cocoa.h>
#include <AppKit.h>

class CocoaInitializer::Private
{
public:
    NSAutoreleasePool *autoReleasePool;
};

CocoaInitializer::CocoaInitializer()
{
    p = new CocoaInitializer::Private();
    NSApplicationLoad();
    p->autoReleasePool = [[NSAutoreleasePool alloc] init];
}

CocoaInitializer::~CocoaInitializer()
{
    [p->autoReleasePool release];
    delete p;
}

//--------------------------------------------------------------------------------------------------------------------

void MacWindow::initObjC_GC()
{

}

void MacWindow::setWindowStyle(long win_id)
{
    NSView *nativeView = reinterpret_cast<NSView *>(win_id);
    NSWindow *nativeWindow = [nativeView window];

    [nativeWindow setStyleMask: [nativeWindow styleMask] | NSFullSizeContentViewWindowMask | NSWindowTitleHidden];
    [nativeWindow setTitlebarAppearsTransparent: YES];
    [nativeWindow setMovableByWindowBackground: YES];
}

#endif
