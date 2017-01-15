#include "src/ui/macwindow.h"

#include <Cocoa/Cocoa.h>
#include <Appkit/AppKit.h>

class AutoReleasePool::Private
{
public:
    NSAutoreleasePool *autoReleasePool;
};

AutoReleasePool::AutoReleasePool()
{
    p = new AutoReleasePool::Private();
    NSApplicationLoad();
    p->autoReleasePool = [[NSAutoreleasePool alloc] init];
}

AutoReleasePool::~AutoReleasePool()
{
    [p->autoReleasePool release];
    delete p;
}

//---------------------------------------------------------------------------------------------------------------------

void MacWindow::setWindowStyle(long win_id)
{
    NSView *nativeView = reinterpret_cast<NSView *>(win_id);
    NSWindow *nativeWindow = [nativeView window];

    [nativeWindow setStyleMask: [nativeWindow styleMask] | NSFullSizeContentViewWindowMask | NSWindowTitleHidden];
    [nativeWindow setTitlebarAppearsTransparent: YES];
    [nativeWindow setMovableByWindowBackground: YES];
}
