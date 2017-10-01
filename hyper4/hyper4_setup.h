/* Copyright 2016-present NetArch Lab, Tsinghua University.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CONTROLLER_HYPERV_SETUP_H
#define CONTROLLER_HYPERV_SETUP_H


#include <cstdint>
#include "hyper4_controller.h"

class hyper4_setup {
private:
    uint8_t program_id;
    hyper4_controller * controller;
    pi_dev_id_t  dev_id;
public:
    hyper4_setup (pi_dev_id_t dev_id,
                  uint8_t program_id,
                  hyper4_controller* controller) {
        this->program_id = program_id;
        this->controller = controller;
        this->dev_id = dev_id;

    }

    /**
     *
     * @param packet_header_length
     */
    void set_header_length(uint8_t packet_header_length);

    /**
     *
     * @param table_id
     */
    void set_first_table(uint8_t table_id);

    /**
     *
     */
    void set_program(uint16_t port,
                     uint8_t src,
                     uint8_t dst);

};


#endif //CONTROLLER_HYPERV_SETUP_H
