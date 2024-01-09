// Copyright 2024 libsese
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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

        int setOptions(const std::string &opt);

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