#include <sese/archive/ArchiveReader.h>
#include <sese/record/Marco.h>
#include <sese/io/File.h>

#include <gtest/gtest.h>

using sese::io::File;
using sese::archive::ArchiveReader;

TEST(TestReader, Extract) {
    EXPECT_TRUE(ArchiveReader::extract(PROJECT_PATH "/test/archive.zip", PROJECT_BIN_PATH "/archive-p", "password123"));
}

TEST(TestReader, Info) {
    auto file = File::create(PROJECT_PATH "/test/archive.zip", BINARY_READ_EXISTED);
    ArchiveReader reader(file.get());
    EXPECT_EQ(reader.setPassword("password123"), 0);
    EXPECT_TRUE(reader.extract(
            [](const std::filesystem::path &wpath,
               sese::archive::Config::EntryType type,
               sese::io::InputStream *input,
               size_t size) -> bool {
                    auto path = wpath.string();
                    SESE_INFO("path: %s size: %zu type: %d", path.c_str(), size, static_cast<int>(type));
                return true;
            }));
    file->close();
}