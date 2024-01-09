#include <sese/archive/ArchiveOutputStream.h>

#include <archive.h>

sese::archive::ArchiveOutputStream::ArchiveOutputStream(void *archive_write) :
        archive_write(archive_write) {
}

int64_t sese::archive::ArchiveOutputStream::write(const void *buffer, size_t length) {
    return archive_write_data(static_cast<struct  archive *>(this->archive_write), buffer, length);
}