#! /usr/bin/make -f
#-------------------------------------------------------------------------------
# Copyright (C) 2017 Create-Net / FBK.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0
# which accompanies this distribution, and is available at
# https://www.eclipse.org/legal/epl-2.0/
# 
# Contributors:
#     Create-Net / FBK - initial API and implementation
#-------------------------------------------------------------------------------
# //******************************************************************
# //
# // Author: Mattia Antonini <mattia.antonini1@studenti.unipr.it>
# //                         <m.antonini@fbk.eu>
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
LIBS+=-loc -loc_logger -loctbstack -lpthread

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

srcs?=$(wildcard ./src/*.cpp)
objs?=${srcs:.cpp=.o}

client?=${local_bindir}/client
#client_objs?=${srcs:.cpp=.o}

all?= ${oic_client} 

all: prep ${all}

###############

client: clean remove_client prep ${client}
	exit

remove_client:
	rm -f ${client}

###############

${local_bindir}/%: %.o ${objs}
	@-mkdir -p ${@D}
	${CXX} -o ${@} $^ ${LDFLAGS} ${LIBS}

clean:
	rm -f ./*.o *~ ${objs} */*.o iotivity
	rm -rf ${local_bindir}

distclean: clean
	rm -f ${oic_client}

iotivity: ${iotivity_dir}
	@echo "TODO: workaround for namespace"

${iotivity_dir}: ${usr_include_dir}
	ls $</iotivity && ln -fs $</iotivity $@ || ln -fs $< $@
	ls $@/resource

prep: ${prep_tasks}
