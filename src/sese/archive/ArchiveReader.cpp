#include <sese/archive/ArchiveReader.h>
#include <sese/archive/ArchiveInputStream.h>
#include <sese/io/File.h>

#include <archive.h>
#include <archive_entry.h>

using sese::io::File;
using sese::io::InputStream;
using sese::archive::ArchiveReader;

bool ArchiveReader::extract(const std::filesystem::path &src_path, const std::filesystem::path &dest_path) {
    auto src = src_path.string();
    auto dest = dest_path.string();

    struct archive *a = archive_read_new();
    archive_read_support_compression_all(a);
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    archive_read_open_filename(a, src.c_str(), 4096);

    struct archive_entry *entry{};
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        std::string filename = archive_entry_pathname(entry);

        auto size = archive_entry_size(entry);
        auto type = archive_entry_filetype(entry);
        if (AE_IFDIR == type) {
            if ('/' == filename.at(filename.length() - 1)) {
                filename = filename.substr(0, filename.length() - 1);
            }
            auto path = dest_path / filename;
            if (std::filesystem::exists(path)) {
                if (!std::filesystem::is_directory(path)) {
                    archive_read_free(a);
                    return false;
                }
            } else {
                if (!std::filesystem::create_directories(path)) {
                    archive_read_free(a);
                    return false;
                }
            }
            continue;
        }

        auto parent = (dest_path / filename).parent_path();
        if (!std::filesystem::exists(parent)) {
            if (!std::filesystem::create_directories(parent)) {
                archive_read_free(a);
                return false;
            }
        }

        size_t len;
        char buffer[4096];
        auto file = File::create(dest + filename, BINARY_WRITE_CREATE_TRUNC);
        while ((len = archive_read_data(a, buffer, sizeof(buffer))) > 0) {
            file->write(buffer, len);
        }
        file->flush();
        file->close();
    }

    archive_read_free(a);
    return true;
}

inline int open(struct archive *a, void *data) {
    return ArchiveReader::openCallback(a, static_cast<ArchiveReader *>(data));
}

inline int64_t read(struct archive *a, void *data, const void **buffer) {
    return ArchiveReader::readCallback(a, static_cast<ArchiveReader *>(data), buffer);
}

inline int close(struct archive *a, void *data) {
    return ArchiveReader::closeCallback(a, static_cast<ArchiveReader *>(data));
}

#define XX ((struct archive *) (this->archive))

ArchiveReader::ArchiveReader(io::InputStream *input)
        : input(input), archive(archive_read_new()) {
    archive_read_support_compression_all(XX);
    archive_read_support_filter_all(XX);
    archive_read_support_format_all(XX);
    archive_read_open(
            XX,
            this,
            open,
            read,
            close
    );
}

ArchiveReader::~ArchiveReader() {
    archive_read_free(XX);
}

bool ArchiveReader::extract(const ArchiveReader::ExtractCallback &callback) {
    struct archive_entry *entry{};
    auto archiveInputStream = ArchiveInputStream(archive);
    while (archive_read_next_header(XX, &entry) == ARCHIVE_OK) {
        std::string filename = archive_entry_pathname(entry);

        auto size = archive_entry_size(entry);
        auto type = archive_entry_filetype(entry);

        callback(filename, Config::fromValue(type), &archiveInputStream, size);
    }
    return true;
}

int ArchiveReader::openCallback(void *a, ArchiveReader *_this) {
    return ARCHIVE_OK;
}

int64_t ArchiveReader::readCallback(void *a, ArchiveReader *_this, const void **buffer) {
    auto read = _this->input->read(_this->buffer, 4096);
    *buffer = _this->buffer;
    return read;
}

int ArchiveReader::closeCallback(void *a, ArchiveReader *_this) {
    return ARCHIVE_OK;
}

