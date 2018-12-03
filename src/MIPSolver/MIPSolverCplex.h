/**
   The Supporting Hyperplane Optimization Toolkit (SHOT).

   @author Andreas Lundell, Åbo Akademi University

   @section LICENSE 
   This software is licensed under the Eclipse Public License 2.0. 
   Please see the README and LICENSE files for more information.
*/

#pragma once
#include "IMIPSolver.h"
#include "MIPSolverBase.h"
#include <mutex>

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#include "ilcplex/ilocplex.h"
#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wignored-attributes"
#endif

namespace SHOT
{
class MIPSolverCplex : public IMIPSolver, public MIPSolverBase
{
  public:
    MIPSolverCplex();
    MIPSolverCplex(EnvironmentPtr envPtr);
    virtual ~MIPSolverCplex();

    virtual void checkParameters();

    virtual bool createLinearProblem(OptProblem *origProblem);
    virtual bool createLinearProblem(ProblemPtr sourceProblem);

    virtual void addVariable(int index, std::string name, E_VariableType type, double lowerBound, double upperBound);

    virtual void initializeObjective();
    virtual void addLinearTermToObjective(double coefficient, int variableIndex);
    virtual void addQuadraticTermToObjective(double coefficient, int firstVariableIndex, int secondVariableIndex);
    virtual void finalizeObjective(bool isMinimize, double constant = 0.0);

    virtual void initializeConstraint();
    virtual void addLinearTermToConstraint(double coefficient, int variableIndex);
    virtual void addQuadraticTermToConstraint(double coefficient, int firstVariableIndex, int secondVariableIndex);
    virtual void finalizeConstraint(std::string name, double valueLHS, double valueRHS, double constant = 0.0);

    virtual void initializeSolverSettings();

    virtual void writeProblemToFile(std::string filename);
    virtual void writePresolvedToFile(std::string filename);

    virtual int addLinearConstraint(std::vector<PairIndexValue> elements, double constant)
    {
        return (addLinearConstraint(elements, constant, false));
    }
    virtual int addLinearConstraint(std::vector<PairIndexValue> elements, double constant, bool isGreaterThan);

    virtual void createHyperplane(Hyperplane hyperplane)
    {
        MIPSolverBase::createHyperplane(hyperplane);
    }

    virtual void createIntegerCut(VectorInteger binaryIndexes)
    {
        MIPSolverBase::createIntegerCut(binaryIndexes);
    }

    virtual void createIntegerCut(VectorInteger binaryIndexes,
                                  std::function<IloConstraint(IloRange)> addConstraintFunction);

    virtual void createHyperplane(Hyperplane hyperplane,
                                  std::function<IloConstraint(IloRange)> addConstraintFunction);

    virtual void createInteriorHyperplane(Hyperplane hyperplane)
    {
        MIPSolverBase::createInteriorHyperplane(hyperplane);
    }

    virtual boost::optional<std::pair<std::vector<PairIndexValue>, double>> createHyperplaneTerms(
        Hyperplane hyperplane)
    {
        return (MIPSolverBase::createHyperplaneTerms(hyperplane));
    }

    virtual void fixVariable(int varIndex, double value);

    virtual void fixVariables(VectorInteger variableIndexes, VectorDouble variableValues)
    {
        MIPSolverBase::fixVariables(variableIndexes, variableValues);
    }

    virtual void unfixVariables()
    {
        MIPSolverBase::unfixVariables();
    }

    virtual void updateVariableBound(int varIndex, double lowerBound, double upperBound);
    virtual PairDouble getCurrentVariableBounds(int varIndex);

    virtual void presolveAndUpdateBounds()
    {
        return (MIPSolverBase::presolveAndUpdateBounds());
    }

    virtual std::pair<VectorDouble, VectorDouble> presolveAndGetNewBounds();

    virtual void activateDiscreteVariables(bool activate);
    virtual bool getDiscreteVariableStatus()
    {
        return (MIPSolverBase::getDiscreteVariableStatus());
    }

    virtual E_ProblemSolutionStatus solveProblem();
    virtual E_ProblemSolutionStatus getSolutionStatus();
    virtual int getNumberOfSolutions();
    virtual VectorDouble getVariableSolution(int solIdx);
    virtual std::vector<SolutionPoint> getAllVariableSolutions()
    {
        return (MIPSolverBase::getAllVariableSolutions());
    }
    virtual double getDualObjectiveValue();
    virtual double getObjectiveValue(int solIdx);
    virtual double getObjectiveValue()
    {
        return (MIPSolverBase::getObjectiveValue());
    }

    virtual int increaseSolutionLimit(int increment);
    virtual void setSolutionLimit(long limit);
    virtual int getSolutionLimit();

    virtual void setTimeLimit(double seconds);

    virtual void setCutOff(double cutOff);

    virtual void addMIPStart(VectorDouble point);
    virtual void deleteMIPStarts();

    virtual bool supportsQuadraticObjective();
    virtual bool supportsQuadraticConstraints();

    virtual std::vector<GeneratedHyperplane> *getGeneratedHyperplanes()
    {
        return (MIPSolverBase::getGeneratedHyperplanes());
    }

    virtual void updateNonlinearObjectiveFromPrimalDualBounds()
    {
        return (MIPSolverBase::updateNonlinearObjectiveFromPrimalDualBounds());
    }

    virtual int getNumberOfExploredNodes();
    virtual int getNumberOfOpenNodes();

    IloModel cplexModel;
    IloCplex cplexInstance;

  protected:
    IloEnv cplexEnv;

    IloNumVarArray cplexVars;
    IloRangeArray cplexConstrs;
    std::vector<IloConversion> cplexVarConvers;

    IloExpr objExpression;
    IloExpr constrExpression;

    int prevSolutionLimit = 1;

    bool modelUpdated /*= true*/;
};
} // namespace SHOT