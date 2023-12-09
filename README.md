# libhandy

libhandy is soley a C library with functions that are commonly
used by a variety of underview software.

**Building**
```
$ meson build
$ ninja -C build

# For Yocto SDK builds
$ meson --prefix="${SDKTARGETSYSROOT}/usr" \
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
