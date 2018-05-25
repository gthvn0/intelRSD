/*
 * Copyright (c) 2017-2018 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.intel.podm.business.redfish.services.assembly.tasks;

import com.intel.podm.business.services.context.Context;
import com.intel.podm.common.enterprise.utils.beans.BeanFactory;
import com.intel.podm.common.types.EntityRole;
import com.intel.podm.common.types.Id;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import javax.transaction.Transactional;

import static javax.transaction.Transactional.TxType.MANDATORY;

@Dependent
@Transactional(MANDATORY)
public class EndpointTaskFactory {
    @Inject
    private BeanFactory beanFactory;

    public EndpointAssemblyTask create(Id volumeId, EntityRole endpointRole, Context context) {
        EndpointAssemblyTask endpointAssemblyTask = beanFactory.create(EndpointAssemblyTask.class);
        endpointAssemblyTask.setVolumeId(volumeId);
        endpointAssemblyTask.setEndpointRole(endpointRole);
        endpointAssemblyTask.setContext(context);

        return endpointAssemblyTask;
    }

    public NodeTask create(EntityRole endpointRole, Context context) {
        EndpointAssemblyTask endpointAssemblyTask = beanFactory.create(EndpointAssemblyTask.class);
        endpointAssemblyTask.setEndpointRole(endpointRole);
        endpointAssemblyTask.setContext(context);

        return endpointAssemblyTask;
    }

    public NodeTask create(EntityRole endpointRole) {
        EndpointAssemblyTask endpointAssemblyTask = beanFactory.create(EndpointAssemblyTask.class);
        endpointAssemblyTask.setEndpointRole(endpointRole);

        return endpointAssemblyTask;
    }
}
