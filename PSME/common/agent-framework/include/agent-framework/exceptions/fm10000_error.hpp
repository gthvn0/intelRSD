/*!
 * @copyright
 * Copyright (c) 2015-2018 Intel Corporation
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
 *
 * @file fm10000_error.hpp
 *
 * @brief Declaration of FM10000 error interface
 * */

#pragma once



#include "agent-framework/exceptions/gami_exception.hpp"



namespace agent_framework {
namespace exceptions {

/*! @brief Fm10000 exception class */
class Fm10000Error final : public GamiException {
public:

    /*!
     * @brief Constructor
     *
     * @param[in] msg Error message
     * */
    explicit Fm10000Error(const std::string& msg) :
        GamiException(ErrorCode::FM10000, msg) {}


    Fm10000Error(const Fm10000Error&) = default;


    Fm10000Error& operator=(const Fm10000Error&) = default;


    Fm10000Error(Fm10000Error&&) = default;


    Fm10000Error& operator=(Fm10000Error&&) = default;


    /*! @brief Destructor */
    ~Fm10000Error();
};

} // namespace exceptions
} // namespace agent_framework

