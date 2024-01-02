#pragma once

namespace sese::archive {
    class Config {
    public:
        enum class EntryType {
            MT,
            REG,
            LNK,
            SOCK,
            CHR,
            BLK,
            DIR,
            IFO
        };
        static int toValue(EntryType type);
        static EntryType fromValue(int value);
    };
}