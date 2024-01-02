#include <sese/archive/ArchiveInputStream.h>

#include <archive.h>

sese::archive::ArchiveInputStream::ArchiveInputStream(void *archive_read)
        : archive_read(archive_read) {
}

int64_t sese::archive::ArchiveInputStream::read(void *buffer, size_t length) {
    return archive_read_data(static_cast<struct archive *>(archive_read), buffer, length);
}