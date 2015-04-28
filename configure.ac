AC_INIT([netdumper], [0.1], [nekit123400@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_PROG_CC

CPPFLAGS="-D_FORTIFY_SOURCE=2"

CFLAGS_WARNINGS="-Wall -Wextra -Wpedantic -pedantic -Wformat-security -Werror=format-security -Wconversion -Wsign-conversion"

# Release
CFLAGS="-std=c11 -mfpmath=both -flto -Ofast $CFLAGS_WARNINGS -pipe -fstack-protector-strong --param=ssp-buffer-size=4 -pie -fPIE -s $CFLAGS"

# Debug
#CFLAGS="-std=c11 -march=native -Og -g3 $CFLAGS_WARNINGS -pipe -fstack-protector-strong --param=ssp-buffer-size=4 -pie -fPIE $CFLAGS"

# clang
#CFLAGS="-std=c11 -march=native -Ofast -Weverything $CFLAGS_WARNINGS -pipe -fPIE -fstack-protector-strong --param=ssp-buffer-size=4 $CFLAGS"

CXXFLAGS="$CFLAGS $CXXFLAGS"

LDFLAGS="-Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now"

# mingw doesn't like nice things
#LDFLAGS="-Wl,-O1,--sort-common,--as-needed"

AC_CHECK_HEADERS([systemd/sd-daemon.h])
AC_CONFIG_HEADERS([config.h])
PKG_CHECK_MODULES(LIBSYSTEMD, libsystemd)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
