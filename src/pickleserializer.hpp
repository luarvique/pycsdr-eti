#pragma once

#include <csdr-eti/meta.hpp>

namespace Csdr::Eti {

    class PickleSerializer: public Serializer {
        public:
            std::string serialize(std::map<std::string, datatype> data) override;
            std::string serializeProgrammes(std::map<uint16_t, std::string> data) override;
    };

}