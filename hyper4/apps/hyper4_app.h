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

#ifndef CONTROLLER_HYPER4_APP_H
#define CONTROLLER_HYPER4_APP_H


#include <cstdint>
#include "../hyper4_controller.h"

class hyper4_app {
protected:
    uint8_t program_id;
    uint8_t start_stage;
    pi_dev_id_t  dev_id;
    hyper4_controller * controller;
    uint8_t next_stage_id;
    uint8_t next_stage_type;
    bool first_app;
public:
    /**
     *
     * @param dev_id
     * @param program_id
     * @param start_stage
     * @param controller
     */
    hyper4_app(pi_dev_id_t dev_id,
               uint8_t program_id,
               uint8_t start_stage,
               hyper4_controller * controller) {
        this->program_id = program_id;
        this->start_stage = start_stage;
        this->controller = controller;
        this->dev_id = dev_id;
        this->next_stage_id = 0;
    }

    void set_next_stage(uint8_t table) {
        this->next_stage_id = table;

    }

    hyper4_app &operator>>(hyper4_app& app) {
        this->next_stage_id = app.start_stage;
        this->next_stage_type = app.get_table_type();
        return app;
    }

    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual uint8_t get_table_type() = 0;
    virtual uint8_t get_stage_num() = 0;
};


#endif //CONTROLLER_HYPER4_APP_H
