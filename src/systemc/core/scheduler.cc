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

#include "systemc/core/scheduler.hh"

#include "base/fiber.hh"
#include "base/logging.hh"
#include "sim/eventq.hh"
#include "systemc/core/kernel.hh"
#include "systemc/ext/core/sc_main.hh"

namespace sc_gem5
{

Scheduler::Scheduler() :
    eq(nullptr), readyEvent(this, false, ReadyPriority),
    pauseEvent(this, false, PausePriority),
    stopEvent(this, false, StopPriority),
    scMain(nullptr),
    starvationEvent(this, false, StarvationPriority),
    _started(false), _paused(false), _stopped(false),
    maxTickEvent(this, false, MaxTickPriority),
    _numCycles(0), _current(nullptr), initReady(false)
{}

void
Scheduler::prepareForInit()
{
    for (Process *p = toFinalize.getNext(); p; p = toFinalize.getNext()) {
        p->finalize();
        p->popListNode();
    }

    for (Process *p = initList.getNext(); p; p = initList.getNext()) {
        p->finalize();
        p->popListNode();
        p->ready();
    }

    for (auto ets: eventsToSchedule)
        eq->schedule(ets.first, ets.second);
    eventsToSchedule.clear();

    if (_started)
        eq->schedule(&maxTickEvent, maxTick);

    initReady = true;
}

void
Scheduler::reg(Process *p)
{
    if (initReady) {
        // If we're past initialization, finalize static sensitivity.
        p->finalize();
        // Mark the process as ready.
        p->ready();
    } else {
        // Otherwise, record that this process should be initialized once we
        // get there.
        initList.pushLast(p);
    }
}

void
Scheduler::dontInitialize(Process *p)
{
    if (initReady) {
        // Pop this process off of the ready list.
        p->popListNode();
    } else {
        // Push this process onto the list of processes which still need
        // their static sensitivity to be finalized. That implicitly pops it
        // off the list of processes to be initialized/marked ready.
        toFinalize.pushLast(p);
    }
}

void
Scheduler::yield()
{
    _current = readyList.getNext();
    if (!_current) {
        // There are no more processes, so return control to evaluate.
        Fiber::primaryFiber()->run();
    } else {
        _current->popListNode();
        // Switch to whatever Fiber is supposed to run this process. All
        // Fibers which aren't running should be parked at this line.
        _current->fiber()->run();
        // If the current process needs to be manually started, start it.
        if (_current && _current->needsStart())
            _current->run();
    }
    if (_current && _current->excWrapper) {
        // Make sure this isn't a method process.
        assert(!_current->needsStart());
        auto ew = _current->excWrapper;
        _current->excWrapper = nullptr;
        ew->throw_it();
    }
}

void
Scheduler::ready(Process *p)
{
    // Clump methods together to minimize context switching.
    if (p->procKind() == ::sc_core::SC_METHOD_PROC_)
        readyList.pushFirst(p);
    else
        readyList.pushLast(p);

    scheduleReadyEvent();
}

void
Scheduler::requestUpdate(Channel *c)
{
    updateList.pushLast(c);
    scheduleReadyEvent();
}

void
Scheduler::scheduleReadyEvent()
{
    // Schedule the evaluate and update phases.
    if (!readyEvent.scheduled()) {
        panic_if(!eq, "Need to schedule ready, but no event manager.\n");
        eq->schedule(&readyEvent, eq->getCurTick());
        if (starvationEvent.scheduled())
            eq->deschedule(&starvationEvent);
    }
}

void
Scheduler::scheduleStarvationEvent()
{
    if (!starvationEvent.scheduled()) {
        panic_if(!eq, "Need to schedule starvation event, "
                "but no event manager.\n");
        eq->schedule(&starvationEvent, eq->getCurTick());
        if (readyEvent.scheduled())
            eq->deschedule(&readyEvent);
    }
}

void
Scheduler::runReady()
{
    bool empty = readyList.empty();

    // The evaluation phase.
    do {
        yield();
    } while (!readyList.empty());

    if (!empty)
        _numCycles++;

    // The update phase.
    update();

    if (starved() && !runToTime)
        scheduleStarvationEvent();

    // The delta phase will happen naturally through the event queue.
}

void
Scheduler::update()
{
    Channel *channel = updateList.getNext();
    while (channel) {
        channel->popListNode();
        channel->update();
        channel = updateList.getNext();
    }
}

void
Scheduler::pause()
{
    _paused = true;
    kernel->status(::sc_core::SC_PAUSED);
    scMain->run();
}

void
Scheduler::stop()
{
    _stopped = true;
    kernel->stop();
    scMain->run();
}

void
Scheduler::start(Tick max_tick, bool run_to_time)
{
    // We should be running from sc_main. Keep track of that Fiber to return
    // to later.
    scMain = Fiber::currentFiber();

    _started = true;
    _paused = false;
    _stopped = false;
    runToTime = run_to_time;

    maxTick = max_tick;

    if (starved() && !runToTime)
        return;

    if (initReady) {
        kernel->status(::sc_core::SC_RUNNING);
        eq->schedule(&maxTickEvent, maxTick);
    }

    // Return to gem5 to let it run events, etc.
    Fiber::primaryFiber()->run();

    if (pauseEvent.scheduled())
        eq->deschedule(&pauseEvent);
    if (stopEvent.scheduled())
        eq->deschedule(&stopEvent);
    if (maxTickEvent.scheduled())
        eq->deschedule(&maxTickEvent);
    if (starvationEvent.scheduled())
        eq->deschedule(&starvationEvent);
}

void
Scheduler::schedulePause()
{
    if (pauseEvent.scheduled())
        return;

    eq->schedule(&pauseEvent, eq->getCurTick());
}

void
Scheduler::scheduleStop(bool finish_delta)
{
    if (stopEvent.scheduled())
        return;

    if (!finish_delta) {
        // If we're not supposed to finish the delta cycle, flush the list
        // of ready processes and scheduled updates.
        Process *p;
        while ((p = readyList.getNext()))
            p->popListNode();
        Channel *c;
        while ((c = updateList.getNext()))
            c->popListNode();
    }
    eq->schedule(&stopEvent, eq->getCurTick());
}

Scheduler scheduler;

} // namespace sc_gem5
