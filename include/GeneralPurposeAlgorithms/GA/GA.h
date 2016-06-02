#ifndef GA_H
#define GA_H

#include <memory>
#include <vector>
#include <map>

//! Contains the simple genetic algorithm
namespace GeneticAlgorithm
{

class GA_Generation;

/**
 * @brief The GA class contains a simple genetic algorithm, similar to the NSGA-II
 * algorithm but simpler in nature, as this algorithm isn't multiobjective.
 */
class GA
{
public:
    /**
     * @brief GA is the standard constructor for the GA class.
     */
    GA();

    /**
     * @brief run_Generation runs a generation.
     *
     * If it's being called by the first time, creates the initial generation.
     * Then, from the last generation, creates a new one (by breeding and mutating),
     * and the best Individuals from those two generations (classified by their
     * parameters and their Pareto Front) are selected to form the next generation.
     */
    void run_Generation();
    /**
     * @brief createInitialGeneration creates numIndiv Individuals to form the
     * first generation.
     */
    virtual void createInitialGeneration() = 0;

    /**
     * @brief mutationPoints is the number of crossover points on the mutation of
     * each individual.
     */
    static constexpr unsigned int mutationPoints = 2;
    /**
     * @brief numGen is the number of generations run by the NSGA2.
     */
    static constexpr unsigned int numGen = 300;
    /**
     * @brief numIndiv is the number of Individuals in each generation.
     */
    static constexpr unsigned int numIndiv = 40;
    /**
     * @brief alpha is the number of fitter individuals that guaranteedly survive
     * to the next generation.
     */
    static constexpr unsigned int alphaIndiv = 10;
    /**
     * @brief binaryTournamentParameter is a parameter to the Binary Tournament,
     * that takes part in the Selection process. Each Individual is tested against
     * binaryTournamentParameter other Individuals.
     */
    static constexpr unsigned int binaryTournamentParameter = 2;
    /**
     * @brief evolution is a vector containing the evolutions of this GA algorithm.
     */
    std::vector<std::shared_ptr<GA_Generation>> evolution;

protected:
    /**
     * @brief natural_selection uses binaryTournament to select numIndiv fit
     * Individuals from gen and copy them into dest.
     * @param gen is the origin Generation.
     * @param dest is the destination Generation.
     */
    void natural_selection(std::shared_ptr<GA_Generation> gen,
                           std::shared_ptr<GA_Generation> dest);

    std::shared_ptr<GA_Generation> newGeneration
    (std::shared_ptr<GA_Generation> prnt);

    unsigned int generation;
};
}

#endif // GA_H
