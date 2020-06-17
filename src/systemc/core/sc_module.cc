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

#include <memory>
#include <vector>

#include "base/logging.hh"
#include "systemc/core/kernel.hh"
#include "systemc/core/module.hh"
#include "systemc/core/process_types.hh"
#include "systemc/ext/core/sc_module.hh"
#include "systemc/ext/core/sc_module_name.hh"

namespace sc_gem5
{

Process *
newMethodProcess(const char *name, ProcessFuncWrapper *func)
{
    Process *p = new Method(name, func);
    scheduler.reg(p);
    return p;
}

Process *
newThreadProcess(const char *name, ProcessFuncWrapper *func)
{
    Process *p = new Thread(name, func);
    scheduler.reg(p);
    return p;
}

Process *
newCThreadProcess(const char *name, ProcessFuncWrapper *func)
{
    Process *p = new CThread(name, func);
    scheduler.reg(p);
    return p;
}

} // namespace sc_gem5

namespace sc_core
{

sc_bind_proxy::sc_bind_proxy(const sc_interface &_interface) :
    _interface(&_interface), _port(nullptr)
{}

sc_bind_proxy::sc_bind_proxy(const sc_port_base &_port) :
    _interface(nullptr), _port(&_port)
{}

const sc_bind_proxy SC_BIND_PROXY_NUL(*(const sc_port_base *)nullptr);

sc_module::~sc_module() { delete _gem5_module; }

const sc_bind_proxy SC_BIND_PROXY_NIL(*(const sc_port_base *)nullptr);

void
sc_module::operator () (const sc_bind_proxy &p001,
                        const sc_bind_proxy &p002,
                        const sc_bind_proxy &p003,
                        const sc_bind_proxy &p004,
                        const sc_bind_proxy &p005,
                        const sc_bind_proxy &p006,
                        const sc_bind_proxy &p007,
                        const sc_bind_proxy &p008,
                        const sc_bind_proxy &p009,
                        const sc_bind_proxy &p010,
                        const sc_bind_proxy &p011,
                        const sc_bind_proxy &p012,
                        const sc_bind_proxy &p013,
                        const sc_bind_proxy &p014,
                        const sc_bind_proxy &p015,
                        const sc_bind_proxy &p016,
                        const sc_bind_proxy &p017,
                        const sc_bind_proxy &p018,
                        const sc_bind_proxy &p019,
                        const sc_bind_proxy &p020,
                        const sc_bind_proxy &p021,
                        const sc_bind_proxy &p022,
                        const sc_bind_proxy &p023,
                        const sc_bind_proxy &p024,
                        const sc_bind_proxy &p025,
                        const sc_bind_proxy &p026,
                        const sc_bind_proxy &p027,
                        const sc_bind_proxy &p028,
                        const sc_bind_proxy &p029,
                        const sc_bind_proxy &p030,
                        const sc_bind_proxy &p031,
                        const sc_bind_proxy &p032,
                        const sc_bind_proxy &p033,
                        const sc_bind_proxy &p034,
                        const sc_bind_proxy &p035,
                        const sc_bind_proxy &p036,
                        const sc_bind_proxy &p037,
                        const sc_bind_proxy &p038,
                        const sc_bind_proxy &p039,
                        const sc_bind_proxy &p040,
                        const sc_bind_proxy &p041,
                        const sc_bind_proxy &p042,
                        const sc_bind_proxy &p043,
                        const sc_bind_proxy &p044,
                        const sc_bind_proxy &p045,
                        const sc_bind_proxy &p046,
                        const sc_bind_proxy &p047,
                        const sc_bind_proxy &p048,
                        const sc_bind_proxy &p049,
                        const sc_bind_proxy &p050,
                        const sc_bind_proxy &p051,
                        const sc_bind_proxy &p052,
                        const sc_bind_proxy &p053,
                        const sc_bind_proxy &p054,
                        const sc_bind_proxy &p055,
                        const sc_bind_proxy &p056,
                        const sc_bind_proxy &p057,
                        const sc_bind_proxy &p058,
                        const sc_bind_proxy &p059,
                        const sc_bind_proxy &p060,
                        const sc_bind_proxy &p061,
                        const sc_bind_proxy &p062,
                        const sc_bind_proxy &p063,
                        const sc_bind_proxy &p064)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

const std::vector<sc_object *> &
sc_module::get_child_objects() const
{
    return _gem5_module->obj()->get_child_objects();
}

const std::vector<sc_event *> &
sc_module::get_child_events() const
{
    return _gem5_module->obj()->get_child_events();
}

sc_module::sc_module() :
    sc_object(sc_gem5::newModule()->name()),
    _gem5_module(sc_gem5::currentModule())
{}

sc_module::sc_module(const sc_module_name &) : sc_module() {}
sc_module::sc_module(const char *_name) : sc_module(sc_module_name(_name)) {}
sc_module::sc_module(const std::string &_name) :
    sc_module(sc_module_name(_name.c_str()))
{}

void
sc_module::reset_signal_is(const sc_in<bool> &, bool)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

void
sc_module::reset_signal_is(const sc_inout<bool> &, bool)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

void
sc_module::reset_signal_is(const sc_out<bool> &, bool)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

void
sc_module::reset_signal_is(const sc_signal_in_if<bool> &, bool)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}


void
sc_module::async_reset_signal_is(const sc_in<bool> &, bool)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

void
sc_module::async_reset_signal_is(const sc_inout<bool> &, bool)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

void
sc_module::async_reset_signal_is(const sc_out<bool> &, bool)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

void
sc_module::async_reset_signal_is(const sc_signal_in_if<bool> &, bool)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}


void
sc_module::dont_initialize()
{
    ::sc_gem5::Process::newest()->dontInitialize();
}

void
sc_module::set_stack_size(size_t size)
{
    ::sc_gem5::Process::newest()->setStackSize(size);
}


void sc_module::next_trigger() { ::sc_core::next_trigger(); }

void
sc_module::next_trigger(const sc_event &e)
{
    ::sc_core::next_trigger(e);
}

void
sc_module::next_trigger(const sc_event_or_list &eol)
{
    ::sc_core::next_trigger(eol);
}

void
sc_module::next_trigger(const sc_event_and_list &eal)
{
    ::sc_core::next_trigger(eal);
}

void
sc_module::next_trigger(const sc_time &t)
{
    ::sc_core::next_trigger(t);
}

void
sc_module::next_trigger(double d, sc_time_unit u)
{
    ::sc_core::next_trigger(d, u);
}

void
sc_module::next_trigger(const sc_time &t, const sc_event &e)
{
    ::sc_core::next_trigger(t, e);
}

void
sc_module::next_trigger(double d, sc_time_unit u, const sc_event &e)
{
    ::sc_core::next_trigger(d, u, e);
}

void
sc_module::next_trigger(const sc_time &t, const sc_event_or_list &eol)
{
    ::sc_core::next_trigger(t, eol);
}

void
sc_module::next_trigger(double d, sc_time_unit u, const sc_event_or_list &eol)
{
    ::sc_core::next_trigger(d, u, eol);
}

void
sc_module::next_trigger(const sc_time &t, const sc_event_and_list &eal)
{
    ::sc_core::next_trigger(t, eal);
}

void
sc_module::next_trigger(double d, sc_time_unit u, const sc_event_and_list &eal)
{
    ::sc_core::next_trigger(d, u, eal);
}


bool
sc_module::timed_out()
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
    return false;
}


void
sc_module::wait()
{
    ::sc_core::wait();
}

void
sc_module::wait(int i)
{
    ::sc_core::wait(i);
}

void
sc_module::wait(const sc_event &e)
{
    ::sc_core::wait(e);
}

void
sc_module::wait(const sc_event_or_list &eol)
{
    ::sc_core::wait(eol);
}

void
sc_module::wait(const sc_event_and_list &eal)
{
    ::sc_core::wait(eal);
}

void
sc_module::wait(const sc_time &t)
{
    ::sc_core::wait(t);
}

void
sc_module::wait(double d, sc_time_unit u)
{
    ::sc_core::wait(d, u);
}

void
sc_module::wait(const sc_time &t, const sc_event &e)
{
    ::sc_core::wait(t, e);
}

void
sc_module::wait(double d, sc_time_unit u, const sc_event &e)
{
    ::sc_core::wait(d, u, e);
}

void
sc_module::wait(const sc_time &t, const sc_event_or_list &eol)
{
    ::sc_core::wait(t, eol);
}

void
sc_module::wait(double d, sc_time_unit u, const sc_event_or_list &eol)
{
    ::sc_core::wait(d, u, eol);
}

void
sc_module::wait(const sc_time &t, const sc_event_and_list &eal)
{
    ::sc_core::wait(t, eal);
}

void
sc_module::wait(double d, sc_time_unit u, const sc_event_and_list &eal)
{
    ::sc_core::wait(d, u, eal);
}


void
sc_module::halt()
{
    ::sc_core::halt();
}

void
sc_module::at_posedge(const sc_signal_in_if<bool> &s)
{
    ::sc_core::at_posedge(s);
}

void
sc_module::at_posedge(const sc_signal_in_if<sc_dt::sc_logic> &s)
{
    ::sc_core::at_posedge(s);
}

void
sc_module::at_negedge(const sc_signal_in_if<bool> &s)
{
    ::sc_core::at_negedge(s);
}

void
sc_module::at_negedge(const sc_signal_in_if<sc_dt::sc_logic> &s)
{
    ::sc_core::at_negedge(s);
}


void
next_trigger()
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(nullptr);
}

void
next_trigger(const sc_event &e)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(new ::sc_gem5::SensitivityEvent(p, &e));
}

void
next_trigger(const sc_event_or_list &eol)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(new ::sc_gem5::SensitivityEventOrList(p, &eol));
}

void
next_trigger(const sc_event_and_list &eal)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(new ::sc_gem5::SensitivityEventAndList(p, &eal));
}

void
next_trigger(const sc_time &t)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(new ::sc_gem5::SensitivityTimeout(p, t));
}

void
next_trigger(double d, sc_time_unit u)
{
    next_trigger(sc_time(d, u));
}

void
next_trigger(const sc_time &t, const sc_event &e)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(new ::sc_gem5::SensitivityTimeoutAndEvent(p, t, &e));
}

void
next_trigger(double d, sc_time_unit u, const sc_event &e)
{
    next_trigger(sc_time(d, u), e);
}

void
next_trigger(const sc_time &t, const sc_event_or_list &eol)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(
            new ::sc_gem5::SensitivityTimeoutAndEventOrList(p, t, &eol));
}

void
next_trigger(double d, sc_time_unit u, const sc_event_or_list &eol)
{
    next_trigger(sc_time(d, u), eol);
}

void
next_trigger(const sc_time &t, const sc_event_and_list &eal)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(
            new ::sc_gem5::SensitivityTimeoutAndEventAndList(p, t, &eal));
}

void
next_trigger(double d, sc_time_unit u, const sc_event_and_list &eal)
{
    next_trigger(sc_time(d, u), eal);
}

bool
timed_out()
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
    return false;
}


void
wait()
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(nullptr);
    sc_gem5::scheduler.yield();
}

void
wait(int n)
{
    for (int i = 0; i < n; i++)
        wait();
}

void
wait(const sc_event &e)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(new ::sc_gem5::SensitivityEvent(p, &e));
    sc_gem5::scheduler.yield();
}

void
wait(const sc_event_or_list &eol)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(new ::sc_gem5::SensitivityEventOrList(p, &eol));
    sc_gem5::scheduler.yield();
}

void
wait(const sc_event_and_list &eal)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(new ::sc_gem5::SensitivityEventAndList(p, &eal));
    sc_gem5::scheduler.yield();
}

void
wait(const sc_time &t)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(new ::sc_gem5::SensitivityTimeout(p, t));
    sc_gem5::scheduler.yield();
}

void
wait(double d, sc_time_unit u)
{
    wait(sc_time(d, u));
}

void
wait(const sc_time &t, const sc_event &e)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(new ::sc_gem5::SensitivityTimeoutAndEvent(p, t, &e));
    sc_gem5::scheduler.yield();
}

void
wait(double d, sc_time_unit u, const sc_event &e)
{
    wait(sc_time(d, u), e);
}

void
wait(const sc_time &t, const sc_event_or_list &eol)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(
            new ::sc_gem5::SensitivityTimeoutAndEventOrList(p, t, &eol));
    sc_gem5::scheduler.yield();
}

void
wait(double d, sc_time_unit u, const sc_event_or_list &eol)
{
    wait(sc_time(d, u), eol);
}

void
wait(const sc_time &t, const sc_event_and_list &eal)
{
    sc_gem5::Process *p = sc_gem5::scheduler.current();
    p->setDynamic(
            new ::sc_gem5::SensitivityTimeoutAndEventAndList(p, t, &eal));
    sc_gem5::scheduler.yield();
}

void
wait(double d, sc_time_unit u, const sc_event_and_list &eal)
{
    wait(sc_time(d, u), eal);
}

void
halt()
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

void
at_posedge(const sc_signal_in_if<bool> &)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

void
at_posedge(const sc_signal_in_if<sc_dt::sc_logic> &)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

void
at_negedge(const sc_signal_in_if<bool> &)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

void
at_negedge(const sc_signal_in_if<sc_dt::sc_logic> &)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
}

const char *
sc_gen_unique_name(const char *)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
    return "";
}

bool
sc_hierarchical_name_exists(const char *name)
{
    warn("%s not implemented.\n", __PRETTY_FUNCTION__);
    return false;
}

bool
sc_start_of_simulation_invoked()
{
    return ::sc_gem5::kernel->startOfSimulationComplete();
}

bool
sc_end_of_simulation_invoked()
{
    return ::sc_gem5::kernel->endOfSimulationComplete();
}

sc_module *
sc_module_sc_new(sc_module *mod)
{
    static std::vector<std::unique_ptr<sc_module> > modules;
    modules.emplace_back(mod);
    return mod;
}

} // namespace sc_core
