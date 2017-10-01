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

#include "hyper4_match.h"
#include "hyper4.h"
#include "hyper4_utils.h"

void
hyper4_match::init_program_state(uint8_t action_id,
                                 uint32_t match_id,
                                 uint8_t next_table) {

    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                    "init_program_state");
    this->action = new ActionData(p4info, a_id);


    pi_p4_id_t p;

    p = pi_p4info_action_param_id_from_name(p4info,
                                            a_id,
                                            "action_ID");
    this->action->set_arg(p, action_id);

    p = pi_p4info_action_param_id_from_name(p4info,
                                            a_id,
                                            "match_ID");
    this->action->set_arg(p, match_id);

    p = pi_p4info_action_param_id_from_name(p4info,
                                            a_id,
                                            "next_table");
    this->action->set_arg(p, next_table);
}

MatchKey
hyper4_header_match::get_match_key() {
    MatchKey key(p4info, this->table_id);

    pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                 table_id,
                                                                 "meta_ctrl.program");
    key.set_exact(key_id, program_id);

    key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                      table_id,
                                                      "extracted.data");
    key.set_ternary(key_id,
                    (const char*) hdr,
                    (const char*) hdr_mask,
                    100);
    key.set_priority(this->priority);
    return key;
}

void
hyper4_header_match::set_value(int offset, uint64_t value, int width) {
    byte_buffer bytes(this->hdr, HDR_SIZE, HDR_SIZE);
    bytes.set_value(offset, value, width);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, HDR_SIZE);
    mask_bytes.set_value(offset, 0xFFFFFFFFFFFFFFFFULL, width);
}

void
hyper4_header_match::set_value(int offset, unsigned int value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, HDR_SIZE);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, HDR_SIZE);
    mask_bytes.set_value(offset, (unsigned int) 0xFFFFFFFF);
}

void
hyper4_header_match::set_value(int offset, uint16_t value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, HDR_SIZE);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, HDR_SIZE);
    mask_bytes.set_value(offset, (uint16_t) 0xFFFF);
}

void
hyper4_header_match::set_value(int offset, uint8_t value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, HDR_SIZE);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, HDR_SIZE);
    mask_bytes.set_value(offset, (uint8_t) 0xFF);
}


void
hyper4_header_match::set_eth_type(uint16_t type) {
    set_value(ETH_TYPE_BYTE_OFFSET, type);
}

void
hyper4_header_match::set_ip_proto(uint8_t proto) {
    set_value(IP_PROTO_BYTE_OFFSET, proto);
}

void
hyper4_header_match::set_ip_dst(uint32_t dst) {
    set_value(IP_DST_BYTE_OFFSET, dst);
}

void
hyper4_header_match::set_eth_dst(uint64_t dst) {
    set_value(ETH_DST_BYTE_OFFSET, dst, 6);
}

void
hyper4_header_match::set_ip_src(uint32_t src) {
    set_value(IP_SRC_BYTE_OFFSET, src);
}

void
hyper4_metadata_match::set_value(int offset, unsigned int value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (unsigned int) 0xFFFFFFFF);
}

void
hyper4_metadata_match::set_value(int offset, uint16_t value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (uint16_t) 0xFFFF);

}

void
hyper4_metadata_match::set_value(int offset, uint8_t value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (uint8_t) 0xFF);
}


MatchKey
hyper4_metadata_match::get_match_key() {

    MatchKey key(p4info, this->table_id);
    pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                 table_id,
                                                                 "meta_ctrl.program");
    key.set_exact(key_id, program_id);
    key.set_ternary(key_meta_id,
                    (const char*)meta,
                    (const char*)meta_mask,
                    64);
    key.set_priority(this->priority);
    return key;
}

MatchKey
hyper4_intrinsic_match::get_match_key() {
    MatchKey key(p4info, this->table_id);
    pi_p4_id_t key_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                 table_id,
                                                                 "meta_ctrl.program");
    key.set_exact(key_id, program_id);


    key.set_priority(this->priority);
    return key;
}

void
hyper4_intrinsic_match::match_ingress_port(uint16_t port) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                    "set_meta_stdmeta");
    this->set_std_action = new ActionData(p4info, a_id);


    pi_p4_id_t p;

    p = pi_p4info_action_param_id_from_name(p4info,
                                            a_id,
                                            "stdmeta_ID");
    this->set_std_action->set_arg(p,
                                  STDMETA_INGRESS_PORT);


    pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info,
                                                    get_ingress_port_table_name().c_str());

    this->match_key = new MatchKey(p4info, t_id);



    p = pi_p4info_table_match_field_id_from_name(p4info, t_id, "meta_ctrl.program");


    this->match_key->set_exact(p, program_id);

    p = pi_p4info_table_match_field_id_from_name(p4info, t_id, "standard_metadata.ingress_port");

    this->match_key->set_exact(p, port);

    this->match_table_name = get_ingress_port_table_name();



}

void hyper4_intrinsic_match::install(hyper4_controller *controller,
                                     pi_dev_id_t dev_id) {

    {

        MatchKey key = get_match_key();

        controller->add_entry(dev_id,
                              get_table_name().c_str(),
                              key,
                              *set_std_action);
    }

    {
        ActionData action = get_action_data();
        controller->add_entry(dev_id,
                              match_table_name.c_str(),
                              *match_key,
                              action);
    }
}

void
hyper4_intrinsic_match::match_egress_spec(uint16_t port) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                    "set_meta_stdmeta");
    this->set_std_action = new ActionData(p4info, a_id);


    pi_p4_id_t p;

    p = pi_p4info_action_param_id_from_name(p4info,
                                            a_id,
                                            "stdmeta_ID");
    this->set_std_action->set_arg(p,
                                  STDMETA_EGRESS_SPEC);



    pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info,
                                                   get_egress_spec_table_name().c_str());
    this->match_key = new MatchKey(p4info, t_id);

    p = pi_p4info_table_match_field_id_from_name(p4info,
                                                 t_id,
                                                 "meta_ctrl.program");

    this->match_key->set_exact(p, program_id);

    p = pi_p4info_table_match_field_id_from_name(p4info,
                                                 t_id,
                                                 "standard_metadata.egress_spec");

    this->match_key->set_exact(p, port);

    this->match_table_name = get_egress_spec_table_name();

}

void
hyper4_header_match::set_eth_src(uint64_t src) {
    set_value(ETH_SRC_BYTE_OFFSET, src, 6);
}
