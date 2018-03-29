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

#include "DBusUtility.h"

//Clone a GVariant
GVariant* g_variant_deep_copy(GVariant *value)
{
  switch (g_variant_classify (value))
    {
    case G_VARIANT_CLASS_MAYBE:
    case G_VARIANT_CLASS_ARRAY:
    case G_VARIANT_CLASS_TUPLE:
    case G_VARIANT_CLASS_DICT_ENTRY:
    case G_VARIANT_CLASS_VARIANT:
      {
        GVariantBuilder builder;
        GVariantIter iter;
        GVariant *child;

        g_variant_builder_init (&builder, g_variant_get_type (value));
        g_variant_iter_init (&iter, value);

        while ((child = g_variant_iter_next_value (&iter)))
          {
            g_variant_builder_add_value (&builder, g_variant_deep_copy(child));
            g_variant_unref (child);
          }

        return g_variant_builder_end (&builder);
      }

    case G_VARIANT_CLASS_BOOLEAN:
      return g_variant_new_boolean (g_variant_get_boolean (value));

    case G_VARIANT_CLASS_BYTE:
      return g_variant_new_byte (g_variant_get_byte (value));

    case G_VARIANT_CLASS_INT16:
      return g_variant_new_int16 (g_variant_get_int16 (value));

    case G_VARIANT_CLASS_UINT16:
      return g_variant_new_uint16 (g_variant_get_uint16 (value));

    case G_VARIANT_CLASS_INT32:
      return g_variant_new_int32 (g_variant_get_int32 (value));

    case G_VARIANT_CLASS_UINT32:
      return g_variant_new_uint32 (g_variant_get_uint32 (value));

    case G_VARIANT_CLASS_INT64:
      return g_variant_new_int64 (g_variant_get_int64 (value));

    case G_VARIANT_CLASS_UINT64:
      return g_variant_new_uint64 (g_variant_get_uint64 (value));

    case G_VARIANT_CLASS_HANDLE:
      return g_variant_new_handle (g_variant_get_handle (value));

    case G_VARIANT_CLASS_DOUBLE:
      return g_variant_new_double (g_variant_get_double (value));

    case G_VARIANT_CLASS_STRING:
      return g_variant_new_string (g_variant_get_string (value, NULL));

    case G_VARIANT_CLASS_OBJECT_PATH:
      return g_variant_new_object_path (g_variant_get_string (value, NULL));

    case G_VARIANT_CLASS_SIGNATURE:
      return g_variant_new_signature (g_variant_get_string (value, NULL));
    }

  g_assert_not_reached ();
}
