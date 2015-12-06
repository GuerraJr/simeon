#ifndef CALL_H
#define CALL_H

#include <memory>
#include <Calls/Event.h>
#include <GeneralClasses/TransmissionBitrate.h>

class Node;

/**
 * @brief The Call class is the standard object representing a connection
 * between two nodes.
 */
class Call {
  public:
    /**
    * @brief Call is the standard constructor for a Call object.
    * @param tOcurr is the instant when the call occurs.
    * @param tDur is the duration of the call.
    * @param Origin is a weak_ptr for the Origin node.
    * @param Destination is a weak_ptr for the Destination node.
    * @param Bitrate is a TransmissionBitrate.
    */
    Call(long double tOcurr, long double tDur, std::weak_ptr<Node> Origin,
         std::weak_ptr<Node> Destination, TransmissionBitrate Bitrate = 0);

    std::shared_ptr<Event> CallRequisition;
    std::shared_ptr<Event> CallEnding;
    std::weak_ptr<Node> Origin;
    std::weak_ptr<Node> Destination;
    TransmissionBitrate Bitrate;
};

#endif // CALL_H
