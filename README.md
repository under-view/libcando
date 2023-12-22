# libcando

<p align="center">
    <a href="https://libcando.readthedocs.io/en/latest/?badge=latest">
        <img src="https://readthedocs.org/projects/libcando/badge/?version=latest" alt="Documentation Status">
    </a>
</p>

---

libcando is soley a C library with functions that are commonly
used by a variety of underview software.

#### 📚 Documentation

Complete documentation: https://libcando.readthedocs.io

**Building**
```
$ meson build
$ ninja -C build

# For Yocto SDK builds
$ meson setup --prefix="${SDKTARGETSYSROOT}/usr" \
              --libdir="${SDKTARGETSYSROOT}/usr/lib64" \
              build
$ ninja -C build
```

**Install**
```sh
$ ninja install -C build
```

**Inclusion**
```C
#include <cando/cando.h>
```

**Testing**
```sh
$ ninja test -C build
```
