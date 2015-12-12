#ifndef SPLITTER_H
#define SPLITTER_H

#include <Devices/Device.h>

/**
 * @brief The Splitter class represents a Splitter. Is only used with the
 * Broadcast-And-Select node architecture.
 */
class Splitter : public Device {
  public:
    /**
     * @brief Splitter is the standard constructor for a splitter.
     * @param NumPorts is the number of ports of the splitter.
     */
    Splitter(int NumPorts);

    Gain &get_Gain();
    Power &get_Noise();
    std::shared_ptr<Device> clone();

    void set_NumPorts(int NumPorts);

  private:
    int NumPorts;
    Gain SplitterLoss;
    Power NoisePower;
};

#endif // SPLITTER_H
