/*
 * Copyright 2017 CREATE-NET
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @ingroup
 * @{
 *
 * @file        DBusUtility.cpp
 * @brief       DBusUtility. Implementation file
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

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
