/*******************************************************************************
 * Copyright (C) 2017 Create-Net / FBK.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 * 
 * Contributors:
 *     Create-Net / FBK - initial API and implementation
 ******************************************************************************/
#ifndef __DBUS_UTILITY_H__
#define __DBUS_UTILITY_H__

#include <gio/gunixfdlist.h>
#include <gio/gio.h>

GVariant* g_variant_deep_copy(GVariant *);

#endif
