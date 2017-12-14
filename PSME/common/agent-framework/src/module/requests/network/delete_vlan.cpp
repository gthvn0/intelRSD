/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2015-2017 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @section DESCRIPTION
 *
 * @file requests/network/delete_vlan.cpp
 *
 * @brief Network request DeleteVlan implementation
 * */

#include "agent-framework/module/requests/network/delete_vlan.hpp"
#include "agent-framework/module/constants/network.hpp"
#include "agent-framework/module/constants/common.hpp"
#include "json-wrapper/json-wrapper.hpp"

using namespace agent_framework::model::requests;
using namespace agent_framework::model::literals;

DeleteVlan::DeleteVlan(const std::string& vlan,
            agent_framework::model::attribute::Oem):
                m_vlan(vlan),
                m_oem{} {}

json::Json DeleteVlan::to_json() const {
    json::Json value;
    value[Vlan::VLAN] = m_vlan;
    value[Oem::OEM] = m_oem.to_json();
    return value;
}

DeleteVlan DeleteVlan::from_json(const json::Json& json) {
    return DeleteVlan{
        json[Vlan::VLAN],
        agent_framework::model::attribute::Oem{}

    };
}