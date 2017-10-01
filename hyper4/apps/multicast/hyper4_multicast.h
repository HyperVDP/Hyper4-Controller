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

#ifndef CONTROLLER_HYPER4_MULTICAST_H
#define CONTROLLER_HYPER4_MULTICAST_H


#include <PI/pi_base.h>
#include "../hyper4_app.h"
#include "../../hyper4.h"

class hyper4_multicast : public hyper4_app {
public:
    hyper4_multicast(pi_dev_id_t dev_id,
                     uint8_t program_id,
                     uint8_t start_stage,
                     hyper4_controller * controller) :
            hyper4_app(dev_id,
                       program_id,
                       start_stage,
                       controller) {

    }

    /**
     *
     * @param dst_ip
     * @param dst_mac
     * @param src_mac
     * @param port
     */
    void install(uint16_t dst_mac,
                 uint8_t seq_id,
                 uint16_t port);

    void activate() override ;
    void deactivate() override ;

    uint8_t get_table_type() override {
        return EXTRACTED_EXACT;
    }
    uint8_t get_stage_num() override {
        return 1;
    }

};


#endif //CONTROLLER_HYPER4_MULTICAST_H
