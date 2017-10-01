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

#include "hyper4_setup.h"
#include "hyper4.h"

void
hyper4_setup::set_header_length(uint8_t header_length) {
    pi_p4info_t *p4info = controller->get_p4info();
    {

        pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info,
                                                       "parse_control");


        MatchKey key(p4info, t_id);

        pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                     t_id,
                                                                     "meta_ctrl.program");
        key.set_exact(key_id,
                      program_id);
        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "parse_ctrl.numbytes");
        key.set_exact(key_id,
                      (uint8_t) 0);


        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "parse_ctrl.state");
        key.set_exact(key_id,
                      (uint8_t) 0);

        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "extract_more");
        ActionData action(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "numbytes");
        action.set_arg(p, header_length);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "state");
        action.set_arg(p,
                       (uint8_t) 1);

        controller->add_entry(dev_id, "parse_control", key, action);

    }
    {

        pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info,
                                                       "parse_control");


        MatchKey key(p4info, t_id);

        pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                     t_id,
                                                                     "meta_ctrl.program");
        key.set_exact(key_id,
                      program_id);
        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "parse_ctrl.numbytes");
        key.set_exact(key_id, header_length);


        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "parse_ctrl.state");
        key.set_exact(key_id, (uint8_t) 1);

        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "set_next_action");
        ActionData action(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_action");
        action.set_arg(p, PROCEED);

        controller->add_entry(dev_id, "parse_control", key, action);

    }
    {
        pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info,
                                                       "t_set_validbits");


        MatchKey key(p4info, t_id);

        pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                     t_id,
                                                                     "meta_ctrl.program");
        key.set_exact(key_id,
                      program_id);


        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "parse_ctrl.state");
        key.set_exact(key_id, (uint8_t) 1);

        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "a_set_validbits");
        ActionData action(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "val");
        action.set_arg(p, header_length);

        controller->add_entry(dev_id,
                              "t_set_validbits",
                              key,
                              action);
    }
}

void
hyper4_setup::set_first_table(uint8_t table_id) {
    pi_p4info_t *p4info = controller->get_p4info();
    {

        pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info,
                                                       "t_set_first_table");


        MatchKey key(p4info, t_id);

        pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                     t_id,
                                                                     "meta_ctrl.program");
        key.set_exact(key_id, program_id);

        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "parse_ctrl.state");
        key.set_exact(key_id, (uint8_t) 1);

        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "a_set_first_table");
        ActionData action(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "tableID");
        action.set_arg(p, table_id);

        controller->add_entry(dev_id, "t_set_first_table", key, action);
    }
}

void hyper4_setup::set_program(uint16_t port, uint8_t src, uint8_t dst) {
    pi_p4info_t *p4info = controller->get_p4info();
    {

        pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info,
                                                       "t_prog_select");


        MatchKey key(p4info, t_id);

        pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                     t_id,
                                                                     "standard_metadata.ingress_port");
        key.set_exact(key_id, port);

        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "ext[3].data");
        key.set_ternary(key_id,
                        dst,
                        (uint8_t) 0xff);

        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "ext[9].data");
        key.set_ternary(key_id,
                        src,
                        (uint8_t) 0xff);

        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "set_program");
        ActionData action(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "program");
        action.set_arg(p,
                       program_id);

        p = pi_p4info_action_param_id_from_name(p4info, a_id, "virt_ingress_port");
        action.set_arg(p, (uint8_t) 0);

        controller->add_entry(dev_id,
                              "t_prog_select",
                              key,
                              action);
    }
}