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

#include "hyper4_action.h"
#include <sstream>
#include <iostream>


void
hyper4_action::install(pi_dev_id_t dev_id) {
    int i = 0;
    for(; i < primitives.size(); i ++) {
        primitives[i]->set_primitive_metadata(dev_id);
        primitives[i]->install_primitive(dev_id);

        primitives[i]->set_program_state(dev_id);
    }
}

void
hyper4_action::add_primitive(hyper4_primitive * primitive) {

    primitive_index ++;
    primitive->set_index(primitive_index);

    if (primitives.size() > 0) {
        auto i = primitives.end() - 1;
        (*i)->set_next_index(primitive_index);
    }

    primitive->set_index(primitive_index);
    primitives.push_back(primitive);
}

void
hyper4_action::add_forward(uint16_t port) {
    auto * mod = new do_modify_field(program_id,
                                     stage_id, action_id,
                                     match_id,
                                     MOD_EGRESS_SPEC,
                                     next_stage_id,
                                     controller);
    mod->set_egress_spec(port);
    add_primitive(mod);
}



void
hyper4_action::add_multicast(uint8_t seq_id, uint16_t port) {
    auto * multicast = new do_multicast(program_id,
                                        stage_id, action_id,
                                        match_id,
                                        0,
                                        next_stage_id,
                                        controller);
    multicast->set_multicast(seq_id, port);
    add_primitive(multicast);
}

void
hyper4_primitive::set_next_index(uint8_t index) {
    this->next_primitive_index = index;
}

void
hyper4_primitive::set_index(uint8_t index) {
    this->primitive_index = index;
}

void
do_modify_field::install_primitive(pi_dev_id_t dev_id) {
    pi_p4info_t *p4info = controller->get_p4info();
    {

        pi_p4_id_t t_id =
                pi_p4info_table_id_from_name(p4info,
                                             get_primitive_table_name().c_str());

        MatchKey key(p4info, t_id);

        pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                     t_id,
                                                                     "meta_ctrl.program");
        key.set_exact(key_id, this->program_id);

        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "meta_primitive_state.subtype");
        key.set_exact(key_id, this->subtype);

        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "meta_primitive_state.match_ID");

        key.set_exact(key_id,
                      this->match_id);

        controller->add_entry(dev_id,
                              get_primitive_table_name().c_str(),
                              key,
                              *action);
    }
}

string
do_modify_field::get_primitive_table_name() {
    std::ostringstream ostr;
    ostr << "t_mod_";
    ostr << (int) stage_id;
    ostr << (int) primitive_index;
    return ostr.str();
}

void
hyper4_primitive::set_primitive_metadata(pi_dev_id_t dev_id) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t t_id =
                pi_p4info_table_id_from_name(p4info,
                                             get_primitive_metadata_table_name().c_str());
        MatchKey key(p4info, t_id);

        pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                     t_id,
                                                                     "meta_ctrl.program");
        key.set_exact(key_id, this->program_id);

        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "meta_primitive_state.action_ID");
        key.set_exact(key_id, this->action_id);

        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "meta_primitive_state.primitive_index");
        key.set_exact(key_id, this->primitive_index);

        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "a_set_primitive_metadata");
        ActionData action(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "primitive");
        action.set_arg(p, this->primitive);

        p = pi_p4info_action_param_id_from_name(p4info, a_id, "primitive_subtype");
        action.set_arg(p, this->subtype);

        controller->add_entry(dev_id,
                              get_primitive_metadata_table_name().c_str(),
                              key,
                              action);
    }
}

void
hyper4_primitive::set_program_state(pi_dev_id_t dev_id) {
    pi_p4info_t *p4info = controller->get_p4info();
    {

        pi_p4_id_t t_id =
                pi_p4info_table_id_from_name(p4info,
                                             get_program_state_table_name().c_str());

        MatchKey key(p4info, t_id);

        pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                     t_id,
                                                                     "meta_ctrl.program");
        key.set_exact(key_id, this->program_id);

        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "meta_primitive_state.action_ID");

        key.set_exact(key_id, this->action_id);

        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "meta_primitive_state.primitive_index");
        key.set_exact(key_id, this->primitive_index);

        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "set_program_state");
        ActionData action(p4info, a_id);


        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "action_ID");
        action.set_arg(p, this->action_id);

        p = pi_p4info_action_param_id_from_name(p4info, a_id, "primitive_index");
        action.set_arg(p, this->next_primitive_index);

        p = pi_p4info_action_param_id_from_name(p4info, a_id, "stage_state");

        if (this->next_primitive_index == 0) {
            action.set_arg(p, COMPLETE);
        }
        else {
            action.set_arg(p, CONTINUE);
        }

        p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_stage");
        action.set_arg(p, next_stage_id);

        controller->add_entry(dev_id,
                              get_program_state_table_name().c_str(),
                              key,
                              action);
    }
}

void
do_modify_field::set_egress_spec(uint16_t port) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "mod_stdmeta_egressspec_const");
        this->action = new ActionData(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "val");
        this->action->set_arg(p, port);

    }
}

void
do_modify_field::set_eth_src(uint64_t type) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "mod_extracted_const");
        this->action = new ActionData(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "val");
        this->action->set_arg(p, type);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "leftshift");
        this->action->set_arg(p, 0ul);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "emask");
        this->action->set_arg(p, 0xFFFFFFFFFFFFFFFFul);
    }
}

void
do_modify_field::set_eth_dst(uint64_t type) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "mod_extracted_const");
        this->action = new ActionData(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "val");
        this->action->set_arg(p, type);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "leftshift");
        this->action->set_arg(p, 0ul);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "emask");
        this->action->set_arg(p, 0xFFFFFFFFFFFFFFFFul);
    }
}

void
do_modify_field::set_meta_const(char (&val)[32],
                                char (&left_shift)[32],
                                char (&tmeta_mask)[32]) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "mod_meta_const");
        this->action = new ActionData(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "val");
        this->action->set_arg(p, val, sizeof(val));

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "leftshift");
        this->action->set_arg(p, left_shift, sizeof(left_shift));

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "tmeta_mask");
        this->action->set_arg(p, tmeta_mask, sizeof(tmeta_mask));
    }
}


void
do_modify_field::set_meta_const(uint32_t val,
                                uint32_t left_shift,
                                uint32_t tmeta_mask) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "mod_meta_const");
        this->action = new ActionData(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "val");
        this->action->set_arg(p, val);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "leftshift");
        this->action->set_arg(p, left_shift);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "tmeta_mask");
        this->action->set_arg(p, tmeta_mask);
    }
}


void do_modify_field::set_meta_const(uint16_t val,
                                     uint16_t left_shift,
                                     uint16_t tmeta_mask) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "mod_meta_const");
        this->action = new ActionData(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "val");
        this->action->set_arg(p, val);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "leftshift");
        this->action->set_arg(p, left_shift);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "tmeta_mask");
        this->action->set_arg(p, tmeta_mask);
    }
}


void do_modify_field::set_meta_const(uint8_t val,
                                     uint8_t left_shift,
                                     uint8_t tmeta_mask) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "mod_meta_const");
        this->action = new ActionData(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "val");
        this->action->set_arg(p, val);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "leftshift");
        this->action->set_arg(p, left_shift);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "tmeta_mask");
        this->action->set_arg(p, tmeta_mask);
    }
}

string
do_math_on_field::get_primitive_table_name() {
    std::ostringstream ostr;
    ostr << "t_math_on_field_";
    ostr << (int) stage_id;
    ostr << (int) primitive_index;
    return ostr.str();
}

void
do_math_on_field::install_primitive(pi_dev_id_t dev_id) {
    pi_p4info_t *p4info = controller->get_p4info();
    {

        pi_p4_id_t t_id =
                pi_p4info_table_id_from_name(p4info,
                                             get_primitive_table_name().c_str());

        MatchKey key(p4info, t_id);

        pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                     t_id,
                                                                     "meta_ctrl.program");
        key.set_exact(key_id, this->program_id);

        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "meta_primitive_state.subtype");
        key.set_exact(key_id, this->subtype);

        key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                          t_id,
                                                          "meta_primitive_state.match_ID");

        key.set_exact(key_id,
                      this->match_id);

        controller->add_entry(dev_id,
                              get_primitive_table_name().c_str(),
                              key,
                              *action);
    }
}

void
do_math_on_field::set_subff_extracted_const_u(char (&val)[100], char (&left_shift)[100]) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "a_subff_extracted_const_u");
        this->action = new ActionData(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "val");
        this->action->set_arg(p, val, sizeof(val));

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "leftshift");

        this->action->set_arg(p, left_shift, sizeof(left_shift));
    }
}


void
do_math_on_field::set_subff_extracted_const_u(uint8_t val, uint8_t right_byte_shift) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "a_subff_extracted_const_u");
        this->action = new ActionData(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "val");
        this->action->set_arg(p, val);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "leftshift");

        right_byte_shift = (uint8_t)(100 - right_byte_shift);
        uint16_t left_shift =  (uint16_t) (right_byte_shift * 8);
        this->action->set_arg(p, left_shift);
    }
}

string
do_drop::get_primitive_table_name() {
    std::ostringstream ostr;
    ostr << "t_drop_";
    ostr << (int) stage_id;
    ostr << (int) primitive_index;
    return ostr.str();
}

void do_drop::install_primitive(pi_dev_id_t dev_id) {
    pi_p4info_t *p4info = controller->get_p4info();
    {

        pi_p4_id_t t_id =
                pi_p4info_table_id_from_name(p4info,
                                             get_primitive_table_name().c_str());

        MatchKey key(p4info, t_id);

        controller->add_entry(dev_id,
                              get_primitive_table_name().c_str(),
                              key,
                              *action);
    }
}

void do_drop::set_drop() {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "a_drop");
        this->action = new ActionData(p4info, a_id);
    }
}

void do_multicast::set_multicast(uint8_t seq_id, uint16_t port) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                        "a_multicast");
        this->action = new ActionData(p4info, a_id);

        pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info,
                                                           a_id,
                                                           "seq_id");
        this->action->set_arg(p, seq_id);

        p = pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "highport");
        this->action->set_arg(p, port);

    }
}

string
do_multicast::get_primitive_table_name() {
    std::ostringstream ostr;
    ostr << "t_multicast_";
    ostr << (int) stage_id;
    ostr << (int) primitive_index;
    return ostr.str();
}

void
do_multicast::install_primitive(pi_dev_id_t dev_id) {
    pi_p4info_t *p4info = controller->get_p4info();
    {

        pi_p4_id_t t_id =
                pi_p4info_table_id_from_name(p4info,
                                             get_primitive_table_name().c_str());

        MatchKey key(p4info, t_id);

        pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                     t_id,
                                                                     "meta_ctrl.program");
        key.set_exact(key_id,
                      this->match_id);

        controller->add_entry(dev_id,
                              get_primitive_table_name().c_str(),
                              key,
                              *action);
    }
}

void
no_op::install_primitive(pi_dev_id_t dev_id) {
    // Nothing
}


string
no_op::get_primitive_table_name() {
    return "";
}


void
hyper4_action::add_no_op() {
    auto * nop = new no_op(program_id,
                             stage_id, action_id,
                             match_id,
                             0,
                             next_stage_id,
                             controller);
    add_primitive(nop);
}

void hyper4_action::add_mod_eth_dst(uint64_t dst) {
    auto * mod = new do_modify_field(program_id,
                                     stage_id, action_id,
                                     match_id,
                                     MOD_HEADER,
                                     next_stage_id,
                                     controller);
    mod->set_eth_dst(dst);
    add_primitive(mod);
}

void
hyper4_action::add_mod_eth_src(uint64_t src) {
    auto * mod = new do_modify_field(program_id,
                                     stage_id, action_id,
                                     match_id,
                                     MOD_HEADER,
                                     next_stage_id,
                                     controller);
    mod->set_eth_src(src);
    add_primitive(mod);
}

void
hyper4_action::add_drop() {
    auto * drop = new do_drop(program_id,
                             stage_id,
                             action_id,
                             match_id,
                             0,
                             next_stage_id,
                             controller);
    drop->set_drop();
    add_primitive(drop);
}

void
hyper4_action::add_sub_const(uint8_t val, uint8_t right_byte_shift) {
    auto * math = new do_math_on_field(program_id,
                                       stage_id,
                                       action_id,
                                       match_id,
                                       MATH_SUB_HEADER,
                                       next_stage_id,
                                       controller);
    math->set_subff_extracted_const_u(val, right_byte_shift);
    add_primitive(math);
}


void hyper4_action::add_sub_ttl(uint8_t val) {
    add_sub_const(val, TTL_BYTE_OFFSET);
}

void hyper4_action::add_mod_meta_const(uint8_t val,
                                       uint8_t left_shift,
                                       uint8_t tmeta_mask) {
    auto * mod = new do_modify_field(program_id,
                                     stage_id, action_id,
                                     match_id,
                                     MOD_META,
                                     next_stage_id,
                                     controller);
    mod->set_meta_const(val, left_shift, tmeta_mask);
    add_primitive(mod);
}