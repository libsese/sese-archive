#pragma once

#include <sese/io/InputStream.h>

namespace sese::archive {

    class ArchiveInputStream final : public io::InputStream {
    public:
        explicit ArchiveInputStream(void *archive_read);

        int64_t read(void *buffer, size_t length) override;

    protected:
        void *archive_read;
    };
}