#include "include/GeneralPurposeAlgorithms/GA/GA.h"
#include "include/GeneralPurposeAlgorithms/GA/GA_Individual.h"
#include "include/GeneralPurposeAlgorithms/GA/GA_Generation.h"
#include "include/GeneralClasses/RandomGenerator.h"
#include <algorithm>

using namespace GeneticAlgorithm;

GA::GA() : generation(0)
{

}

void GA::run_Generation()
{
    if (evolution.empty())
        {
        createInitialGeneration();
        }

    evolution.back()->eval();

    auto currentgeneration = evolution.back();
    (*currentgeneration) += (newGeneration(currentgeneration));
    currentgeneration->eval();

    std::shared_ptr<GA_Generation> newGen = createEmptyGeneration();
    natural_selection(currentgeneration, newGen);
    evolution.push_back(newGen);
    ++generation;
}

void GA::natural_selection(std::shared_ptr<GA_Generation> gen,
                           std::shared_ptr<GA_Generation> dest)
{
    dest->people.clear();
    std::sort(gen->people.begin(), gen->people.end(),
              [](const std::shared_ptr<GA_Individual> &l,
                 const std::shared_ptr<GA_Individual> &r)
        {
        return *l < *r;
        });

    for (size_t i = 0; i < GA::alphaIndiv; ++i)
        {
        auto indiv = gen->people.begin();
        std::advance(indiv, i);
        dest->people.push_back((*indiv)->clone());
        }

    while (dest->people.size() < numIndiv)
        {
        std::uniform_int_distribution<> dist(0, gen->people.size() - 1);

        //selects random Individual
        auto individual = gen->people.begin();
        std::advance(individual, dist(random_generator));

        dest->people.push_back((*individual)->clone());
        }
}

std::shared_ptr<GA_Generation> GA::newGeneration
(std::shared_ptr<GA_Generation> prnt)
{
    std::shared_ptr<GA_Generation> gen_r = createEmptyGeneration();
    std::shared_ptr<GA_Generation> gen_q = createEmptyGeneration();

    natural_selection(prnt, gen_r);

    while (!gen_r->people.empty())
        {
        if (gen_r->people.size() == 2)
            {
            gen_r->breed(0, 1, gen_q);
            }
        else
            {
            std::uniform_int_distribution<> dist(0, gen_r->people.size() - 1);
            gen_r->breed(dist(random_generator), dist(random_generator), gen_q);
            }
        }

    for (auto indiv : gen_q->people)
        {
        indiv->mutate();
        }

    return gen_q;
}
