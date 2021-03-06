/*!
 * @brief Drive discoverer interface.
 *
 * @copyright Copyright (c) 2018-2019 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file spdk_drive_discoverer.hpp
 */

#pragma once



#include "agent-framework/discovery/discoverers/drive_discoverer.hpp"
#include "spdk_agent_context.hpp"



namespace agent {
namespace spdk {
namespace discovery {

class SpdkDriveDiscoverer : agent_framework::discovery::DriveDiscoverer {
public:

    /*!
     * @brief Constructor
     * @param context SPDK agent context pointer.
     */
    SpdkDriveDiscoverer(AgentContext::SPtr context) : m_context(context) {}


    /*! Virtual destructor */
    virtual ~SpdkDriveDiscoverer() {}


    /*!
    * @brief Perform discovery of drive objects.
    * @param parent_uuid Parent UUID.
    * @return Drives collection filled by the method with discovered data.
    * */
    std::vector<agent_framework::model::Drive> discover(const Uuid& parent_uuid) override;


private:
    AgentContext::SPtr m_context{};

};

}
}
}


