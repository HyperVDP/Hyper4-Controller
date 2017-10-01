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

#ifndef CONTROLLER_HYPER4_CONTROLLER_H
#define CONTROLLER_HYPER4_CONTROLLER_H


#include <PI/pi_base.h>
#include <vector>

#include <PI/pi.h>
#include <PI/frontends/generic/pi.h>
#include <PI/frontends/cpp/tables.h>

using namespace pi;

class hyper4_controller {
private:
    uint8_t action_ids[256] = {0};
    uint32_t match_ids[256] = {0};
    uint8_t program_id_counter = 0;
    pi_p4info_t * p4info;
    std::vector<pi_dev_id_t> dev_id_list;
    pi_dev_id_t dev_id_counter = 0;
    pi_session_handle_t sess;
    pi_entry_handle_t  entry_handler;
public:
    hyper4_controller(pi_p4info_t * p4info) {
        this->p4info = p4info;
        pi_session_init(&sess);
    }


    /**
     *
     * @param dev_id
     * @param table_name
     * @param key
     * @param data
     */
    void add_entry(pi_dev_id_t dev_id,
                   const char* table_name,
                   MatchKey & key,
                   ActionData & data);

    /**
     *
     * @param port
     */
    void assign_device(char* port);

    /**
     *
     * @return
     */
    pi_p4info_t * get_p4info() {
        return p4info;
    }


    /**
     *
     * @param i
     * @return
     */
    pi_dev_id_t get_dev_id(unsigned long i) {
        return dev_id_list[i];
    }

    /**
     *
     * @return
     */
    int
    set_default_entries();


    uint8_t allocate_new_action_id(uint8_t program_id) {
        return ++action_ids[program_id];
    }

    uint32_t allocate_new_match_id(uint8_t program_id) {
        return ++match_ids[program_id];
    }

    /**
     *
     * @param dev_id
     * @param program_id
     */
    void set_first_table(pi_dev_id_t dev_id,
                         uint16_t port,
                         uint8_t src,
                         uint8_t dst,
                         uint8_t program_id,
                         uint8_t first_table);
    /**
     *
     * @return
     */
    uint8_t get_new_program_id() {
        return ++this->program_id_counter;
    }

};


#endif //CONTROLLER_HYPER4_CONTROLLER_H
