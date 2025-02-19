#pragma once

#include <fcntl.h>
#include <common/StringRef.h>

namespace DB::HybridCache
{

class File
{
public:
    File() noexcept : fd_(-1), owns_fd_(false) { }
    explicit File(int fd, bool owns_fd = false) noexcept;

    explicit File(const char * name, int flags = O_RDONLY, mode_t mode = 0666);
    explicit File(const std::string & name, int flags = O_RDONLY, mode_t mode = 0666);
    explicit File(StringRef name, int flags = O_RDONLY, mode_t mode = 0666);

    ~File();

    int fd() const { return fd_; }

    explicit operator bool() const { return fd_ != -1; }

    void close();

    bool closeNoThrow();

    int release() noexcept;

    File(File &&) noexcept;
    File & operator=(File &&);

    void lock();
    bool tryLock();
    void unlock() const;

    void lockShared();
    bool tryLockShared();
    void unlockShared() const;

    void swap(File & other) noexcept;

    File(const File &) = delete;
    File & operator=(const File &) = delete;

private:
    void doLock(int op) const;
    bool doTryLock(int op) const;

    int fd_;
    bool owns_fd_;
};

}
