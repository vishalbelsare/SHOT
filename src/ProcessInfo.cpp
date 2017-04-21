#include "ProcessInfo.h"
#include "OptProblems/OptProblemOriginal.h"

bool ProcessInfo::instanceFlag = false;
ProcessInfo* ProcessInfo::single = NULL;
OSResult *osResult = NULL;

extern const OSSmartPtr<OSOutput> osoutput;

void ProcessInfo::addPrimalSolution(vector<double> pt, E_PrimalSolutionSource source, double objVal, int iter,
		IndexValuePair maxConstrDev)
{
	PrimalSolution sol =
	{ pt, source, objVal, iter, maxConstrDev };

	primalSolutions.push_back(sol);
}

void ProcessInfo::addPrimalSolution(vector<double> pt, E_PrimalSolutionSource source, double objVal, int iter)
{

	auto maxConstrDev = originalProblem->getMostDeviatingConstraint(pt);

	ProcessInfo::addPrimalSolution(pt, source, objVal, iter, maxConstrDev);
}

void ProcessInfo::addDualSolution(vector<double> pt, E_DualSolutionSource source, double objVal, int iter)
{
	DualSolution sol =
	{ pt, source, objVal, iter };

	addDualSolution(sol);
}

void ProcessInfo::addDualSolution(SolutionPoint pt, E_DualSolutionSource source)
{
	DualSolution sol =
	{ pt.point, source, pt.objectiveValue, pt.iterFound };

	addDualSolution(sol);
}

void ProcessInfo::addDualSolution(DualSolution solution)
{
	if (dualSolutions.size() == 0)
	{
		dualSolutions.push_back(solution);
	}
	else
	{
		dualSolutions.at(0) = solution;
	}
}

void ProcessInfo::addPrimalSolutionCandidate(vector<double> pt, E_PrimalSolutionSource source, int iter)
{
	PrimalSolution sol =
	{ pt, source, originalProblem->calculateOriginalObjectiveValue(pt), iter,
			originalProblem->getMostDeviatingConstraint(pt) };

	primalSolutionCandidates.push_back(sol);

	this->checkPrimalSolutionCandidates();
}

void ProcessInfo::addPrimalSolutionCandidates(vector<vector<double> > pts, E_PrimalSolutionSource source, int iter)
{
	for (auto PT : pts)
	{
		addPrimalSolutionCandidate(PT, source, iter);
	}
}

void ProcessInfo::addDualSolutionCandidate(vector<double> pt, E_DualSolutionSource source, int iter)
{
	double tmpObjVal = this->originalProblem->calculateOriginalObjectiveValue(pt);

	DualSolution sol =
	{ pt, source, tmpObjVal, iter };

	addDualSolutionCandidate(sol);
}

void ProcessInfo::addDualSolutionCandidate(SolutionPoint pt, E_DualSolutionSource source)
{
	DualSolution sol =
	{ pt.point, source, pt.objectiveValue, pt.iterFound };

	addDualSolutionCandidate(sol);
}

void ProcessInfo::addDualSolutionCandidates(std::vector<SolutionPoint> pts, E_DualSolutionSource source)
{
	for (auto pt : pts)
	{
		addDualSolutionCandidate(pt, source);
	}
}

void ProcessInfo::addDualSolutionCandidate(DualSolution solution)
{
	dualSolutionCandidates.push_back(solution);

	this->checkDualSolutionCandidates();
}

pair<double, double> ProcessInfo::getCorrectedObjectiveBounds()
{
	pair<double, double> bounds;

	if (originalProblem->isTypeOfObjectiveMinimize())
	{
		bounds.first = currentObjectiveBounds.first;
		bounds.second = currentObjectiveBounds.second;
	}
	else
	{
		bounds.first = currentObjectiveBounds.second;
		bounds.second = currentObjectiveBounds.first;
	}

	return (bounds);
}

void ProcessInfo::addPrimalSolution(SolutionPoint pt, E_PrimalSolutionSource source)
{
	PrimalSolution sol =
	{ pt.point, source, pt.objectiveValue, pt.iterFound };

	if (settings->getBoolSetting("SaveAllPrimalSolutions", "SHOTSolver"))
	{
		primalSolutions.push_back(sol);
	}
	else
	{
		if (primalSolutions.size() == 0) primalSolutions.push_back(sol);
		else primalSolutions.at(0) = sol;
	}
}

void ProcessInfo::addPrimalSolutionCandidate(SolutionPoint pt, E_PrimalSolutionSource source)
{
	PrimalSolution sol =
	{ pt.point, source, pt.objectiveValue, pt.iterFound };

	primalSolutionCandidates.push_back(sol);

	this->checkPrimalSolutionCandidates();
}

void ProcessInfo::addPrimalSolutionCandidates(std::vector<SolutionPoint> pts, E_PrimalSolutionSource source)
{
	for (auto pt : pts)
	{
		addPrimalSolutionCandidate(pt, source);
	}
}

void ProcessInfo::addPrimalFixedNLPCandidate(vector<double> pt, E_PrimalNLPSource source, double objVal, int iter,
		IndexValuePair maxConstrDev)
{
	PrimalFixedNLPCandidate cand =
	{ pt, source, objVal, iter };

	primalFixedNLPCandidates.push_back(cand);
}

void ProcessInfo::setObjectiveUpdatedByLinesearch(bool updated)
{
	objectiveUpdatedByLinesearch = updated;
}

bool ProcessInfo::getObjectiveUpdatedByLinesearch()
{
	return (objectiveUpdatedByLinesearch);
}

void ProcessInfo::outputAlways(std::string message)
{
	osoutput->OSPrint(ENUM_OUTPUT_AREA_main, ENUM_OUTPUT_LEVEL_always, message);
}

void ProcessInfo::outputError(std::string message)
{
	osoutput->OSPrint(ENUM_OUTPUT_AREA_main, ENUM_OUTPUT_LEVEL_error, message);
}

void ProcessInfo::outputError(std::string message, std::string errormessage)
{
	osoutput->OSPrint(ENUM_OUTPUT_AREA_main, ENUM_OUTPUT_LEVEL_error, message);
	osoutput->OSPrint(ENUM_OUTPUT_AREA_main, ENUM_OUTPUT_LEVEL_error, "Error message: " + errormessage);
}

void ProcessInfo::outputSummary(std::string message)
{
	osoutput->OSPrint(ENUM_OUTPUT_AREA_main, ENUM_OUTPUT_LEVEL_summary, message);
}

void ProcessInfo::outputWarning(std::string message)
{
	osoutput->OSPrint(ENUM_OUTPUT_AREA_main, ENUM_OUTPUT_LEVEL_warning, message);
}

void ProcessInfo::outputInfo(std::string message)
{
	osoutput->OSPrint(ENUM_OUTPUT_AREA_main, ENUM_OUTPUT_LEVEL_info, message);
}

void ProcessInfo::outputDebug(std::string message)
{
	osoutput->OSPrint(ENUM_OUTPUT_AREA_main, ENUM_OUTPUT_LEVEL_debug, message);
}

void ProcessInfo::outputTrace(std::string message)
{
	//osoutput->OSPrint(ENUM_OUTPUT_AREA_main, ENUM_OUTPUT_LEVEL_detailed_trace, message);
}

void ProcessInfo::outputDetailedTrace(std::string message)
{
	//osoutput->OSPrint(ENUM_OUTPUT_AREA_main, ENUM_OUTPUT_LEVEL_detailed_trace, message);
}

void ProcessInfo::checkPrimalSolutionCandidates()
{
	this->startTimer("PrimalBoundTotal");

	for (auto cand : this->primalSolutionCandidates)
	{
		this->checkPrimalSolutionPoint(cand);
	}

	this->primalSolutionCandidates.clear();

	this->stopTimer("PrimalBoundTotal");
}

void ProcessInfo::checkDualSolutionCandidates()
{
	bool isMinimization = this->originalProblem->isTypeOfObjectiveMinimize();

	double currDualBound = this->getDualBound();
	double currPrimalBound = this->getPrimalBound();

	for (auto C : this->dualSolutionCandidates)
	{

		if ((isMinimization && (C.objValue > currDualBound && C.objValue <= currPrimalBound))
				|| (!isMinimization && (C.objValue < currDualBound && C.objValue >= currPrimalBound)))
		{
			// New dual solution
			this->currentObjectiveBounds.first = C.objValue;
			currDualBound = C.objValue;
			this->iterLastDualBoundUpdate = this->getCurrentIteration()->iterationNumber;
			this->timeLastDualBoundUpdate = this->getElapsedTime("Total");

			// If the solution is MILP feasible we only have a bound, no variable solutions
			if (C.sourceType != E_DualSolutionSource::MILPSolutionFeasible)
			{
				this->addDualSolution(C);
			}

			std::string sourceDesc;

			switch (C.sourceType)
			{
				/*case E_DualSolutionSource::Linesearch:
				 sourceDesc = "line search";
				 break;*/
				case E_DualSolutionSource::LPSolution:
					sourceDesc = "LP solution";
					break;
				case E_DualSolutionSource::MILPSolutionOptimal:
					sourceDesc = "MILP solution";
					break;
				case E_DualSolutionSource::MILPSolutionFeasible:
					sourceDesc = "MILP solution bound";
					break;
				case E_DualSolutionSource::ObjectiveConstraint:
					sourceDesc = "Obj. constr. linesearch";
					break;
				default:
					break;
			}

			auto tmpLine = boost::format("     New dual bound %1% (%2%) ") % C.objValue % sourceDesc;

			this->outputInfo(tmpLine.str());

		}
	}

	this->dualSolutionCandidates.clear();
}

bool ProcessInfo::isRelativeObjectiveGapToleranceMet()
{
	if (this->getRelativeObjectiveGap() <= settings->getDoubleSetting("GapTermTolRelative", "Algorithm"))
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

bool ProcessInfo::isAbsoluteObjectiveGapToleranceMet()
{
	if (this->getAbsoluteObjectiveGap() <= settings->getDoubleSetting("GapTermTolAbsolute", "Algorithm"))
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

bool ProcessInfo::checkPrimalSolutionPoint(PrimalSolution primalSol)
{
	std::string sourceDesc;

	std::vector<double> tmpPoint(primalSol.point);
	double tmpObjVal = primalSol.objValue;

	bool isMinimization = this->originalProblem->isTypeOfObjectiveMinimize();

	char HPadded = ' ';

	bool isLinConstrFulfilled = false;
	bool isNonLinConstrFulfilled = false;
	IndexValuePair mostDev;

	switch (primalSol.sourceType)
	{
		case E_PrimalSolutionSource::Linesearch:
			sourceDesc = "line search";
			break;
		case E_PrimalSolutionSource::LinesearchFixedIntegers:
			sourceDesc = "line search fixed";
			break;
		case E_PrimalSolutionSource::NLPFixedIntegers:
			sourceDesc = "NLP fixed";
			break;
		case E_PrimalSolutionSource::NLPRelaxed:
			sourceDesc = "NLP relaxed";
			break;
		case E_PrimalSolutionSource::MILPSolutionPool:
			sourceDesc = "MILP sol. pool";
			break;
		case E_PrimalSolutionSource::ObjectiveConstraint:
			sourceDesc = "Obj. constr.";
			break;
		case E_PrimalSolutionSource::LPFixedIntegers:
			sourceDesc = "LP fixed";
			break;
		case E_PrimalSolutionSource::LazyConstraintCallback:
			sourceDesc = "Lazy constraint callback";
			break;
		case E_PrimalSolutionSource::HeuristicCallback:
			sourceDesc = "Heuristic constraint callback";
			break;
		case E_PrimalSolutionSource::IncumbentCallback:
			sourceDesc = "Incumbent constraint callback";
			break;
		default:
			break;
	}

	if (std::isnan(primalSol.objValue))
	{
		tmpObjVal = this->originalProblem->calculateOriginalObjectiveValue(primalSol.point);
	}

	if (primalSol.sourceType == E_PrimalSolutionSource::MILPSolutionPool
			|| primalSol.sourceType == E_PrimalSolutionSource::NLPFixedIntegers/*
			 || primalSol.sourceType == E_PrimalSolutionSource::LazyConstraintCallback*/)
	{
		isLinConstrFulfilled = true;
		mostDev = primalSol.maxDevatingConstraint;
	}
	else
	{
		bool isRounded = false;

		auto discreteVarIndexes = this->originalProblem->getDiscreteVariableIndices();

		std::vector<double> ptRounded(tmpPoint);

		for (int i = 0; i < discreteVarIndexes.size(); i++)
		{
			int idx = discreteVarIndexes.at(i);
			double rounded = round(tmpPoint.at(idx));

			if (abs(rounded - tmpPoint.at(idx)) > 0)
			{
				ptRounded.at(idx) = rounded;
				isRounded = true;
			}
		}

		if (isRounded)
		{
			tmpPoint = ptRounded;
			tmpObjVal = this->originalProblem->calculateOriginalObjectiveValue(ptRounded);

			if (this->originalProblem->isObjectiveFunctionNonlinear())
			{
				tmpPoint.at(this->originalProblem->getNonlinearObjectiveVariableIdx()) = tmpObjVal;
			}

		}

		isLinConstrFulfilled = this->originalProblem->isLinearConstraintsFulfilledInPoint(tmpPoint,
				settings->getDoubleSetting("PrimalBoundLinearTolerance", "PrimalBound"));
	}

	isNonLinConstrFulfilled = this->originalProblem->isConstraintsFulfilledInPoint(tmpPoint,
			settings->getDoubleSetting("PrimalBoundNonlinearTolerance", "PrimalBound"));

	bool isMostDevConstrNonlinear = this->originalProblem->isConstraintNonlinear(mostDev.idx);

	if (primalSol.sourceType != E_PrimalSolutionSource::MILPSolutionPool && isMostDevConstrNonlinear
			&& ((mostDev.value > 0
			/*&& mostDev.value < 10000000000 * settings->getDoubleSetting("ConstrTermTolMILP", "Algorithm")*/)
					|| primalSol.sourceType == E_PrimalSolutionSource::ObjectiveConstraint)) // Add point as hyperplane
	{
		Hyperplane hyperplane;
		hyperplane.sourceConstraintIndex = mostDev.idx;
		hyperplane.generatedPoint = tmpPoint;
		hyperplane.source = E_HyperplaneSource::PrimalSolutionSearch;

		this->hyperplaneWaitingList.push_back(hyperplane);

		HPadded = '*';

	}

	bool updatePrimal = isLinConstrFulfilled && isNonLinConstrFulfilled;

	updatePrimal = updatePrimal
			&& ((isMinimization && tmpObjVal < this->currentObjectiveBounds.second)
					|| (!isMinimization && tmpObjVal > this->currentObjectiveBounds.second));

	if (updatePrimal)
	{
		char HPobjadded = ' ';

		if (settings->getBoolSetting("UsePrimalObjectiveCut", "MILP")
				&& this->originalProblem->isObjectiveFunctionNonlinear())
		{
			auto objConstrVal = this->originalProblem->calculateConstraintFunctionValue(-1, primalSol.point)
					- primalSol.point.back();

			if (objConstrVal < 0)
			{

				Hyperplane hyperplane;
				hyperplane.sourceConstraintIndex = -1;
				hyperplane.generatedPoint = tmpPoint;
				hyperplane.source = E_HyperplaneSource::PrimalSolutionSearchInteriorObjective;

				this->hyperplaneWaitingList.push_back(hyperplane);

				HPobjadded = '#';
			}
		}

		this->currentObjectiveBounds.second = tmpObjVal;

		auto tmpLine = boost::format("     New primal bound %1% with dev. %2% (%3%) %4% %5%") % tmpObjVal
				% mostDev.value % sourceDesc % HPadded % HPobjadded;

		this->outputInfo(tmpLine.str());

		primalSol.objValue = tmpObjVal;
		primalSol.point = tmpPoint;
		primalSol.maxDevatingConstraint = mostDev;

		this->primalSolutions.push_back(primalSol);

		this->primalSolution = tmpPoint;

		if (this->interiorPts.size() > 0)
		{
			// Add the new point if it is deeper within the feasible region
			if (primalSol.maxDevatingConstraint.value < this->interiorPts.at(0).maxDevatingConstraint.value)
			{
				InteriorPoint tmpIP;
				tmpIP.point = tmpPoint;
				tmpIP.maxDevatingConstraint = mostDev;

				this->outputSummary(
						"     Interior point replaced with primal solution point due to constraint deviation.");

				this->interiorPts.back() = tmpIP;
			}
			else if (settings->getIntSetting("AddPrimalBoundAsInteriorPoint", "Algorithm")
					== static_cast<int>(ES_AddPrimalPointAsInteriorPoint::KeepBoth) && mostDev.value < 0)
			{
				InteriorPoint tmpIP;

				tmpIP.point = tmpPoint;
				tmpIP.maxDevatingConstraint = mostDev;

				this->outputSummary("     Primal solution point used as additional interior point.");

				if (this->interiorPts.size() == this->numOriginalInteriorPoints)
				{
					this->interiorPts.push_back(tmpIP);
				}
				else
				{
					this->interiorPts.back() = tmpIP;
				}
			}
			else if (settings->getIntSetting("AddPrimalBoundAsInteriorPoint", "Algorithm")
					== static_cast<int>(ES_AddPrimalPointAsInteriorPoint::KeepNew) && mostDev.value < 0)
			{
				InteriorPoint tmpIP;

				// Add the new point only
				tmpIP.point = tmpPoint;
				tmpIP.maxDevatingConstraint = mostDev;

				this->outputSummary("     Interior point replaced with primal solution point.");

				this->interiorPts.back() = tmpIP;

			}
			else if (settings->getIntSetting("AddPrimalBoundAsInteriorPoint", "Algorithm")
					== static_cast<int>(ES_AddPrimalPointAsInteriorPoint::OnlyAverage) && mostDev.value < 0)
			{
				InteriorPoint tmpIP;

				// Find a new point in the midpoint between the original and new
				for (int i = 0; i < tmpPoint.size(); i++)
				{
					tmpPoint.at(i) = (0.5 * tmpPoint.at(i) + 0.5 * this->interiorPts.at(0).point.at(i));
				}

				tmpIP.point = tmpPoint;
				tmpIP.maxDevatingConstraint = this->originalProblem->getMostDeviatingConstraint(tmpPoint);

				this->outputSummary("     Interior point replaced with primal solution point.");

				this->interiorPts.back() = tmpIP;

			}
		}

		return (true);
	}

	return (false);
}

ProcessInfo::ProcessInfo()
{
	createTimer("Total", "Total solution time");

	iterLP = 0;
	iterQP = 0;
	iterFeasMILP = 0;
	iterOptMILP = 0;
	iterFeasMIQP = 0;
	iterOptMIQP = 0;

	numNLPProbsSolved = 0;
	numPrimalFixedNLPProbsSolved = 0;

	itersWithStagnationMILP = 0;
	iterSignificantObjectiveUpdate = 0;
	itersMILPWithoutNLPCall = 0;
	solTimeLastNLPCall = 0;

	numFunctionEvals = 0;
	numGradientEvals = 0;

	iterLastPrimalBoundUpdate = 0;
	iterLastDualBoundUpdate = 0;

	numOriginalInteriorPoints = 0;

	numConstraintsRemovedInPresolve = 0;

	numIntegerCutsAdded = 0;

	currentObjectiveBounds.first = -OSDBL_MAX;
	currentObjectiveBounds.second = OSDBL_MAX;

	settings = SHOTSettings::Settings::getInstance();

	tasks = new TaskHandler();

	objectiveUpdatedByLinesearch = false;
}

ProcessInfo * ProcessInfo::getInstance()
{
	if (!instanceFlag)
	{
		single = new ProcessInfo();
		instanceFlag = true;
		return (single);
	}
	else
	{
		return (single);
	}
}

void ProcessInfo::setOriginalProblem(OptProblemOriginal *problem)
{
	this->originalProblem = problem;

	bool isMinimization = originalProblem->isTypeOfObjectiveMinimize();

	if (isMinimization)
	{
		currentObjectiveBounds.first = -OSDBL_MAX;
		currentObjectiveBounds.second = OSDBL_MAX;
	}
	else
	{
		currentObjectiveBounds.first = OSDBL_MAX;
		currentObjectiveBounds.second = -OSDBL_MAX;
	}

}

void ProcessInfo::createTimer(string name, string description)
{
	Timer tmpTimer = Timer(name, description);

	timers.push_back(tmpTimer);
}

void ProcessInfo::startTimer(string name)
{
	auto timer = std::find_if(timers.begin(), timers.end(), [name](Timer const & T)
	{
		return (T.name == name);
	});

	if (timer == timers.end())
	{
		outputError("Timer with name  \" " + name + "\" not found!");
		return;
	}

	timer->start();
}

void ProcessInfo::restartTimer(string name)
{
	auto timer = std::find_if(timers.begin(), timers.end(), [name](Timer const & T)
	{
		return (T.name == name);
	});

	if (timer == timers.end())
	{
		outputError("Timer with name  \" " + name + "\" not found!");
		return;
	}

	timer->restart();
}

void ProcessInfo::stopTimer(string name)
{
	auto timer = std::find_if(timers.begin(), timers.end(), [name](Timer const & T)
	{
		return (T.name == name);
	});

	if (timer == timers.end())
	{
		outputError("Timer with name  \" " + name + "\" not found!");
		return;
	}

	timer->stop();
}

double ProcessInfo::getElapsedTime(string name)
{
	auto timer = std::find_if(timers.begin(), timers.end(), [name](Timer const & T)
	{
		return (T.name == name);
	});

	if (timer == timers.end())
	{
		outputError("Timer with name  \" " + name + "\" not found!");
		return (0.0);
	}

	return (timer->elapsed());
}

void ProcessInfo::initializeResults(int numObj, int numVar, int numConstr)
{
	osResult = new OSResult();
	osResult->setObjectiveNumber(numObj);
	osResult->setVariableNumber(numVar);
	osResult->setConstraintNumber(numConstr);
}

std::string ProcessInfo::getOSrl()
{
	auto varNames = originalProblem->getVariableNames();
	auto constrNames = originalProblem->getConstraintNames();
	int numConstr = osResult->getConstraintNumber();

	int numVar = osResult->getVariableNumber();

	int numPrimalSols = primalSolutions.size();

	osResult->setNumberOfOtherGeneralResults(1);
	osResult->setOtherGeneralResultName(0, "UsedOptions");

	//std::cout << settings->getSettingsAsString() << std::endl;
	osResult->setOtherGeneralResultValue(0, settings->getSettingsAsString());

	if (numPrimalSols == 0)
	{
		osResult->setSolutionNumber(1);

		osResult->setNumberOfObjValues(0, 1);

		std::stringstream strstrdb;
		strstrdb << std::fixed << std::setprecision(15) << getDualBound();

		osResult->setAnOtherSolutionResult(0, "DualObjectiveBound", strstrdb.str(), "Final solution",
				"The dual bound for the objective", 0, NULL);

		if (dualSolutions.size() > 0)
		{
			osResult->setObjValue(0, 0, -1, "", dualSolutions.back().objValue);

			std::stringstream strstr;
			strstr << std::fixed << std::setprecision(15)
					<< this->originalProblem->getMostDeviatingConstraint(dualSolutions.back().point).value;

			osResult->setAnOtherSolutionResult(0, "MaxErrorConstrs", strstr.str(), "Final solution",
					"Maximal error in constraint", 0, NULL);
		}

		std::stringstream strstr2;
		strstr2 << std::fixed << std::setprecision(15) << getAbsoluteObjectiveGap();

		osResult->setAnOtherSolutionResult(numPrimalSols - 1, "AbsOptimalityGap", strstr2.str(), "Final solution",
				"The absolute optimality gap", 0, NULL);

		std::stringstream strstr3;
		strstr3 << std::fixed << std::setprecision(15) << getRelativeObjectiveGap();

		osResult->setAnOtherSolutionResult(numPrimalSols - 1, "RelOptimalityGap", strstr3.str(), "Final solution",
				"The relative optimality gap", 0, NULL);

	}
	else
	{

		osResult->setSolutionNumber(numPrimalSols);

		for (int i = 0; i < numPrimalSols; i++)
		{
			osResult->setNumberOfVarValues(i, numVar);
			osResult->setNumberOfObjValues(i, 1);
			osResult->setNumberOfPrimalVariableValues(i, numVar);
			osResult->setObjValue(i, 0, -1, "", primalSolutions.at(i).objValue);

			auto primalPoint = primalSolutions.at(i).point;

			osResult->setPrimalVariableValuesDense(i, &primalPoint[0]);

			for (int j = 0; j < numVar; j++)
			{
				osResult->setVarValue(i, j, j, varNames.at(j), primalPoint.at(j));
			}

			std::vector<double> tmpConstrVals;

			osResult->setNumberOfDualValues(i, numConstr);

			for (int j = 0; j < numConstr; j++)
			{
				//tmpConstrVals.push_back(originalProblem->calculateConstraintFunctionValue(j, primalPoint));
				osResult->setDualValue(i, j, j, constrNames.at(j),
						originalProblem->calculateConstraintFunctionValue(j, primalPoint));
			}

			//osResult->setConstraintValuesDense(i, &tmpConstrVals[0]);
		}

		std::stringstream strstrdb;
		strstrdb << std::fixed << std::setprecision(15) << getDualBound();

		osResult->setAnOtherSolutionResult(numPrimalSols - 1, "DualObjectiveBound", strstrdb.str(), "Final solution",
				"The dual bound for the objective", 0, NULL);

		std::stringstream strstrpb;
		strstrpb << std::fixed << std::setprecision(15) << getPrimalBound();

		osResult->setAnOtherSolutionResult(numPrimalSols - 1, "PrimalObjectiveBound", strstrpb.str(), "Final solution",
				"The primal bound for the objective", 0, NULL);

		std::stringstream strstr;
		strstr << std::fixed << std::setprecision(15) << getCurrentIteration()->maxDeviation;

		osResult->setAnOtherSolutionResult(numPrimalSols - 1, "MaxErrorConstrs", strstr.str(), "Final solution",
				"Maximal error in constraint", 0, NULL);

		std::stringstream strstr2;
		strstr2 << std::fixed << std::setprecision(15) << getAbsoluteObjectiveGap();

		osResult->setAnOtherSolutionResult(numPrimalSols - 1, "AbsOptimalityGap", strstr2.str(), "Final solution",
				"The absolute optimality gap", 0, NULL);

		std::stringstream strstr3;
		strstr3 << std::fixed << std::setprecision(15) << getRelativeObjectiveGap();

		osResult->setAnOtherSolutionResult(numPrimalSols - 1, "RelOptimalityGap", strstr3.str(), "Final solution",
				"The relative optimality gap", 0, NULL);
	}

	for (auto T : timers)
	{
		osResult->addTimingInformation(T.name, "SHOT", "second", T.description, T.elapsed());
	}

	numPrimalSols = max(1, numPrimalSols); // To make sure we also print the following even if we have no primal solution

	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "LP", std::to_string(iterLP), "ProblemsSolved",
			"Relaxed LP problems solved", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "QP", std::to_string(iterQP), "ProblemsSolved",
			"Relaxed QP problems solved", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "FeasibleMILP", std::to_string(iterFeasMILP),
			"ProblemsSolved", "MILP problems solved to feasibility", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "OptimalMILP", std::to_string(iterOptMILP), "ProblemsSolved",
			"MILP problems solved to optimality", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "FeasibleMIQP", std::to_string(iterFeasMIQP),
			"ProblemsSolved", "MIQP problems solved to feasibility", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "OptimalMIQP", std::to_string(iterOptMIQP), "ProblemsSolved",
			"MIQP problems solved to optimality", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "Total",
			std::to_string(iterLP + iterFeasMILP + iterOptMILP + iterQP + iterFeasMIQP + iterOptMIQP), "ProblemsSolved",
			"Total number of (MI)QP/(MI)LP subproblems solved", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "NLP", std::to_string(numNLPProbsSolved), "ProblemsSolved",
			"NLP problems solved", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "Functions", std::to_string(numFunctionEvals), "Evaluations",
			"Total number of function evaluations in SHOT", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "Gradients", std::to_string(numGradientEvals), "Evaluations",
			"Total number of gradient evaluations in SHOT", 0, NULL);

	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "FileName",
			this->originalProblem->getProblemInstance()->getInstanceName(), "Problem", "The original filename", 0,
			NULL);

	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "NumberVariables",
			std::to_string(this->originalProblem->getProblemInstance()->getVariableNumber()), "Problem",
			"Total number of variables", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "NumberContinousVariables",
			std::to_string(
					this->originalProblem->getProblemInstance()->getVariableNumber()
							- this->originalProblem->getNumberOfIntegerVariables()
							- this->originalProblem->getNumberOfBinaryVariables()), "Problem",
			"Number of continuous variables", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "NumberBinaryVariables",
			std::to_string(this->originalProblem->getProblemInstance()->getNumberOfBinaryVariables()), "Problem",
			"Number of binary variables", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "NumberIntegerVariables",
			std::to_string(this->originalProblem->getProblemInstance()->getNumberOfIntegerVariables()), "Problem",
			"Number of integer variables", 0, NULL);

	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "NumberConstraints",
			std::to_string(this->originalProblem->getProblemInstance()->getConstraintNumber()), "Problem",
			"Number of constraints", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "NumberNonlinearConstraints",
			std::to_string(this->originalProblem->getProblemInstance()->getNumberOfNonlinearConstraints()), "Problem",
			"Number of nonlinear constraints", 0, NULL);
	osResult->setAnOtherSolutionResult(numPrimalSols - 1, "NumberLinearConstraints",
			std::to_string(
					this->originalProblem->getProblemInstance()->getConstraintNumber()
							- this->originalProblem->getNumberOfNonlinearConstraints()), "Problem",
			"Number of linear constraints", 0, NULL);

	std::string modelStatus;
	std::string modelDescription;

	if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::Optimal)
	{
		modelStatus = "optimal";
		modelDescription = "Optimal solution found.";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::Feasible)
	{
		modelStatus = "feasible";
		modelDescription = "Feasible solution found.";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::Unbounded)
	{
		modelStatus = "unbounded";
		modelDescription = "Problem is unbounded.";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::Error)
	{
		modelStatus = "error";
		modelDescription = "Error occured.";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::Infeasible)
	{
		modelStatus = "infeasible";
		modelDescription = "Problem is infeasible.";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::IterationLimit)
	{
		modelStatus = "feasible";
		modelDescription = "Termination due to iteration limit.";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::SolutionLimit)
	{
		modelStatus = "feasible";
		modelDescription = "";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::TimeLimit)
	{
		modelStatus = "feasible";
		modelDescription = "Termination due to time limit.";
	}
	else
	{
		modelStatus = "NA";
	}

	osResult->setSolutionStatusType(numPrimalSols - 1, modelStatus);

	OSrLWriter writer;

	writer.m_bWhiteSpace = false;

	using boost::property_tree::ptree;
	ptree pt;
	boost::property_tree::xml_writer_settings < std::string > settings('\t', 1);

	stringstream ss;
	ss << writer.writeOSrL(osResult);

	read_xml(ss, pt, boost::property_tree::xml_parser::trim_whitespace);

	std::ostringstream oss;
	write_xml(oss, pt, settings);

	return (oss.str());
}

std::string ProcessInfo::getTraceResult()
{
	std::stringstream ss;

	this->originalProblem->getProblemInstance()->initializeNonLinearStructures();

	if (this->originalProblem->getProblemInstance()->getNumberOfQuadraticTerms() > 0)
	{
		this->originalProblem->getProblemInstance()->addQTermsToExpressionTree();
	}

	this->originalProblem->getProblemInstance()->bVariablesModified = true;
	this->originalProblem->getProblemInstance()->bConstraintsModified = true;
	this->originalProblem->getProblemInstance()->bObjectivesModified = true;

	ss << this->originalProblem->getProblemInstance()->getInstanceName() << ",";
	ss << "MINLP" << ",";
	ss << "SHOT" << ",";
	ss << "IPOpt" << ",";
	ss << "Cplex" << ",";
	ss << "9999999" << ",";
	ss << (originalProblem->getProblemInstance()->getObjectiveMaxOrMins()[0] == "min" ? "0" : "1") << ",";
	ss << this->originalProblem->getProblemInstance()->getConstraintNumber() + 1 << ","; // +1 to comply with GAMS objective style
	ss << this->originalProblem->getProblemInstance()->getVariableNumber() + 1 << ","; // +1 to comply with GAMS objective style
	ss << this->originalProblem->getNumberOfBinaryVariables() + this->originalProblem->getNumberOfIntegerVariables()
			<< ",";

	auto nonzeroes = this->originalProblem->getProblemInstance()->getLinearConstraintCoefficientNumber()
			+ this->originalProblem->getProblemInstance()->getAllNonlinearVariablesIndexMap().size() + 1;
//this->originalProblem->getObjectiveCoefficientNumbers()[0] + 1;

	ss << nonzeroes << ",";
	ss << this->originalProblem->getProblemInstance()->getAllNonlinearVariablesIndexMap().size() << ",";
	ss << "1" << ",";

	std::string solverStatus = "";
	std::string modelStatus = "";

	auto t = this->getCurrentIteration()->solutionStatus;

	if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::Optimal)
	{
		modelStatus = "8";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::Feasible)
	{
		modelStatus = "7";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::Unbounded)
	{
		modelStatus = "18";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::Error)
	{
		modelStatus = "12";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::Infeasible)
	{
		modelStatus = "4";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::IterationLimit)
	{
		modelStatus = "7";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::SolutionLimit)
	{
		modelStatus = "7";
	}
	else if (this->getCurrentIteration()->solutionStatus == E_ProblemSolutionStatus::TimeLimit)
	{
		modelStatus = "7";
	}
	else
	{
		modelStatus = "NA";
		outputError("Unknown return code from model solution.");
	}

	if (this->terminationReason == E_TerminationReason::TimeLimit)
	{
		solverStatus = "3";
	}
	else if (this->terminationReason == E_TerminationReason::IterationLimit)
	{
		solverStatus = "2";
	}
	else if (this->terminationReason == E_TerminationReason::ObjectiveStagnation)
	{
		solverStatus = "2";
	}
	else if (this->terminationReason == E_TerminationReason::Error)
	{
		solverStatus = "10";
	}
	else if (this->terminationReason == E_TerminationReason::InfeasibleProblem)
	{
		solverStatus = "4";
	}
	else if (this->terminationReason == E_TerminationReason::ConstraintTolerance
			|| this->terminationReason == E_TerminationReason::AbsoluteGap
			|| this->terminationReason == E_TerminationReason::RelativeGap)
	{
		solverStatus = "1";
	}
	else
	{
		solverStatus = "10";
		outputError("Unknown return code obtainedfrom solver.");
	}

	ss << modelStatus << ",";
	ss << solverStatus << ",";
	ss << std::setprecision(std::numeric_limits<double>::digits10 + 1);
//ss << this->getCurrentIteration()->objectiveValue << ",";
	ss << this->getPrimalBound() << ",";
	;
	ss << this->getDualBound() << ",";
	;
	ss << this->getElapsedTime("Total") << ",";
	ss << iterFeasMILP + iterOptMILP << ",";
	ss << "0" << ",";
	ss << "0" << ",";
	ss << "#";

	return (ss.str());
}

void ProcessInfo::createIteration()
{
	Iteration iter = Iteration();
	iter.iterationNumber = iterations.size() + 1;

	iter.numHyperplanesAdded = 0;

	if (iterations.size() == 0) iter.totNumHyperplanes = 0;
	else iter.totNumHyperplanes = iterations.at(iterations.size() - 1).totNumHyperplanes;

	iter.maxDeviation = OSDBL_MAX;
	iter.boundaryDistance = OSDBL_MAX;
	iter.MILPSolutionLimitUpdated = false;

	iter.type = relaxationStrategy->getProblemType();
//getCurrentIteration()->type = relaxationStrategy->getProblemType();

	iterations.push_back(iter);

}

Iteration * ProcessInfo::getCurrentIteration()
{
	return (&iterations.back());
}

Iteration * ProcessInfo::getPreviousIteration()
{
	if (iterations.size() > 1) return (&(iterations[iterations.size() - 2]));
	else throw ErrorClass("Only one iteration!");
}

double ProcessInfo::getPrimalBound()
{
	auto primalBound = this->currentObjectiveBounds.second;

	return (primalBound);
}

double ProcessInfo::getDualBound()
{
	auto dualBound = this->currentObjectiveBounds.first;

	return (dualBound);
}

double ProcessInfo::getAbsoluteObjectiveGap()
{
	double gap = abs(getDualBound() - getPrimalBound());

	return (gap);
}

double ProcessInfo::getRelativeObjectiveGap()
{
	double gap = abs(getDualBound() - getPrimalBound()) / ((1e-10) + abs(getPrimalBound()));

	return (gap);
}
