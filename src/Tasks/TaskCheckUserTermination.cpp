/**
   The Supporting Hyperplane Optimization Toolkit (SHOT).

   @author Andreas Lundell, Åbo Akademi University

   @section LICENSE
   This software is licensed under the Eclipse Public License 2.0.
   Please see the README and LICENSE files for more information.
*/

#include "TaskCheckUserTermination.h"

namespace SHOT
{

TaskCheckUserTermination::TaskCheckUserTermination(EnvironmentPtr envPtr, std::string taskIDTrue)
    : TaskBase(envPtr), taskIDIfTrue(taskIDTrue)
{
}

TaskCheckUserTermination::~TaskCheckUserTermination() {}

void TaskCheckUserTermination::run()
{
    env->events->notify(E_EventType::UserTerminationCheck);

    if(env->tasks->isTerminated())
    {
        env->results->terminationReason = E_TerminationReason::UserAbort;
        env->tasks->setNextTask(taskIDIfTrue);
        env->results->terminationReasonDescription = "Terminated by user.";
    }
}

std::string TaskCheckUserTermination::getType()
{
    std::string type = typeid(this).name();
    return (type);
}
} // namespace SHOT