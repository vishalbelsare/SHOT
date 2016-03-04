/*
 * TaskCheckDualSolutionCandidates.h
 *
 *  Created on: Nov 22, 2015
 *      Author: alundell
 */

#pragma once

#include <TaskBase.h>
#include "../ProcessInfo.h"
#include "../OptProblems/OptProblemOriginal.h"

class TaskCheckDualSolutionCandidates: public TaskBase
{
	public:
		TaskCheckDualSolutionCandidates();
		virtual ~TaskCheckDualSolutionCandidates();

		virtual void run();
		virtual std::string getType();
	private:

		SHOTSettings::Settings *settings;
		ProcessInfo *processInfo;
};

