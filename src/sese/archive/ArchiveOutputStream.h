#pragma once

#include <sese/io/OutputStream.h>

namespace sese::archive {
    class ArchiveOutputStream : public io::OutputStream {
    public:
        explicit ArchiveOutputStream(void *archive_write);

        int64_t write(const void *buffer, size_t length) override;

    protected:
        void *archive_write{};
    };
}