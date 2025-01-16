/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef DLLOADER_HPP
#define DLLOADER_HPP

#include <dlfcn.h>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <vector>

namespace RayTracer {
    class DLLoader {
    public:
        explicit DLLoader(const std::string &filename) : handle(nullptr)
        {
            handle = ::dlopen(filename.c_str(), RTLD_LAZY | RTLD_GLOBAL);
            if (!handle) {
                throw std::runtime_error(dlerror());
            }
        }

        ~DLLoader() {
            if (handle) {
                ::dlclose(handle);
            }
        }

        template<typename T>
        T *getInstance(const std::string &symbolName) {
            (void)::dlerror();
            void *symbol = ::dlsym(handle, symbolName.c_str());
            const char *error = ::dlerror();

            if (error != nullptr) {
                throw std::runtime_error(error);
            }

            auto (*createFunc)() = reinterpret_cast<T *(*)()>(symbol);
            void* instancePtr = createFunc();

            return static_cast<T*>(instancePtr);
        }

    private:
        void *handle;
    };
}

#endif
