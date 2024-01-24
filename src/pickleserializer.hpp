#pragma once

#include <csdr-eti/meta.hpp>

namespace Csdr::Eti {

    class PickleSerializer: public Serializer {
        public:
            std::string serialize(std::map<std::string, std::string> data) override;
    };

}