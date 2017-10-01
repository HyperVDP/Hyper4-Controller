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

#include <PI/pi.h>
#include <cstring>
#include <PI/frontends/cpp/tables.h>
#include "hyper4_controller.h"
#include "hyper4_setup.h"

#include <vector>
#include <iostream>
using namespace std;

using std::vector;

void
hyper4_controller::assign_device(char *port) {
    pi_assign_extra_t assign_options[2];
    memset(assign_options, 0, sizeof(assign_options));
    {
        pi_assign_extra_t *rpc_port = &assign_options[0];
        rpc_port->key = "port";
        rpc_port->v = port;
    }
    assign_options[1].end_of_extras = true;

    this->dev_id_list.push_back(this->dev_id_counter);

    pi_assign_device(this->dev_id_counter++, p4info, assign_options);
}

int
hyper4_controller::set_default_entries() {
    vector<pi_dev_id_t>::iterator i;
    for(i = dev_id_list.begin(); i != dev_id_list.end(); i++) {
        pi_dev_tgt_t dev_tgt = {*i, 0xFFFF};
        {
            pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, "t_norm_SEB");
            pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "a_norm_SEB");

            pi::ActionData action_data(p4info, a_id);
            pi::MatchTable mt(sess, dev_tgt, p4info, t_id);

            if (mt.default_entry_set(action_data) != PI_STATUS_SUCCESS) {
                std::cout << "Error when adding default entry to 't_norm_SEB'\n";
            }
        }
        {
            pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, "t_norm_20_39");
            pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "a_norm_20_39");

            pi::ActionData action_data(p4info, a_id);
            pi::MatchTable mt(sess, dev_tgt, p4info, t_id);

            if (mt.default_entry_set(action_data) != PI_STATUS_SUCCESS) {
                std::cout << "Error when adding default entry to 't_norm_20_39'\n";
            }
        }
        {
            pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, "t_norm_40_59");
            pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "a_norm_40_59");

            pi::ActionData action_data(p4info, a_id);
            pi::MatchTable mt(sess, dev_tgt, p4info, t_id);

            if (mt.default_entry_set(action_data) != PI_STATUS_SUCCESS) {
                std::cout << "Error when adding default entry to 't_norm_40_59'\n";
            }
        }
        {
            pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, "t_norm_60_79");
            pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "a_norm_60_79");

            pi::ActionData action_data(p4info, a_id);
            pi::MatchTable mt(sess, dev_tgt, p4info, t_id);

            if (mt.default_entry_set(action_data) != PI_STATUS_SUCCESS) {
                std::cout << "Error when adding default entry to 't_norm_60_79'\n";
            }
        }
        {
            pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, "t_norm_80_99");
            pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "a_norm_80_99");

            pi::ActionData action_data(p4info, a_id);
            pi::MatchTable mt(sess, dev_tgt, p4info, t_id);

            if (mt.default_entry_set(action_data) != PI_STATUS_SUCCESS) {
                std::cout << "Error when adding default entry to 't_norm_80_99'\n";
            }
        }
        {
            pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, "t_prep_deparse_SEB");
            pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "a_prep_deparse_SEB");

            pi::ActionData action_data(p4info, a_id);
            pi::MatchTable mt(sess, dev_tgt, p4info, t_id);

            if (mt.default_entry_set(action_data) != PI_STATUS_SUCCESS) {
                std::cout << "Error when adding default entry to 't_prep_deparse_SEB'\n";
            }
        }
        {
            pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, "mc_skip");
            pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "a_mc_skip");

            pi::ActionData action_data(p4info, a_id);
            pi::MatchTable mt(sess, dev_tgt, p4info, t_id);

            if (mt.default_entry_set(action_data) != PI_STATUS_SUCCESS) {
                std::cout << "Error when adding default entry to 'mc_skip'\n";
            }
        }
    }
}


void
hyper4_controller::add_entry(pi_dev_id_t dev_id,
                             const char *table_name,
                             MatchKey & key,
                             ActionData & data) {

    pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, table_name);
    pi_dev_tgt_t dev_tgt = { dev_id, 0xFFFF};

    MatchTable table(sess, dev_tgt, p4info, t_id);

    table.entry_add(key, data, true, &entry_handler);
}

void
hyper4_controller::set_first_table(pi_dev_id_t dev_id,
                                   uint16_t port,
                                   uint8_t src,
                                   uint8_t dst,
                                   uint8_t program_id,
                                   uint8_t first_table) {
    hyper4_setup setup(dev_id,
                       program_id,
                       this);

    setup.set_first_table(first_table);
    setup.set_program(port, src, dst);
    setup.set_header_length(34);
}