#pragma once

/*!
    @file   ProcessManager.hpp
    @author dasyprocta
 */

#include "afx_base_impl.hpp"
#include "ProcessManager.h"
#include <vector>
#include <map>

BEGIN_NAMESPACE_NYANCO_IMPL

typedef boost::shared_ptr<ProcessBase>  ProcessPtr;

// ============================================================================
class ProcessManager : nyanco::ProcessManager
{
public:
    ProcessId create(ProcessType type);
    void kill(ProcessId id);

    typedef boost::function<ProcessBase* ()> Creator;
    typedef std::vector<ProcessPtr>     ProcessList;

    class Factory
    {
    public:
        bool registerCreator(ProcessType type, Creator& creator)
        {
            m_callbackMap[type] = creator;
            return true;
        }

    private:
        typedef std::map<ProcessType, Creator> CallbackMap;

        ProcessPtr create(ProcessType type) const
        {
            CallbackMap::const_iterator it = m_callbackMap.find(type);
            if (it != m_callbackMap.end())
            {
                return ProcessPtr(it->second());
            }
            return ProcessPtr();
        }
        CallbackMap                     m_callbackMap;

        friend ProcessManager;
    };
    static Factory                      m_factory;

    void execute();

private:
    ProcessList                         m_processes;
};

END_NAMESPACE_NYANCO_IMPL
