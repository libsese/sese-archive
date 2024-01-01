#pragma once

#include <sese/io/InputStream.h>
#include <sese/io/OutputStream.h>

#include <filesystem>

namespace sese::archive {
    class ArchiveWriter {
    public:
        explicit ArchiveWriter(io::OutputStream *output);

        virtual ~ArchiveWriter();

        int setFilterGZip();

        int setFilterBZip2();

        int setFilterLZ4();

        int setFilterLZip();

        int setFilterZstd();

        int setFilterXZ();

        int setFilterLzma();

        int setFilterLzop();

        int setFilterGRZip();

        int setFilterLRZip();

        int setFilterNone();

        int setFormatXar();

        int setFormatTar();

        int setFormatZip();

        int setFormat7z();

        int setFormatISO();

        int setPassword(const std::string &pwd);

        int setOptions(const std::string &opt);

        int getError();

        const char *getErrorString();

        bool begin();

        bool done();

        bool addPath(const std::filesystem::path &path);

        bool addPath(const std::filesystem::path &base, const std::filesystem::path &path);

        bool addFile(const std::filesystem::path &file);

        bool addFile(const std::filesystem::path &base, const std::filesystem::path &file);

        bool addDirectory(const std::filesystem::path &dir);

        bool addDirectory(const std::filesystem::path &base, const std::filesystem::path &dir);

        bool addStream(const std::filesystem::path &path, io::InputStream *input, size_t len);

        static int openCallback(void *archive, ArchiveWriter *_this);

        static int64_t writeCallback(void *archive, ArchiveWriter *_this, const void *buffer, size_t len);

        static int closeCallback(void *archive, ArchiveWriter *_this);

        static int freeCallback(void *archive, ArchiveWriter *_this);

        static const char *passphraseCallback(void *archive, ArchiveWriter *_this);

    protected:
        io::OutputStream *output{};
        void *archive{};
    };
}