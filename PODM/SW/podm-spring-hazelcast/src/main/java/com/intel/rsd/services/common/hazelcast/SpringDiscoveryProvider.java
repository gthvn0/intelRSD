/*
 * Copyright (c) 2017-2019 Intel Corporation
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

package com.intel.rsd.services.common.hazelcast;

import com.hazelcast.spi.discovery.integration.DiscoveryService;
import com.hazelcast.spi.discovery.integration.DiscoveryServiceProvider;
import com.hazelcast.spi.discovery.integration.DiscoveryServiceSettings;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class SpringDiscoveryProvider implements DiscoveryServiceProvider {
    private final SpringDiscoveryService springDiscoveryService;

    @Autowired
    public SpringDiscoveryProvider(SpringDiscoveryService springDiscoveryService) {
        this.springDiscoveryService = springDiscoveryService;
    }

    @Override
    public DiscoveryService newDiscoveryService(DiscoveryServiceSettings settings) {
        return springDiscoveryService;
    }
}
