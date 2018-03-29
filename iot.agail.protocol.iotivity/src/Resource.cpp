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
