/*******************************************************************************
 * Copyright (c) 2016, 2017 FBK CREATE-NET
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    https://www.eclipse.org/legal/epl-2.0/
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Description: DBusUtility
 *              Header file for DBusUtility
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#ifndef __DBUS_UTILITY_H__
#define __DBUS_UTILITY_H__

#include <gio/gunixfdlist.h>
#include <gio/gio.h>

GVariant* g_variant_deep_copy(GVariant *);

#endif
