#pragma once
#include "../Enums.h"
#include "IMILPSolver.h"
#include "gurobi_c++.h"
#include "MILPSolverBase.h"
class MILPSolverGurobi: public IMILPSolver, MILPSolverBase
{
	public:
		MILPSolverGurobi();
		virtual ~MILPSolverGurobi();

		virtual void checkParameters();

		virtual bool createLinearProblem(OptProblem *origProblem);
		virtual void initializeSolverSettings();
		virtual void writeProblemToFile(std::string filename);

		virtual int addLinearConstraint(std::vector<IndexValuePair> elements, double constant)
		{
			return (addLinearConstraint(elements, constant, false));
		}
		virtual int addLinearConstraint(std::vector<IndexValuePair> elements, double constant, bool isGreaterThan);
		virtual void changeConstraintToLazy(GeneratedHyperplane &hyperplane);
		virtual void createHyperplane(Hyperplane hyperplane)
		{
			MILPSolverBase::createHyperplane(hyperplane);
		}
		virtual void createInteriorHyperplane(Hyperplane hyperplane)
		{
			MILPSolverBase::createInteriorHyperplane(hyperplane);
		}

		virtual void fixVariable(int varIndex, double value);
		virtual void updateVariableBound(int varIndex, double lowerBound, double upperBound);
		virtual pair<double, double> getCurrentVariableBounds(int varIndex);

		virtual void activateDiscreteVariables(bool activate);
		virtual bool getDiscreteVariableStatus()
		{
			return (MILPSolverBase::getDiscreteVariableStatus());
		}

		virtual E_ProblemSolutionStatus solveProblem();
		virtual E_ProblemSolutionStatus getSolutionStatus();
		virtual int getNumberOfSolutions();
		virtual std::vector<double> getVariableSolution(int solIdx);
		virtual std::vector<SolutionPoint> getAllVariableSolutions()
		{
			return (MILPSolverBase::getAllVariableSolutions());
		}
		virtual double getDualObjectiveValue();
		virtual double getObjectiveValue(int solIdx);
		virtual double getObjectiveValue()
		{
			return (MILPSolverBase::getObjectiveValue());
		}

		virtual int increaseSolutionLimit(int increment);
		virtual void setSolutionLimit(int limit);
		virtual int getSolutionLimit();

		virtual void setTimeLimit(double seconds);

		virtual void setCutOff(double cutOff);

		virtual void addMIPStart(std::vector<double> point);
		virtual void deleteMIPStarts();

		virtual void populateSolutionPool();

		virtual bool supportsQuadraticObjective();
		virtual bool supportsQuadraticConstraints();
		virtual bool supportsLazyConstraints();

		virtual std::vector<GeneratedHyperplane>* getGeneratedHyperplanes()
		{
			return (MILPSolverBase::getGeneratedHyperplanes());
		}

	private:
		GRBEnv *gurobiEnv;
		GRBModel *gurobiModel;
};
