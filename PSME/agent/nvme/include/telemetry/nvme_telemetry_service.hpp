/*!
 * @brief Nvme agent's TelemetryService declaration.
 *
 * @header{License}
 * @copyright Copyright (c) 2017-2018 Intel Corporation.
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
 * @header{Filesystem}
 * @file compute/include/telemetry/telemetry_service.hpp
 */

#pragma once

#include "tools/drive_discovery/base_drive_handler.hpp"
#include "nvme_agent_context.hpp"
#include "logger/logger_factory.hpp"
#include "agent-framework/eventing/events_queue.hpp"
#include "agent-framework/module/model/resource.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "agent-framework/module/model/metric.hpp"

#include <memory>
#include <type_traits>

namespace agent_framework {
namespace model {
class Drive;
}
}

namespace agent {
namespace nvme {
namespace telemetry {

/*!
 * @brief Nvme telemetry service.
 */
class TelemetryService final {
public:

    /*!
     * @brief Defaulted constructor.
     */
    TelemetryService() = default;

    /*!
     * @brief Defaulted copy constructor.
     */
    TelemetryService(const TelemetryService& rhs) = default;

    /*!
     * @brief Defaulted move constructor.
     */
    TelemetryService(TelemetryService&& rhs) = default;

    /*!
     * @brief Defaulted assignment operator.
     */
    TelemetryService& operator=(const TelemetryService& rhs) = default;

    /*!
     * @brief Defaulted move assignment operator.
     */
    TelemetryService& operator=(TelemetryService&& rhs) = default;

    /*!
     * @brief Defaulted destructor.
     */
    ~TelemetryService() = default;

    /*!
     * @brief Initializes the nvme telemetry.
     *
     * @param context The context for the initialization operation.
     */
    void init(std::shared_ptr<NvmeAgentContext> context) const;

    /*!
     * @brief Updates nvme related metrics.
     *
     * @param context The context for metrics update.
     */
    void update_metrics(std::shared_ptr<NvmeAgentContext> context) const;
private:

    /*!
     * @brief Create metrics for the given drive.
     *
     * @param drive The model drive to create metrics for.
     * @param smart The drive's SMART data.
     */
    void create_drive_metrics(const agent_framework::model::Drive& drive,
        const tools::BaseDriveHandler::SmartData& smart) const;

    /*!
     * @brief Registers metric definitions handled by this telemetry service.
     */
    void register_metric_definitions() const;

    /*!
     * @brief Add or update a resource to the model.
     *
     * @param resource The resource to be added or updated.
     */
    template <typename T, typename std::enable_if<std::is_base_of<agent_framework::model::Resource, T>{}, int>::type = 0>
    void add_or_update(const T& resource) const {
        const auto& status = agent_framework::module::get_manager<T>().add_or_update_entry(resource);
        agent_framework::eventing::EventData edat{};
        edat.set_parent(resource.get_parent_uuid());
        edat.set_component(resource.get_uuid());
        edat.set_type(resource.get_component());

        if (agent_framework::module::TableInterface::UpdateStatus::Added == status) {
            edat.set_notification(agent_framework::eventing::Notification::Add);
            log_info("telemetry",
                "Added " << T::get_component().to_string() << ", UUID " << resource.get_uuid());
            agent_framework::eventing::EventsQueue::get_instance()->push_back(std::move(edat));
        }
        else if (agent_framework::module::TableInterface::UpdateStatus::StatusChanged == status
            || agent_framework::module::TableInterface::UpdateStatus::Updated == status) {
            edat.set_notification(agent_framework::eventing::Notification::Update);
            log_info("telemetry",
                "Updated " << T::get_component().to_string() << ", UUID " << resource.get_uuid());
            agent_framework::eventing::EventsQueue::get_instance()->push_back(std::move(edat));
        }
        else {
            return;
        }
    }

    /*!
     * @brief Update drive health.
     *
     * @param model_drive The drive to be updated.
     * @param smart The drive's SMART data.
     */
    void update_drive_health(agent_framework::model::Drive &model_drive,
        const tools::BaseDriveHandler::SmartData& smart) const;

    /*!
     * @brief Sets drive's health to critical
     * @param drive_uuid Uuid of the drive to be changed
     */
    void set_drive_health_to_critical(const Uuid& drive_uuid) const;
};

template<>
void TelemetryService::add_or_update(const agent_framework::model::Metric& metric) const;

} // namespace telemetry
} // namespace nvme
} // namespace agent
