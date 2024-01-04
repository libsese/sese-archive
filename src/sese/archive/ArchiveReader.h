#pragma once

#include <sese/archive/Config.h>
#include <sese/io/InputStream.h>

#include <functional>
#include <filesystem>

namespace sese::archive {

    class ArchiveReader{
    public:
        using ExtractCallback = std::function<bool(const std::filesystem::path &base_path, Config::EntryType type, io::InputStream *input, size_t readable)>;

        static bool extract(const std::filesystem::path &src_path, const std::filesystem::path &dest_path, const std::string &pwd = {});

        explicit ArchiveReader(io::InputStream *input);

        virtual ~ArchiveReader();

        int setPassword(const std::string &pwd);

        bool extract(const ExtractCallback &callback);

        static int openCallback(void *a, ArchiveReader *_this);

        static int64_t readCallback(void *a, ArchiveReader *_this, const void **buffer);

        static int closeCallback(void *a, ArchiveReader * _this);

    protected:
        char buffer[4096]{};
        void *archive{};
        io::InputStream *input{};
    };
}