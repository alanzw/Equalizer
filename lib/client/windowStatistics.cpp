
/* Copyright (c) 2008, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#include "windowStatistics.h"

#include "window.h"
#include "global.h"

#ifdef WIN32_VC
#  define snprintf _snprintf
#endif

namespace eq
{

WindowStatistics::WindowStatistics( const Statistic::Type type, 
                                    Window* window )
{
    _event.window                     = window;

    const int32_t hint = window->getIAttribute( Window::IATTR_HINT_STATISTICS );
    if( hint == OFF )
        return;

    _event.data.type                  = Event::STATISTIC;
    _event.data.originator            = window->getID();
    _event.data.statistic.type        = type;
    _event.data.statistic.frameNumber = window->getPipe()->getCurrentFrame();

    const std::string& name = window->getName();
    if( name.empty( ))
        snprintf( _event.data.statistic.resourceName, 32, "window %d",
                  window->getID( ));
    else
        snprintf( _event.data.statistic.resourceName, 32, "%s", name.c_str( ));

    if( hint == NICEST )
        window->finish();
    _event.data.statistic.startTime  = window->getConfig()->getTime();
}


WindowStatistics::~WindowStatistics()
{
    Window*     window = _event.window;
    const int32_t hint = window->getIAttribute( Window::IATTR_HINT_STATISTICS );
    if( hint == OFF )
        return;

    if( _event.data.statistic.frameNumber == 0 ) // does not belong to a frame
        return;

    if( hint == NICEST )
        window->finish();

    _event.data.statistic.endTime = window->getConfig()->getTime();
    window->processEvent( _event );
}

}
