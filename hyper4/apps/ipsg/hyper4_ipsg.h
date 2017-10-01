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

#ifndef CONTROLLER_HYPER4_IPSG_H
#define CONTROLLER_HYPER4_IPSG_H


#include "../hyper4_app.h"
#include "../../hyper4.h"

class hyper4_ipsg : public hyper4_app {
public:
    hyper4_ipsg(pi_dev_id_t dev_id,
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
     * @param src_ip
     * @param iport
     */
    void install(uint32_t src_ip,
                 uint16_t iport);

    void activate() override ;
    void deactivate() override ;

    uint8_t get_table_type() override {
        return STDMETA_EXACT;
    }

    uint8_t get_stage_num() {
        return 2;
    }

};


#endif //CONTROLLER_HYPER4_IPSG_H
