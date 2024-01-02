#include <sese/archive/Config.h>

#include <archive_entry.h>

using sese::archive::Config;

int Config::toValue(Config::EntryType type) {
#define XX(str)\
    case EntryType::str:  \
        return AE_IF##str;

    switch (type) {
        XX(MT)
        XX(REG)
        XX(LNK)
        XX(SOCK)
        XX(CHR)
        XX(BLK)
        XX(DIR)
        XX(IFO)
        default:
            return 0;
    }
#undef XX
}

Config::EntryType Config::fromValue(int value) {
#define XX(str) \
    case AE_IF##str: \
        return Config::EntryType::str;
    switch (value) {
        XX(MT)
        XX(REG)
        XX(LNK)
        XX(SOCK)
        XX(CHR)
        XX(BLK)
        XX(DIR)
        XX(IFO)
        default:
            return Config::EntryType::REG;
    }
#undef XX
}