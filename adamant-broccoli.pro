TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle qt

SOURCES += src/main.cpp \
    src/GeneralClasses/TransmissionBitrate.cpp \
    src/Devices/Fiber.cpp \
    src/Devices/SSS.cpp \
    src/Devices/Amplifier.cpp \
    src/GeneralClasses/Gain.cpp \
    src/GeneralClasses/Power.cpp \
    src/Devices/Device.cpp \
    src/Devices/Splitter.cpp \
    src/Structure/Slot.cpp \
    src/Structure/Link.cpp \
    src/Structure/Node.cpp \
    src/ModulationSchemes/ModulationScheme.cpp \
    src/GeneralClasses/Signal.cpp


HEADERS += \
    include/GeneralClasses/TransmissionBitrate.h \
    include/Devices/Device.h \
    include/Devices/Fiber.h \
    include/Devices/SSS.h \
    include/Devices/Amplifier.h \
    include/GeneralClasses/Gain.h \
    include/GeneralClasses/Power.h \
    include/GeneralClasses/PhysicalConstants.h \
    include/Devices/Splitter.h \
    include/Structure/Slot.h \
    include/Structure/Link.h \
    include/Structure/Node.h \
    include/ModulationSchemes/ModulationScheme.h \
    include/GeneralClasses/Signal.h

LIBS += -lboost_system

OTHER_FILES += README.md \
               .astylerc


INCLUDEPATH += include/
