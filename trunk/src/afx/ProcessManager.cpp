/*!
    @file   ProcessManager.cpp
    @author dasyprocta
 */

#include "ProcessManager.hpp"
#include <boost/bind.hpp>

BEGIN_NAMESPACE_NYANCO

// ----------------------------------------------------------------------------
bool ProcessManager::RegisterProcess(ProcessType type, Creator& creator)
{
    return impl::ProcessManager::m_factory.registerCreator(type, creator);
}

// ============================================================================
struct ProcessExecutor
{
    void operator()(impl::ProcessPtr p)
    {
        p->execute(m_pm);
    }
    ProcessExecutor(ProcessManager& pm) : m_pm(pm) {}
    ProcessManager& m_pm;
};

END_NAMESPACE_NYANCO

BEGIN_NAMESPACE_NYANCO_IMPL

// ----------------------------------------------------------------------------
void ProcessManager::execute()
{
    using boost::bind;
    using boost::ref;
    std::for_each(
        m_processes.begin(),
        m_processes.end(),
        ProcessExecutor(*this));
}

END_NAMESPACE_NYANCO_IMPL
