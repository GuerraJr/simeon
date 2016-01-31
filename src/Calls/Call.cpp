#include <Calls/Call.h>
#include <Calls/Event.h>

Call::Call(std::weak_ptr<Node> Origin,
           std::weak_ptr<Node> Destination,
           TransmissionBitrate Bitrate)  :
    Origin(Origin),
    Destination(Destination),
    Bitrate(Bitrate),
    Status(Not_Evaluated),
    Scheme(ModulationScheme(0, Gain(0)))
{

}
