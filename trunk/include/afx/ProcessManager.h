#pragma once

/*!
    @file   ProcessManager.h
    @author dasyprocta
 */

#include "base.h"
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/lambda/construct.hpp>

BEGIN_NAMESPACE_NYANCO

class ProcessManager;
class Framework;

typedef sint32                          ProcessId;
typedef sint32                          SignalId;
typedef sint32                          ProcessType;

struct ProcessExecutor;

// ============================================================================
class ProcessBase : boost::noncopyable
{
public:
    struct State
    {
        enum Type
        {
            Creating,
            Created,
            Running,
        };
    };

    template <typename Derived_>
    class Property
    {
    public:
        static ProcessType const Type = 0; /* UNDONE: generaate a unique number */

    private:
        typedef Derived_ Derived;
        typedef boost::lambda::new_ptr<Derived> Creator;
        static bool const Registered  = ProcessManager::RegisterProcess(Type, Creator());
    };

private:
    virtual void execute(ProcessManager& pm) = 0;

    ProcessId                           m_id;
    State::Type                         m_state;
    sint32                              m_userId;
    sint32                              m_groupId;

    friend ProcessExecutor;
};

template <typename Derived_>
class Process
    : ProcessBase,
      public ProcessBase::Property<Derived_> {};

template <typename Derived_>
class SmProcess : Process<Derived_>
{
    typedef Derived_ Derived;
    typedef void (Derived::*Procedure)(ProcessManager&);

    void execute(ProcessManager& pm)
    {
        if (m_procedure)
        {
            ((static_cast<Derived*>(this))->(*m_procedure))(pm);
        }
    }

    SmProcess() : m_procedure(0) {}
    SmProcess(Procedure procedure) : m_procedure(procedure) {}
    Procedure                           m_procedure;
};

// ============================================================================
class ProcessManager : boost::noncopyable
{
public:
    virtual ProcessId create(ProcessType type) = 0;
    virtual void kill(ProcessId id) = 0;

    typedef boost::function<ProcessBase* ()> Creator;
    static bool RegisterProcess(ProcessType type, Creator& creator);
};

END_NAMESPACE_NYANCO
