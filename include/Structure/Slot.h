#ifndef SLOT_H
#define SLOT_H

#include <memory>

class Link;

/**
 * @brief The Slot class represents a frequency slot.
 */
class Slot
{
public:
    /**
     * @brief BSlot is the bandwidth of a single slot.
     */
    constexpr static double BSlot = 12.5E9;

    /**
     * @brief Slot is the standard constructor of a slot.
     */
    Slot(int);
    /**
     * @brief Slot is the copy constructor of a slot.
     */
    Slot(const Slot &slot);

    Slot &operator= (const Slot &slot);

    /**
     * @brief numSlot identifies the slot in the link.
     */
    int numSlot;
    /**
     * @brief isFree is true, iff the slot is free.
     */
    bool isFree;

    /**
     * @brief freeSlot frees this slot, if it isn't free.
     */
    void freeSlot();
    /**
     * @brief useSlot uses this slot, if it isn't used.
     */
    void useSlot();
};

#endif // SLOT_H
