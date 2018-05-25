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

package com.intel.podm.mappers.redfish;

import com.intel.podm.business.entities.redfish.Drive;
import com.intel.podm.client.WebClient;
import com.intel.podm.client.reader.ResourceSupplier;
import com.intel.podm.client.resources.redfish.DriveResource;
import com.intel.podm.client.resources.redfish.IdentifierObject;
import com.intel.podm.client.resources.redfish.RedfishLocationObject;
import com.intel.podm.common.types.EncryptionAbility;
import com.intel.podm.common.types.EncryptionStatus;
import com.intel.podm.common.types.HotspareType;
import com.intel.podm.common.types.IndicatorLed;
import com.intel.podm.common.types.MediaType;
import com.intel.podm.common.types.Protocol;
import com.intel.podm.common.types.Ref;
import com.intel.podm.common.types.Status;
import com.intel.podm.common.types.StatusIndicator;
import com.intel.podm.mappers.subresources.IdentifierMapper;
import com.intel.podm.mappers.subresources.RedfishLocationMapper;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.testng.Assert;
import org.testng.annotations.BeforeMethod;
import org.testng.annotations.Test;

import java.math.BigDecimal;
import java.net.URI;
import java.util.HashMap;
import java.util.List;

import static com.intel.podm.common.types.EncryptionAbility.OTHER;
import static com.intel.podm.common.types.EncryptionStatus.FOREIGN;
import static com.intel.podm.common.types.HotspareType.CHASSIS;
import static com.intel.podm.common.types.IndicatorLed.BLINKING;
import static com.intel.podm.common.types.MediaType.SSD;
import static com.intel.podm.common.types.Protocol.AHCI;
import static com.intel.podm.common.types.Status.statusFromMap;
import static com.intel.podm.common.types.StatusIndicator.FAIL;
import static java.util.Collections.emptyList;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.testng.Assert.assertEquals;

@SuppressWarnings({
    "checkstyle:ExecutableStatementCount", "checkstyle:MethodLength", "checkstyle:MethodName"
})
public class DriveMapperTest {
    @InjectMocks
    private DriveMapper driveMapper;

    @Mock
    private IdentifierMapper identifierMapper;

    @Mock
    private RedfishLocationMapper redfishLocationMapper;

    @BeforeMethod
    public void initMocks() {
        MockitoAnnotations.initMocks(this);
    }

    @Test
    public void whenPropertiesAreAssigned_shouldCorrectlyMapAllOfThem() {
        Drive drive = new Drive();
        DriveTestResource resource = new DriveTestResource();

        driveMapper.map(resource, drive);

        assertEquals(resource.getName(), drive.getName());
        assertEquals(resource.getDescription(), drive.getDescription());

        assertEquals(resource.getStatus().get(), drive.getStatus());
        assertEquals(resource.getProtocol().get(), drive.getProtocol());
        assertEquals(resource.getMediaType().get(), drive.getMediaType());
        assertEquals(resource.getIndicatorLed().get(), drive.getIndicatorLed());
        assertEquals(resource.getHotspareType().get(), drive.getHotspareType());
        assertEquals(resource.getStatusIndicator().get(), drive.getStatusIndicator());
        assertEquals(resource.getEncryptionStatus().get(), drive.getEncryptionStatus());

        assertEquals(resource.getSku().get(), drive.getSku());
        assertEquals(resource.getModel().get(), drive.getModel());
        assertEquals(resource.getRevision().get(), drive.getRevision());
        assertEquals(resource.getPartNumber().get(), drive.getPartNumber());
        assertEquals(resource.getManufacturer().get(), drive.getManufacturer());
        assertEquals(resource.getSerialNumber().get(), drive.getSerialNumber());
        assertEquals(resource.getCapacityBytes().get(), drive.getCapacityBytes());
        assertEquals(resource.getRotationSpeedRpm().get(), drive.getRotationSpeedRpm());
        assertEquals(resource.getFailurePredicted().get(), drive.getFailurePredicted());

        assertEquals(resource.getAssetTag().get(), drive.getAssetTag());
        assertEquals(resource.getBlockSizeBytes().get(), drive.getBlockSizeBytes());
        assertEquals(resource.getCapableSpeedGbs().get(), drive.getCapableSpeedGbs());
        assertEquals(resource.getRotationSpeedRpm().get(), drive.getRotationSpeedRpm());
        assertEquals(resource.getEncryptionAbility().get(), drive.getEncryptionAbility());
        assertEquals(resource.getNegotiatedSpeedGbs().get(), drive.getNegotiatedSpeedGbs());
        assertEquals(resource.getPredictedMediaLifeLeftPercent().get(), drive.getPredictedMediaLifeLeftPercent());
        assertEquals(resource.getFirmwareVersion().get(), drive.getFirmwareVersion());
        assertEquals(resource.getDriveErased().get(), drive.getDriveErased());
    }

    @Test
    public void whenRefIsUnassigned_propertyShouldNotBeMapped() {
        final Drive drive = new Drive();
        DriveTestResource mock = mock(DriveTestResource.class);
        when(mock.getEncryptionAbility()).thenReturn(Ref.unassigned());
        when(mock.getIdentifiers()).thenReturn(Ref.unassigned());
        when(mock.getLocation()).thenReturn(Ref.unassigned());

        driveMapper.map(mock, drive);

        Assert.assertNull(drive.getEncryptionAbility());
    }

    @SuppressWarnings({"checkstyle:MethodCount", "checkstyle:MagicNumber"})
    static class DriveTestResource extends DriveResource {

        public Ref<StatusIndicator> getStatusIndicator() {
            return Ref.of(FAIL);
        }

        public Ref<IndicatorLed> getIndicatorLed() {
            return Ref.of(BLINKING);
        }

        public Ref<String> getModel() {
            return Ref.of("model");
        }

        public Ref<String> getRevision() {
            return Ref.of("revision");
        }

        public Ref<Long> getCapacityBytes() {
            return Ref.of(123456L);
        }

        public Ref<Boolean> getFailurePredicted() {
            return Ref.of(true);
        }

        public Ref<Protocol> getProtocol() {
            return Ref.of(AHCI);
        }

        public Ref<MediaType> getMediaType() {
            return Ref.of(SSD);
        }

        public Ref<String> getManufacturer() {
            return Ref.of("manufacturer");
        }

        public Ref<String> getSku() {
            return Ref.of("sku");
        }

        public Ref<String> getSerialNumber() {
            return Ref.of("123456789");
        }

        public Ref<String> getPartNumber() {
            return Ref.of("partNumber");
        }

        public Ref<Status> getStatus() {
            return Ref.of(statusFromMap(new HashMap<String, String>() {
                private static final long serialVersionUID = 4446122284148005533L;

                {
                    put("Health", "OK");
                    put("HealthRollup", "OK");
                }
            }));
        }

        public Ref<String> getAssetTag() {
            return Ref.of("assetTag");
        }

        public Ref<List<IdentifierObject>> getIdentifiers() {
            return Ref.of(emptyList());
        }

        public Ref<List<RedfishLocationObject>> getLocation() {
            return Ref.of(emptyList());
        }

        public Ref<HotspareType> getHotspareType() {
            return Ref.of(CHASSIS);
        }

        public Ref<EncryptionAbility> getEncryptionAbility() {
            return Ref.of(OTHER);
        }

        public Ref<EncryptionStatus> getEncryptionStatus() {
            return Ref.of(FOREIGN);
        }

        public Ref<BigDecimal> getRotationSpeedRpm() {
            return Ref.of(new BigDecimal(123456));
        }

        public Ref<Integer> getBlockSizeBytes() {
            return Ref.of(12345);
        }

        public Ref<BigDecimal> getCapableSpeedGbs() {
            return Ref.of(new BigDecimal(456));
        }

        public Ref<BigDecimal> getNegotiatedSpeedGbs() {
            return Ref.of(new BigDecimal(456));
        }

        public Ref<BigDecimal> getPredictedMediaLifeLeftPercent() {
            return Ref.of(new BigDecimal(456));
        }

        public Ref<String> getFirmwareVersion() {
            return Ref.of("1.17");
        }

        public Ref<Boolean> getDriveErased() {
            return Ref.of(true);
        }

        public ResourceSupplier getParentChassis() {
            return null;
        }

        public ResourceSupplier getStorage() {
            return null;
        }

        public ResourceSupplier getPcieDeviceFunction() {
            return null;
        }

        @Override
        public URI getUri() {
            return null;
        }

        @Override
        public void setUri(URI uri) {

        }

        @Override
        public void setWebClient(WebClient webClient) {

        }

        @Override
        public String getId() {
            return null;
        }

        @Override
        public String getName() {
            return "my name";
        }

        @Override
        public String getDescription() {
            return "my description";
        }
    }
}
