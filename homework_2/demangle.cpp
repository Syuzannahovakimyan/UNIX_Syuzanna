// compile with  -rdynamic
#include <iostream>
#include <cstdlib>
#include <cxxabi.h>
#include <execinfo.h>
#include <dlfcn.h>  // for dladdr
#include <cstring>

void stack_dump(int depth, std::ostream &os) {
    void *array[depth];
    int size = backtrace(array, depth);

    os << "Obtained " << size - 1 << " stack frames:\n";

    for (int i = 1; i < size ; i++) {
        Dl_info info;
        if (dladdr(array[i], &info) && info.dli_sname) {

            int status = 0;
            char *demangled_name = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);

            if (status == 0 && demangled_name != nullptr) {
                os << array[i] << " --> " << demangled_name << "\n";
                std::free(demangled_name);
            } else {
                os << array[i] << " --> " << info.dli_sname << " (mangled)\n";
            }
        } else {
            os << array[i] << " --> (unknown)\n";
        }
    }
}

void h() {
    std::cout << "h()\n";
    stack_dump(4, std::cout);
}

void g() {
    std::cout << "g()\n";
    h();
}

void f() {
    std::cout << "f()\n";
    g();
}

int main() {
    f();
    return 0;
}
