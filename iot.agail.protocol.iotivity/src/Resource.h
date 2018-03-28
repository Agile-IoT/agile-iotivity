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

#include <unistd.h>

#include <iotivity/resource/OCResource.h>


class Resource
{
    public:
    std::shared_ptr<OC::OCResource> resource;

    Resource();
    Resource(std::shared_ptr<OC::OCResource>);
    ~Resource();

    bool operator==(const Resource&) const;
};
