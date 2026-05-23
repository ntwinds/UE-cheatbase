#pragma once

namespace cheatbase::core {

template<typename T>
class Singleton {
public:
    CHEATBASE_NODISCARD static T& Get() {
        static T instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;
};

} // namespace cheatbase::core
