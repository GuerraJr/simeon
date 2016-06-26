#include <gtest/gtest.h>

#include <SimulationTypes/NetworkSimulation.h>
#include <Structure/Slot.h>
#include <RMSA/Route.h>
#include <Calls.h>
#include <RMSA/RoutingWavelengthAssignment.h>
#include <iostream>
#include <Structure/Link.h>

using namespace Simulations;
using namespace RMSA;

NetworkSimulation::NetworkSimulation(std::shared_ptr<CallGenerator> Generator,
                                     std::shared_ptr<RoutingWavelengthAssignment> RMSA,
                                     unsigned long NumMaxCalls) :
    Generator(Generator), RMSA(RMSA), NumMaxCalls(NumMaxCalls)
{
    NumCalls =
            NumBlockedCalls =
            NumBlockedCalls_Route =
            NumBlockedCalls_Spectrum =
            NumBlockedCalls_ASE_Noise =
            NumBlockedCalls_FilterImperfection = 0;
    hasSimulated = false;
}

void NetworkSimulation::run()
{
    NumCalls++;
    Generator->generate_Call(); //Generates first call

    while (!Generator->Events.empty())
        {
        std::shared_ptr<Event> evt(Generator->Events.top());
        Generator->Events.pop();

        if (evt->Type == Event::CallRequisition)
            {
            implement_call(evt);
            }
        else if (evt->Type == Event::CallEnding)
            {
            drop_call(evt);
            }
        }

    hasSimulated = true;
}

void NetworkSimulation::implement_call(std::shared_ptr<Event> evt)
{
    auto route = RMSA->routeCall(evt->Parent);
    evt->Parent->CallEnding.lock()->route = evt->route = route;

#ifdef RUN_ASSERTIONS
    EXPECT_NE(evt->Parent->Status, Call::Not_Evaluated) <<
            "Call was neither accepted nor blocked.";
#endif

    if (evt->Parent->Status == Call::Blocked)
        {
        NumBlockedCalls_Route += (evt->Parent->blockingReason & Call::Blocking_Route) != 0;
        NumBlockedCalls_ASE_Noise += (evt->Parent->blockingReason & Call::Blocking_ASE_Noise) != 0;
        NumBlockedCalls_FilterImperfection += (evt->Parent->blockingReason & Call::Blocking_FilterImperfection) != 0;
        NumBlockedCalls_Spectrum += (evt->Parent->blockingReason & Call::Blocking_Spectrum) != 0;
        NumBlockedCalls++;
        }
    else
        {
        unsigned int auxCount = 0;
        for (auto &link : route->Slots)
            {

            if(considerFilterImperfection)
                {
                SpectralDensity thisSpecDensity = route->Segments.begin()->opticalPathSpecDensity.at(auxCount);
                link.first.lock()->linkSpecDens->updateLink(thisSpecDensity, link.second);
                }

            for (auto &slot : link.second)
                {
                slot.lock()->useSlot();
                }

            auxCount++;
            }

        for (auto &reg : route->Regenerators)
            {
            reg.first.lock()->request_Regenerators(reg.second);
            }
        }

    if (NumCalls++ < NumMaxCalls)
        {
        Generator->generate_Call();
        }
}

void NetworkSimulation::drop_call(std::shared_ptr<Event> evt)
{
    if (evt->Parent->Status == Call::Implemented)
        {
        for (auto &node : evt->route->Slots)
            {
            for (auto &slot : node.second)
                {
                slot.lock()->freeSlot();
                }
            }

        for (auto &reg : evt->route->Regenerators)
            {
            reg.first.lock()->free_Regenerators(reg.second);
            }
        }
}

void NetworkSimulation::print()
{
    if (!hasSimulated)
        {
        run();
        }

    std::cout << get_Load() << "\t" << get_CallBlockingProbability() << std::endl;
}

double NetworkSimulation::get_CallBlockingProbability()
{
    if (!hasSimulated)
        {
        run();
        }

    return 1.0 * NumBlockedCalls / (1.0 * NumCalls);
}

double NetworkSimulation::get_Load()
{
    return Generator->h;
}
