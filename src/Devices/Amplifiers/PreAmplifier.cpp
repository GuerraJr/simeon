#include <Devices/Amplifiers/PreAmplifier.h>
#include <Devices/Fiber.h>
#include <Devices/SSS.h>
#include <Structure/Node.h>

PreAmplifier::PreAmplifier(Fiber &Segment,
                           Node &Destination) : Amplifier(Gain(0)) , Destination(Destination) {
    numPorts = Destination.Links.size();

    if (Destination.get_NodeArch() == Node::SwitchingSelect) {
        set_Gain(-SSS::SSSLoss - Segment.get_Loss());
    } else {
        set_Gain(-SSS::SSSLoss - Gain(numPorts + 1, Gain::Linear));
    }
}

Gain PreAmplifier::get_Gain() {
    if (Destination.get_NodeArch() == Node::BroadcastAndSelect &&
            Destination.Links.size() != numPorts)  {
        numPorts = Destination.Links.size();
        set_Gain(-SSS::SSSLoss - Gain(numPorts + 1, Gain::Linear));
    }

    return AmplifierGain;
}
