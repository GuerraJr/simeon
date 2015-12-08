#include <ModulationSchemes/M_QAM.h>

M_QAM::M_QAM(unsigned int M, Gain SNR_Per_Bit) : SNR_Per_Bit(SNR_Per_Bit) {
    this->M = M;
}

unsigned int M_QAM::get_M() {
    return M;
}

Gain M_QAM::get_SNR_Per_Bit() {
    return SNR_Per_Bit;
}

bool M_QAM::operator <(M_QAM Scheme) const {
    if (M < Scheme.get_M()) {
        return true;
    }

    return false;
}

bool M_QAM::operator >(M_QAM Scheme) const {
    if (M > Scheme.get_M()) {
        return true;
    }

    return false;
}