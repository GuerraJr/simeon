TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle qt

QMAKE_CXXFLAGS += -std=c++11 -fopenmp
QMAKE_LFLAGS += -std=c++11 -fopenmp
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

SOURCES += src/main.cpp \
    src/Calls/Call.cpp \
    src/Calls/CallGenerator.cpp \
    src/Calls/Event.cpp \
    src/Devices/Amplifiers/Amplifier.cpp \
    src/Devices/Amplifiers/BoosterAmplifier.cpp \
    src/Devices/Amplifiers/EDFA.cpp \
    src/Devices/Amplifiers/InLineAmplifier.cpp \
    src/Devices/Amplifiers/PreAmplifier.cpp \
    src/Devices/Device.cpp \
    src/Devices/Fiber.cpp \
    src/Devices/Regenerator.cpp \
    src/Devices/Splitter.cpp \
    src/Devices/SSS.cpp \
    src/GeneralClasses/Gain.cpp \
    src/GeneralClasses/ModulationScheme.cpp \
    src/GeneralClasses/Power.cpp \
    src/GeneralClasses/Signal.cpp \
    src/GeneralClasses/SpectralDensity.cpp \
    src/GeneralClasses/TransferFunctions/GaussianTransferFunction.cpp \
    src/GeneralClasses/TransferFunctions/TransferFunction.cpp \
    src/GeneralClasses/TransmissionBitrate.cpp \
    src/GeneralPurposeAlgorithms/IntegrationMethods/IntegrationMethod.cpp \
    src/GeneralPurposeAlgorithms/IntegrationMethods/SimpsonsRule.cpp \
    src/GeneralPurposeAlgorithms/IntegrationMethods/TrapezoidalRule.cpp \
    src/GeneralPurposeAlgorithms/NSGA-II/NSGA2.cpp \
    src/GeneralPurposeAlgorithms/NSGA-II/NSGA2_Generation.cpp \
    src/GeneralPurposeAlgorithms/NSGA-II/NSGA2_Individual.cpp \
    src/GeneralPurposeAlgorithms/NSGA-II/NSGA2_Parameter.cpp \
    src/Structure/Link.cpp \
    src/Structure/Node.cpp \
    src/Structure/Slot.cpp \
    src/Structure/Topology.cpp \
    src/RMSA/RegeneratorAssignmentAlgorithms/FirstLongestReach.cpp \
    src/RMSA/RegeneratorAssignmentAlgorithms/FirstNarrowestSpectrum.cpp \
    src/RMSA/RegeneratorAssignmentAlgorithms/RegeneratorAssignmentAlgorithm.cpp \
    src/RMSA/RegeneratorPlacementAlgorithms/DistanceAdaptative.cpp \
    src/RMSA/RegeneratorPlacementAlgorithms/Empty_RegeneratorPlacementAlgorithm.cpp \
    src/RMSA/RegeneratorPlacementAlgorithms/MostSimultaneouslyUsed.cpp \
    src/RMSA/RegeneratorPlacementAlgorithms/MostUsed.cpp \
    src/RMSA/RegeneratorPlacementAlgorithms/NodalDegreeFirst.cpp \
    src/RMSA/RegeneratorPlacementAlgorithms/NX_MostSimultaneouslyUsed.cpp \
    src/RMSA/RegeneratorPlacementAlgorithms/NX_RegeneratorPlacement.cpp \
    src/RMSA/RegeneratorPlacementAlgorithms/RegeneratorPlacementAlgorithm.cpp \
    src/RMSA/RegeneratorPlacementAlgorithms/SignalQualityPrediction/SignalQualityPrediction.cpp \
    src/RMSA/RegeneratorPlacementAlgorithms/SignalQualityPrediction/SignalQualityPrediction_Variants.cpp \
    src/RMSA/RegeneratorPlacementAlgorithms/SignalQualityPrediction/SQP_NetworkSimulation.cpp \
    src/RMSA/Route.cpp \
    src/RMSA/RoutingAlgorithms/DijkstraRoutingAlgorithm.cpp \
    src/RMSA/RoutingAlgorithms/LengthOccupationRoutingAvailability.cpp \
    src/RMSA/RoutingAlgorithms/LengthOccupationRoutingContiguity.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/PSRVariants/AdaptativeWeighingRouting.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/PSRVariants/LocalPowerSeriesRouting.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/PSRVariants/MatricialPowerSeriesRouting.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/PSRVariants/TensorialPowerSeriesRouting.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Availability.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Bitrate.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Contiguity.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Distance.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_HopDistance.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_LinkLength.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_ModulationScheme.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Noise.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_NormalizedContiguity.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_NormalizedNoise.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Occupability.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_OrigDestIndex.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Slots.cpp \
    src/RMSA/RoutingAlgorithms/PowerSeriesRouting/PowerSeriesRouting.cpp \
    src/RMSA/RoutingAlgorithms/RoutingAlgorithm.cpp \
    src/RMSA/RoutingAlgorithms/StaticRouting/MinimumHops.cpp \
    src/RMSA/RoutingAlgorithms/StaticRouting/ShortestPath.cpp \
    src/RMSA/RoutingAlgorithms/StaticRouting/StaticRoutingAlgorithm.cpp \
    src/RMSA/RoutingWavelengthAssignment.cpp \
    src/RMSA/SpectrumAssignmentAlgorithms/BestFit.cpp \
    src/RMSA/SpectrumAssignmentAlgorithms/ExactFit.cpp \
    src/RMSA/SpectrumAssignmentAlgorithms/FirstFit.cpp \
    src/RMSA/SpectrumAssignmentAlgorithms/LeastUsed.cpp \
    src/RMSA/SpectrumAssignmentAlgorithms/MostUsedSA.cpp \
    src/RMSA/SpectrumAssignmentAlgorithms/RandomFit.cpp \
    src/RMSA/SpectrumAssignmentAlgorithms/SpectrumAssignmentAlgorithm.cpp \
    src/RMSA/TransparentSegment.cpp \
    src/SimulationTypes/NetworkSimulation.cpp \
    src/SimulationTypes/SimulationType.cpp \
    src/SimulationTypes/Simulation_NetworkLoad.cpp \
    src/SimulationTypes/Simulation_NSGA2_RegnPlac/Simulation_NSGA2_RegnPlac.cpp \
    src/SimulationTypes/Simulation_PSROptimization.cpp \
    src/SimulationTypes/Simulation_RegeneratorNumber.cpp \
    src/SimulationTypes/Simulation_StatisticalTrend.cpp \
    src/SimulationTypes/Simulation_TransparencyAnalysis.cpp \
    src/SimulationTypes/Simulation_NSGA2_RegnPlac/NSGA2_Parameter_CapEx.cpp \
    src/SimulationTypes/Simulation_NSGA2_RegnPlac/NSGA2_Parameter_OpEx.cpp \
    src/SimulationTypes/Simulation_NSGA2_RegnPlac/NSGA2_Parameter_BlockingProbability.cpp \
    src/SimulationTypes/Simulation_NSGA2_RegnPlac/NSGA2_Parameter_NumberOfRegenerators.cpp

HEADERS += \
    include/Calls.h \
    include/Calls/Call.h \
    include/Calls/CallGenerator.h \
    include/Calls/Event.h \
    include/Devices.h \
    include/Devices/Amplifiers.h \
    include/Devices/Amplifiers/Amplifier.h \
    include/Devices/Amplifiers/BoosterAmplifier.h \
    include/Devices/Amplifiers/EDFA.h \
    include/Devices/Amplifiers/InLineAmplifier.h \
    include/Devices/Amplifiers/PreAmplifier.h \
    include/Devices/Device.h \
    include/Devices/Fiber.h \
    include/Devices/Regenerator.h \
    include/Devices/SSS.h \
    include/Devices/Splitter.h \
    include/GeneralClasses.h \
    include/GeneralClasses/Gain.h \
    include/GeneralClasses/ModulationScheme.h \
    include/GeneralClasses/PhysicalConstants.h \
    include/GeneralClasses/Power.h \
    include/GeneralClasses/RandomGenerator.h \
    include/GeneralClasses/Signal.h \
    include/GeneralClasses/SpectralDensity.h \
    include/GeneralClasses/TransferFunctions/GaussianTransferFunction.h \
    include/GeneralClasses/TransferFunctions/TransferFunction.h \
    include/GeneralClasses/TransmissionBitrate.h \
    include/GeneralPurposeAlgorithms/IntegrationMethods/IntegrationMethod.h \
    include/GeneralPurposeAlgorithms/IntegrationMethods/SimpsonsRule.h \
    include/GeneralPurposeAlgorithms/IntegrationMethods/TrapezoidalRule.h \
    include/GeneralPurposeAlgorithms/NSGA-2.h \
    include/GeneralPurposeAlgorithms/NSGA-II/NSGA2.h \
    include/GeneralPurposeAlgorithms/NSGA-II/NSGA2_Generation.h \
    include/GeneralPurposeAlgorithms/NSGA-II/NSGA2_Individual.h \
    include/GeneralPurposeAlgorithms/NSGA-II/NSGA2_Parameter.h \
    include/GeneralPurposeAlgorithms/PSO.h \
    include/GeneralPurposeAlgorithms/PSO/ParticleSwarmOptimization.h \
    include/GeneralPurposeAlgorithms/PSO/PSO_Particle.h \
    include/Structure.h \
    include/Structure/Link.h \
    include/Structure/Node.h \
    include/Structure/Slot.h \
    include/Structure/Topology.h \
    include/RMSA.h \
    include/RMSA/RegeneratorAssignmentAlgorithms.h \
    include/RMSA/RegeneratorAssignmentAlgorithms/FirstLongestReach.h \
    include/RMSA/RegeneratorAssignmentAlgorithms/FirstNarrowestSpectrum.h \
    include/RMSA/RegeneratorAssignmentAlgorithms/RegeneratorAssignmentAlgorithm.h \
    include/RMSA/RegeneratorPlacementAlgorithms.h \
    include/RMSA/RegeneratorPlacementAlgorithms/DistanceAdaptative.h \
    include/RMSA/RegeneratorPlacementAlgorithms/Empty_RegeneratorPlacementAlgorithm.h \
    include/RMSA/RegeneratorPlacementAlgorithms/MostSimultaneouslyUsed.h \
    include/RMSA/RegeneratorPlacementAlgorithms/MostUsed.h \
    include/RMSA/RegeneratorPlacementAlgorithms/NodalDegreeFirst.h \
    include/RMSA/RegeneratorPlacementAlgorithms/NX_MostSimultaneouslyUsed.h \
    include/RMSA/RegeneratorPlacementAlgorithms/NX_RegeneratorPlacement.h \
    include/RMSA/RegeneratorPlacementAlgorithms/RegeneratorPlacementAlgorithm.h \
    include/RMSA/RegeneratorPlacementAlgorithms/SignalQualityPrediction/SignalQualityPrediction.h \
    include/RMSA/RegeneratorPlacementAlgorithms/SignalQualityPrediction/SignalQualityPrediction_Variants.h \
    include/RMSA/RegeneratorPlacementAlgorithms/SignalQualityPrediction/SQP_NetworkSimulation.h \
    include/RMSA/Route.h \
    include/RMSA/RoutingAlgorithms.h \
    include/RMSA/RoutingAlgorithms/DijkstraRoutingAlgorithm.h \
    include/RMSA/RoutingAlgorithms/LengthOccupationRoutingAvailability.h \
    include/RMSA/RoutingAlgorithms/LengthOccupationRoutingContiguity.h \
    include/RMSA/RoutingAlgorithms/RoutingAlgorithm.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/PSRVariants.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/PSRVariants/AdaptativeWeighingRouting.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/PSRVariants/LocalPowerSeriesRouting.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/PSRVariants/MatricialPowerSeriesRouting.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/PSRVariants/TensorialPowerSeriesRouting.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Availability.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Bitrate.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Contiguity.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Distance.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_HopDistance.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_LinkLength.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_ModulationScheme.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Noise.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_NormalizedContiguity.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_NormalizedNoise.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Occupability.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_OrigDestIndex.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost_Slots.h \
    include/RMSA/RoutingAlgorithms/PowerSeriesRouting/PowerSeriesRouting.h \
    include/RMSA/RoutingAlgorithms/StaticRouting/MinimumHops.h \
    include/RMSA/RoutingAlgorithms/StaticRouting/ShortestPath.h \
    include/RMSA/RoutingAlgorithms/StaticRouting/StaticRoutingAlgorithm.h \
    include/RMSA/RoutingWavelengthAssignment.h \
    include/RMSA/SpectrumAssignmentAlgorithms.h \
    include/RMSA/SpectrumAssignmentAlgorithms/BestFit.h \
    include/RMSA/SpectrumAssignmentAlgorithms/ExactFit.h \
    include/RMSA/SpectrumAssignmentAlgorithms/FirstFit.h \
    include/RMSA/SpectrumAssignmentAlgorithms/LeastUsed.h \
    include/RMSA/SpectrumAssignmentAlgorithms/MostUsedSA.h \
    include/RMSA/SpectrumAssignmentAlgorithms/RandomFit.h \
    include/RMSA/SpectrumAssignmentAlgorithms/SpectrumAssignmentAlgorithm.h \
    include/RMSA/TransparentSegment.h \
    include/SimulationTypes.h \
    include/SimulationTypes/NetworkSimulation.h \
    include/SimulationTypes/SimulationType.h \
    include/SimulationTypes/Simulation_NetworkLoad.h \
    include/SimulationTypes/Simulation_NSGA2_RegnPlac/Simulation_NSGA2_RegnPlac.h \
    include/SimulationTypes/Simulation_PSROptimization.h \
    include/SimulationTypes/Simulation_RegeneratorNumber.h \
    include/SimulationTypes/Simulation_StatisticalTrend.h \
    include/SimulationTypes/Simulation_TransparencyAnalysis.h \
    include/SimulationTypes/Simulation_NSGA2_RegnPlac/NSGA2_Parameter_CapEx.h \
    include/SimulationTypes/Simulation_NSGA2_RegnPlac/NSGA2_Parameter_OpEx.h \
    include/SimulationTypes/Simulation_NSGA2_RegnPlac/NSGA2_Parameter_BlockingProbability.h \
    include/SimulationTypes/Simulation_NSGA2_RegnPlac/NSGA2_Parameters.h \
    include/SimulationTypes/Simulation_NSGA2_RegnPlac/NSGA2_Parameter_NumberOfRegenerators.h

LIBS += -lboost_system -lboost_program_options -larmadillo

INSTALL_TOPOLOGIES.path = $$OUT_PWD/data/topologies
INSTALL_TOPOLOGIES.files = data/topologies/*

INSTALLS += INSTALL_TOPOLOGIES

OTHER_FILES += README.md \
               .astylerc \
               .travis.yml


INCLUDEPATH += include/
