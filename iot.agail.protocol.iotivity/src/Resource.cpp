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
 * Description: Resource
 *              Implementation file for Resource wrapper
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#include "Resource.h"

Resource::Resource()
{
    resource = nullptr;
}

Resource::Resource(std::shared_ptr<OC::OCResource> res)
{
    resource = res;
}

Resource::~Resource()
{
}

bool Resource::operator==(const Resource &res) const
{
    const Resource t = *this;
    return (*(t.resource) == *(res.resource));
}
