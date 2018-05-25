/*
 * Copyright (c) 2016-2018 Intel Corporation
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

package com.intel.podm.business.redfish.services.allocation.templates.requestednode;

import java.util.List;

public class RequestedNodeWithProcessorsAndMemoryModules extends RequestedNodeBuilder {
    private final List<Processor> processors;
    private final List<Memory> memoryModules;

    public RequestedNodeWithProcessorsAndMemoryModules(List<Processor> processors, List<Memory> memoryModules) {
        this.processors = processors;
        this.memoryModules = memoryModules;
    }

    @Override
    public List<Processor> getProcessors() {
        return processors;
    }

    @Override
    public List<Memory> getMemoryModules() {
        return memoryModules;
    }
}
