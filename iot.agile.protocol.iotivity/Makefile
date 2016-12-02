#! /usr/bin/make -f
# //******************************************************************
# //
# // Author: Mattia Antonini <mattia.antonini1@studenti.unipr.it>
# //                         <m.antonini@create-net.org>
# //
# //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

default: all

package?=agile_iotivity

local_bindir?=bin
local_bindir?=opt
DEST_LIB_DIR?=${DESTDIR}${local_optdir}/${package}/

vpath+=src
VPATH+=src

arch_deps?= $(shell gcc -dumpmachine)

#TODO: workaround missing /usr/include/iotivity namespace
prep_tasks+=iotivity
config_pkgconfig?=1
usr_include_dir?=${PKG_CONFIG_SYSROOT_DIR}/usr/include
iotivity_dir?=${CURDIR}/iotivity
LIBS+=-loc -loc_logger -loctbstack -lpthread -ldbus-1 -lglib-2.0 -lgio-2.0 -lgobject-2.0

LD_LIBRARY_PATHS?= -Wl,-rpath=/usr/local/lib
#Includes
CPPFLAGS+=\
 -I${iotivity_dir} \
 -I${iotivity_dir}/resource/ \
 -I${iotivity_dir}/resource/c_common \
 -I${iotivity_dir}/resource/oc_logger \
 -I${iotivity_dir}/resource/stack \
 -I/usr/include/dbus-1.0 \
 -I/usr/lib/${arch_deps}/dbus-1.0/include \
 -I/usr/include/gio-unix-2.0 \
 -I/usr/include/glib-2.0/ \
 -I/usr/lib/${arch_deps}/glib-2.0/include/

CPPFLAGS+=-I.
V=1

CXXFLAGS+=-std=c++0x

srcs?=$(wildcard ./src/*.cpp ./src/common/*.cpp ./src/agile/*.cpp)
objs?=${srcs:.cpp=.o}

agile_iotivity?=${local_bindir}/agile_iotivity

all?= ${agile_iotivity} 

all: prep ${all}

###############

agile_iotivity_tmp: remove_agile_iotivity ${agile_iotivity}
	exit

agile_iotivity: clean remove_agile_iotivity prep ${agile_iotivity}
	exit

remove_agile_iotivity:
	rm -f ${agile_iotivity}

###############

${local_bindir}/agile_iotivity: ${objs}
	@-mkdir -p ${@D}
	${CXX} -o ${@} $^ ${LD_LIBRARY_PATHS} ${LIBS}

clean:
	rm -f ./*.o *~ ${objs} */*.o iotivity
	rm -rf ${local_bindir}

distclean: clean
	rm -f ${agile_iotivity}

iotivity: ${iotivity_dir}
	@echo "IoTivity symlink created"

${iotivity_dir}: ${usr_include_dir}
	ls $</iotivity && ln -fs $</iotivity $@ || ln -fs $< $@

prep: ${prep_tasks}
