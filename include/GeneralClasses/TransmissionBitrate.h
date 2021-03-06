#ifndef TRANSMISSIONBITRATE_H
#define TRANSMISSIONBITRATE_H

#include <vector>
#include <ostream>

/**
 * @brief The TransmissionBitrate class represents a transmission bitrate.
 */
class TransmissionBitrate
{
public:
/**
 * In DEFAULT_TRANSMISSIONBITRATES there are the default transmission bitrates
 * used through the simulator. To add a new bitrate, add to the list X(BR), where
 * BR is measured in bits-per-second.
 **/
#define DEFAULT_TRANSMISSIONBITRATES \
  X(10E9) \
  X(40E9) \
  X(100E9) \
  X(160E9) \
  X(400E9)

    /**
     * @brief TransmissionBitrate is the standard constructor for a TransmissionBitrate.
     * @param Bitrate is the bitrate, in bits per second.
     */
    TransmissionBitrate(double Bitrate);
    /**
     * @brief TransmissionBitrate is the copy constructor.
     */
    TransmissionBitrate(const TransmissionBitrate &);
    /**
     * @brief get_Bitrate returns the value of the bitrate, in bits per second.
     * @return the value of the bitrate, in bits per second.
     */
    double get_Bitrate() const;

    bool operator ==(const TransmissionBitrate &) const;
    bool operator !=(const TransmissionBitrate &) const;
    bool operator <(const TransmissionBitrate &) const;
    bool operator <=(const TransmissionBitrate &) const;
    bool operator >(const TransmissionBitrate &) const;
    bool operator >=(const TransmissionBitrate &) const;

    /**
     * @brief DefaultBitrates is a vector containing the default bitrates considered
     * through the simulator.
     */
    static std::vector<TransmissionBitrate> DefaultBitrates;

    friend std::ostream& operator <<(std::ostream &out, const TransmissionBitrate &br);
private:
    double Bitrate;
};

#endif // TRANSMISSIONBITRATE_H

