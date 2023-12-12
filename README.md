# libhandy

<p align="center">
    <a href="https://libhandy.readthedocs.io/en/latest/?badge=latest">
        <img src="https://readthedocs.org/projects/libhandy/badge/?version=latest" alt="Documentation Status">
    </a>
</p>

---

libhandy is soley a C library with functions that are commonly
used by a variety of underview software.

#### 📚 Documentation

Complete documentation: https://libhandy.readthedocs.io

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
#include <handy/handy.h>
```

**Testing**
```sh
$ ninja test -C build
```
