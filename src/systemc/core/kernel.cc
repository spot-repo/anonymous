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

#include "systemc/core/kernel.hh"

#include "base/logging.hh"
#include "systemc/core/module.hh"
#include "systemc/core/scheduler.hh"

namespace sc_gem5
{

namespace
{

bool stopAfterCallbacks = false;
bool startComplete = false;
bool endComplete = false;

sc_core::sc_status _status = sc_core::SC_ELABORATION;

} // anonymous namespace

bool Kernel::startOfSimulationComplete() { return startComplete; }
bool Kernel::endOfSimulationComplete() { return endComplete; }

sc_core::sc_status Kernel::status() { return _status; }
void Kernel::status(sc_core::sc_status s) { _status = s; }

Kernel::Kernel(Params *params) :
    SimObject(params), t0Event(this, false, EventBase::Default_Pri - 1) {}

void
Kernel::init()
{
    status(::sc_core::SC_BEFORE_END_OF_ELABORATION);
    for (auto m: sc_gem5::allModules)
        m->sc_mod()->before_end_of_elaboration();

    if (stopAfterCallbacks)
        stopWork();
}

void
Kernel::regStats()
{
    status(::sc_core::SC_END_OF_ELABORATION);
    for (auto m: sc_gem5::allModules)
        m->sc_mod()->end_of_elaboration();

    if (stopAfterCallbacks)
        stopWork();
}

void
Kernel::startup()
{
    status(::sc_core::SC_START_OF_SIMULATION);
    for (auto m: sc_gem5::allModules)
        m->sc_mod()->start_of_simulation();

    startComplete = true;

    if (stopAfterCallbacks)
        stopWork();

    kernel->status(::sc_core::SC_RUNNING);

    schedule(t0Event, curTick());
    // Install ourselves as the scheduler's event manager.
    ::sc_gem5::scheduler.setEventQueue(eventQueue());
    // Run update once before the event queue starts.
    ::sc_gem5::scheduler.update();
}

void
Kernel::stop()
{
    if (status() < ::sc_core::SC_RUNNING)
        stopAfterCallbacks = true;
    else
        stopWork();
}

void
Kernel::stopWork()
{
    status(::sc_core::SC_END_OF_SIMULATION);
    for (auto m: sc_gem5::allModules)
        m->sc_mod()->end_of_simulation();

    endComplete = true;

    status(::sc_core::SC_STOPPED);

    if (stopAfterCallbacks)
        fatal("Simulation called sc_stop during elaboration.\n");
}

void
Kernel::t0Handler()
{
    // Now that the event queue has started, mark all the processes that
    // need to be initialized as ready to run.
    //
    // This event has greater priority than delta notifications and so will
    // happen before them, honoring the ordering for the initialization phase
    // in the spec. The delta phase will happen at normal priority, and then
    // the event which runs the processes which is at a lower priority.
    ::sc_gem5::scheduler.prepareForInit();

    status(::sc_core::SC_RUNNING);
}

Kernel *kernel;

} // namespace sc_gem5

sc_gem5::Kernel *
SystemC_KernelParams::create()
{
    panic_if(sc_gem5::kernel,
            "Only one systemc kernel object may be defined.\n");
    sc_gem5::kernel = new sc_gem5::Kernel(this);
    return sc_gem5::kernel;
}
