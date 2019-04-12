/*
 * Copyright (c) 2015-2019 Intel Corporation
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

package com.intel.rsd.nodecomposer.externalservices.resources.redfish;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsd.nodecomposer.business.services.redfish.odataid.ODataId;
import com.intel.rsd.nodecomposer.externalservices.LinkName;
import com.intel.rsd.nodecomposer.externalservices.OdataTypes;
import com.intel.rsd.nodecomposer.externalservices.WebClientRequestException;
import com.intel.rsd.nodecomposer.externalservices.reader.ResourceSupplier;
import com.intel.rsd.nodecomposer.externalservices.resources.ExternalServiceResourceImpl;
import com.intel.rsd.nodecomposer.types.Status;
import lombok.Getter;

import java.util.ArrayList;
import java.util.List;

import static com.intel.rsd.redfish.ODataTypeVersions.VERSION_PATTERN;

@OdataTypes({
    "#Manager" + VERSION_PATTERN + "Manager"
})
public class ManagerResource extends ExternalServiceResourceImpl {
    @Getter
    @JsonProperty("ManagerType")
    private String managerType;

    @Getter
    @JsonProperty("ServiceEntryPointUUID")
    private String serviceEntryPointUuid;

    @Getter
    @JsonProperty("RemoteRedfishServiceUri")
    private String remoteRedfishServiceUri;

    @Getter
    @JsonProperty("Status")
    private Status status;

    @JsonProperty("Links")
    private Links links = new Links();

    @LinkName("managerForServers")
    public Iterable<ResourceSupplier> getManagerForServers() throws WebClientRequestException {
        return toSuppliers(links.managerForServers);
    }

    @LinkName("managerForServices")
    public Iterable<ResourceSupplier> getManagerForServices() throws WebClientRequestException {
        return toSuppliers(links.oem.rackScaleOem.managerForServices);
    }

    public class Links {
        @JsonProperty("ManagerForServers")
        private List<ODataId> managerForServers = new ArrayList<>();

        @JsonProperty("Oem")
        private Oem oem = new Oem();

        public class Oem {
            @JsonProperty("Intel_RackScale")
            private Oem.RackScaleOem rackScaleOem = new Oem.RackScaleOem();

            public class RackScaleOem {
                @JsonProperty("ManagerForServices")
                private List<ODataId> managerForServices = new ArrayList<>();
            }
        }
    }
}
