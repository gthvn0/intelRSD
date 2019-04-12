/*!
 * @copyright
 * Copyright (c) 2018-2019 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file nvmf_subsystem_add_listener_connector.hpp
 */

#include "json-rpc/json_rpc_response.hpp"
#include "json-rpc/connectors/abstract_client_connector.hpp"
#include "json-rpc/handlers/json_rpc_request_invoker.hpp"



namespace spdk {

class NvmfSubsystemAddListenerConnector : public json_rpc::AbstractClientConnector {
public:
    static constexpr const char SUBSYSTEM_NQN[] = "nqn.2016-06.io.spdk:test";

    /*! Constructor */
    NvmfSubsystemAddListenerConnector();

    virtual std::string send_request(const std::string& message) override;

private:
    json::Json nvmf_subsystem{};

};

}


