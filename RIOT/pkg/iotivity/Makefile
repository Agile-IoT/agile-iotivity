PKG_NAME=iotivity-constrained
PKG_URL=https://gerrit.iotivity.org/gerrit/iotivity-constrained
PKG_VERSION=1b7513395ddf96610bf02d40054a28ac98ffa523
PKG_DIR=$(CURDIR)

PKG_LIB=$(BINDIRBASE)/pkg/$(BOARD)/$(PKG_NAME)


MODULE_MAKEFILE := $(PKG_DIR)/Makefile.module

export PKG_DIR
export PKG_LIB
export PKG_NAME

#TODO: fix this problem in code
CFLAGS := $(filter-out -Werror=old-style-definition,$(CFLAGS))

export RIOT_CFLAGS = ${CFLAGS} ${INCLUDES}

all: git-download submodules-download copy_makefile
	"$(MAKE)" -BC $(PKG_BUILDDIR) lib-iotivity.a
	cp $(PKG_BUILDDIR)/lib-iotivity.a ${BINDIR}/iotivity.a

copy_makefile:
	@echo 'Coping makefile'
	cp ./Makefile.tomove $(PKG_LIB)/Makefile


submodules-download:
	git -C $(PKG_LIB) submodule update --init

include $(RIOTBASE)/pkg/pkg.mk
