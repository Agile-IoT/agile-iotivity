#! /usr/bin/make -f
# //******************************************************************
# //
# // Author: Mattia Antonini <mattia.antonini1@studenti.unipr.it>
# //                         <m.antonini@create-net.org>
# //
# //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

default: all

package?=iotivity-client

local_bindir?=bin
local_bindir?=opt
DEST_LIB_DIR?=${DESTDIR}${local_optdir}/${package}/

vpath+=src
VPATH+=src


#TODO: workaround missing /usr/include/iotivity namespace
prep_tasks+=iotivity
config_pkgconfig?=1
usr_include_dir?=${PKG_CONFIG_SYSROOT_DIR}/usr/include
iotivity_dir?=${CURDIR}/iotivity
LIBS+=-loc -loc_logger -loctbstack

#Includes
CPPFLAGS+=\
 -I${iotivity_dir} \
 -I${iotivity_dir}/resource/ \
 -I${iotivity_dir}/resource/c_common \
 -I${iotivity_dir}/resource/oc_logger \
 -I${iotivity_dir}/resource/stack \

CPPFLAGS+=-I.
V=1

CXXFLAGS+=-std=c++0x

objs?=${srcs:.cpp=.o}


simple_client?=${local_bindir}/simple_client
simple_client_objs?=${cpp=.o}

all?= ${simple_client}


${local_bindir}/simple_client: simple_client.o ${simple_client_objs}
	@-mkdir -p ${@D}
	${CXX} -o ${@} $^ ${LDFLAGS} ${LIBS}

all: prep ${all}

###############

simple_client: clean remove_simple_client prep ${simple_client}
	exit

remove_simple_client:
	rm -f ${simple_client}

###############

${local_bindir}/%: %.o ${objs}
	@-mkdir -p ${@D}
	${CXX} -o ${@} $^ ${LDFLAGS} ${LIBS}

clean:
	rm -f *.o *~ ${objs} */*.o iotivity

distclean: clean
	rm -f ${simple_client}

iotivity: ${iotivity_dir}
	@echo "TODO: workaround for namespace"

${iotivity_dir}: ${usr_include_dir}
	ls $</iotivity && ln -fs $</iotivity $@ || ln -fs $< $@
	ls $@/resource

prep: ${prep_tasks}

run/%: ${CURDIR}/bin/%
	${<D}/${<F}

xterm/% : bin/%
	xterm -e ${MAKE} run/${@F} &
	sleep 5

run: xterm/simple_client
