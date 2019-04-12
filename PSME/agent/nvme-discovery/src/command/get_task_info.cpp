/*!
 * @copyright
 * Copyright (c) 2017-2019 Intel Corporation
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
 * @file get_task_info.cpp
 * */

#include "nvme_agent_commands.hpp"
#include "agent-framework/module/common_components.hpp"

using namespace agent::nvme;
using namespace agent_framework;
using namespace agent_framework::module;
using namespace agent_framework::model;

REGISTER_NVME_COMMAND(GetTaskInfo,
     [] (GetTasksCollection::ContextPtr, const GetTaskInfo::Request& req, GetTaskInfo::Response& rsp) {
         log_debug("nvme-discovery-agent", "Getting Task info");
         rsp = get_manager<Task>().get_entry(req.get_uuid());
     }
);
