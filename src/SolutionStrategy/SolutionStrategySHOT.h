/*

 * SolutionStrategySHOT.h
 *
 *  Created on: Mar 30, 2015
 *      Author: alundell
 */

#pragma once

#include <ISolutionStrategy.h>
#include <TaskFindInteriorPoint.h>

#include "Tasks/TaskBase.h"
#include "Tasks/TaskSequential.h"
#include "Tasks/TaskGoto.h"
#include "Tasks/TaskConditional.h"

#include "Tasks/TaskInitializeOriginalProblem.h"
#include "Tasks/TaskPrintProblemStats.h"
#include "Tasks/TaskInitializeIteration.h"
#include "Tasks/TaskTerminate.h"

#include "Tasks/TaskInitializeMILPSolver.h"
#include "Tasks/TaskCreateMILPProblem.h"

#include "Tasks/TaskPrintIterationHeader.h"
#include "Tasks/TaskExecuteSolutionLimitStrategy.h"
#include "Tasks/TaskExecuteRelaxationStrategy.h"

#include "Tasks/TaskPrintIterationReport.h"
#include "Tasks/TaskPrintSolutionBoundReport.h"

#include "Tasks/TaskSolveIteration.h"

#include "Tasks/TaskCheckAbsoluteGap.h"
#include "Tasks/TaskCheckIterationError.h"
#include "Tasks/TaskCheckIterationLimit.h"
#include "Tasks/TaskCheckObjectiveStagnation.h"
#include "Tasks/TaskCheckConstraintTolerance.h"
#include "Tasks/TaskCheckRelativeGap.h"
#include "Tasks/TaskCheckTimeLimit.h"

#include "Tasks/TaskPrintSolution.h"

#include "Tasks/TaskSelectHyperplanePointsLinesearch.h"
#include "Tasks/TaskSelectHyperplanePointsIndividualLinesearch.h"
#include "Tasks/TaskSelectHyperplanePointsSolution.h"
#include "Tasks/TaskAddHyperplanes.h"

#include "Tasks/TaskCheckPrimalSolutionCandidates.h"
#include "Tasks/TaskSelectPrimalCandidatesFromSolutionPool.h"
#include "Tasks/TaskSelectPrimalFixedNLPPointsFromSolutionPool.h"
#include "Tasks/TaskSelectPrimalCandidatesFromLinesearch.h"
#include "Tasks/TaskSelectPrimalCandidatesFromNLP.h"

#include "Tasks/TaskCheckDualSolutionCandidates.h"

#include "Tasks/TaskUpdateNonlinearObjectiveByLinesearch.h"
#include "Tasks/TaskSolveFixedLinearProblem.h"

#include "Tasks/TaskSwitchToLazyConstraints.h"

#include "SHOTSettings.h"
#include "../ProcessInfo.h"

class SolutionStrategySHOT: public ISolutionStrategy
{
	public:
		SolutionStrategySHOT(OSInstance *osInstance);
		virtual ~SolutionStrategySHOT();

		virtual bool solveProblem();
		virtual void initializeStrategy();

	protected:

		SHOTSettings::Settings *settings;
		ProcessInfo *processInfo;
		//TaskSequential *mainTask;

};

