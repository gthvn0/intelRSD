/*
 * Copyright (c) 2015-2018 Intel Corporation
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

package com.intel.podm.business.redfish.services.allocation.strategy;

import com.intel.podm.business.redfish.services.allocation.AllocationRequestProcessingException;
import com.intel.podm.business.redfish.services.allocation.validation.RequestedNodeResourceContextsValidator;
import com.intel.podm.business.services.redfish.requests.RequestedNode;
import com.intel.podm.common.enterprise.utils.beans.BeanFactory;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import javax.transaction.Transactional;

import static javax.transaction.Transactional.TxType.MANDATORY;

@Dependent
public class AllocationStrategyFactory {
    @Inject
    private RequestedNodeResourceContextsValidator requestedNodeResourceContextsValidator;

    @Inject
    private ComputerSystemAllocationStrategyFactory computerSystemAllocationStrategyFactory;

    @Inject
    private RemoteDriveAllocationStrategyFactory remoteDriveStrategyFactory;

    @Inject
    private BeanFactory beanFactory;

    @Transactional(MANDATORY)
    public AllocationStrategy create(RequestedNode requestedNode) throws AllocationRequestProcessingException {
        requestedNodeResourceContextsValidator.validateExistenceOfIncludedResources(requestedNode);

        ComputerSystemAllocationStrategy computerSystemAllocationStrategy = computerSystemAllocationStrategyFactory.create(requestedNode);
        RemoteDriveAllocationStrategy driveAllocationStrategy = remoteDriveStrategyFactory.create(requestedNode);

        AllocationStrategy allocationStrategy = beanFactory.create(AllocationStrategy.class);
        allocationStrategy.setComputerSystemAllocationStrategy(computerSystemAllocationStrategy);
        allocationStrategy.setRemoteDriveAllocationStrategy(driveAllocationStrategy);

        return allocationStrategy;
    }
}
