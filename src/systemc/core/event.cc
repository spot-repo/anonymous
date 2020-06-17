/*
 * Copyright 2018 Google, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Gabe Black
 */

#include "systemc/core/event.hh"

#include <algorithm>
#include <cstring>
#include <utility>

#include "base/logging.hh"
#include "sim/core.hh"
#include "systemc/core/module.hh"
#include "systemc/core/scheduler.hh"

namespace sc_gem5
{

Event::Event(sc_core::sc_event *_sc_event) : Event(_sc_event, "") {}

Event::Event(sc_core::sc_event *_sc_event, const char *_basename) :
    _sc_event(_sc_event), _basename(_basename), delayedNotifyEvent(this)
{
    Module *p = currentModule();

    if (p)
        parent = p->obj()->sc_obj();
    else if (scheduler.current())
        parent = scheduler.current();
    else
        parent = nullptr;

    if (parent) {
        Object *obj = Object::getFromScObject(parent);
        obj->addChildEvent(_sc_event);
    } else {
        topLevelEvents.emplace(topLevelEvents.end(), _sc_event);
    }

    if (parent)
        _name = std::string(parent->name()) + "." + _basename;
    else
        _name = _basename;

    allEvents.emplace(allEvents.end(), _sc_event);

    // Determine if we're in the hierarchy (created once initialization starts
    // means no).
}

Event::~Event()
{
    if (parent) {
        Object *obj = Object::getFromScObject(parent);
        obj->delChildEvent(_sc_event);
    } else {
        EventsIt it = find(topLevelEvents.begin(), topLevelEvents.end(),
                           _sc_event);
        assert(it != topLevelEvents.end());
        std::swap(*it, topLevelEvents.back());
        topLevelEvents.pop_back();
    }

    EventsIt it = findEvent(_name);
    std::swap(*it, allEvents.back());
    allEvents.pop_back();

    if (delayedNotifyEvent.scheduled())
        scheduler.deschedule(&delayedNotifyEvent);
}

const std::string &
Event::name() const
{
    return _name;
}

const std::string &
Event::basename() const
{
    return _basename;
}

bool
Event::inHierarchy() const
{
    return _name.length() != 0;
}

sc_core::sc_object *
Event::getParentObject() const
{
    return parent;
}

void
Event::notify()
{
    auto local_sensitivities = sensitivities;
    for (auto s: local_sensitivities)
        s->notify(this);
}

void
Event::delayedNotify()
{
    scheduler.eventHappened();
    notify();
}

void
Event::notify(const sc_core::sc_time &t)
{
    //XXX We're assuming the systemc time resolution is in ps.
    Tick new_tick = t.value() * SimClock::Int::ps + scheduler.getCurTick();
    if (delayedNotifyEvent.scheduled()) {
        Tick old_tick = delayedNotifyEvent.when();

        if (new_tick >= old_tick)
            return;

        scheduler.deschedule(&delayedNotifyEvent);
    }

    scheduler.schedule(&delayedNotifyEvent, new_tick);
}

void
Event::cancel()
{
    if (delayedNotifyEvent.scheduled())
        scheduler.deschedule(&delayedNotifyEvent);
}

bool
Event::triggered() const
{
    return false;
}

Events topLevelEvents;
Events allEvents;

EventsIt
findEvent(const std::string &name)
{
    EventsIt it;
    for (it = allEvents.begin(); it != allEvents.end(); it++)
        if (!strcmp((*it)->name(), name.c_str()))
            break;

    return it;
}

} // namespace sc_gem5
