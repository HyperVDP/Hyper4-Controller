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

#ifndef CONTROLLER_HYPER4_MATCH_H
#define CONTROLLER_HYPER4_MATCH_H


#include <cstdint>
#include <cstdio>
#include <PI/pi_base.h>
#include <PI/p4info/tables.h>
#include <string>
#include <PI/pi.h>
#include <PI/frontends/generic/pi.h>
#include <PI/frontends/cpp/tables.h>
#include <sstream>
#include <iostream>
#include "hyper4_controller.h"

using std::string;

using namespace pi;

class hyper4_match {
protected:
    pi_p4info_t * p4info;
    uint8_t program_id;
    uint8_t stage_id;
    pi_p4_id_t table_id;
    int priority;
    ActionData * action;
public:
    hyper4_match(pi_p4info_t * p4info,
                 uint8_t program_id,
                 uint8_t stage_id) {
        this->p4info = p4info;
        this->program_id = program_id;
        this->stage_id = stage_id;
        this->action = nullptr;
        this->priority = 100;
    }
    virtual string get_table_name() = 0;
    virtual MatchKey get_match_key() = 0;

    ActionData get_action_data() {
        return *action;
    }

    virtual void install(hyper4_controller * controller,
                 pi_dev_id_t dev_id) {


        ActionData action = get_action_data();

        MatchKey key = get_match_key();

        controller->add_entry(dev_id,
                              get_table_name().c_str(),
                              key,
                              action);
    }

    /**
     *
     * @param action_id
     * @param match_id
     * @param next_table
     */
    virtual void init_program_state(uint8_t action_id,
                                    uint32_t match_id,
                                    uint8_t next_table);
};


const size_t HDR_SIZE = 100;

class hyper4_header_match: public hyper4_match {

private:
    uint8_t hdr[HDR_SIZE] = {0};
    uint8_t hdr_mask[HDR_SIZE] = {0};
    pi_p4_id_t key_hdr_id;

public:
    /**
     *
     * @param p4info
     * @param inst_id
     * @param stage_id
     */
    hyper4_header_match(pi_p4info_t * p4info,
                        uint8_t program_id,
                        uint8_t stage_id):
            hyper4_match(p4info,
                         program_id,
                         stage_id) {
        table_id = pi_p4info_table_id_from_name(p4info, get_table_name().c_str());
        key_hdr_id =
                pi_p4info_table_match_field_id_from_name(p4info,
                                                         table_id,
                                                         "extracted.data");
    }

    string get_table_name() override {
        std::ostringstream ostr;
        ostr << "t";
        ostr << (int) stage_id;
        ostr << "_extracted_exact";
        return ostr.str();
    }

    /**
     *
     */
    void set_value(int, uint64_t, int);

    /**
     *
     */
    void set_value(int, unsigned int);

    /**
     *
     */
    void set_value(int, uint16_t);

    /**
     *
     * @param type
     */
    void set_eth_dst(uint64_t type);

    /**
     *
     * @param type
     */
    void set_eth_src(uint64_t src);

    /**
     *
     * @param type
     */
    void set_eth_type(uint16_t type);

    /**
     *
     * @param proto
     */
    void set_ip_proto(uint8_t proto);


    /**
     *
     * @param src
     */
    void set_ip_src(uint32_t src);


    /**
     *
     * @param dst
     */
    void set_ip_dst(uint32_t dst);

    /**
 *
 * @param priority
 */
    void set_priority(int priority) {
        this->priority = priority;
    }

    /**
     *
     */
    void set_value(int, uint8_t);

    MatchKey get_match_key() override;
};


class hyper4_metadata_match: public hyper4_match {

private:
    uint8_t meta[64] = {0};
    uint8_t meta_mask[64] = {0};
    pi_p4_id_t key_meta_id;
public:
    /**
     *
     * @param p4info
     * @param inst_id
     * @param stage_id
     */
    hyper4_metadata_match(pi_p4info_t * p4info,
                   uint8_t program_id,
                   uint8_t stage_id):
            hyper4_match(p4info,
                         program_id,
                         stage_id) {
        table_id = pi_p4info_table_id_from_name(p4info, get_table_name().c_str());
        key_meta_id =
                pi_p4info_table_match_field_id_from_name(p4info,
                                                         this->table_id,
                                                         "tmeta.data");

    }

    string get_table_name() override {
        std::ostringstream ostr;
        ostr << "t";
        ostr << (uint) stage_id;
        ostr << "_extracted_exact";
        return ostr.str();
    }

    void set_value(int, unsigned int);
    void set_value(int, uint16_t);
    void set_value(int, uint8_t);

    /**
     *
     * @param priority
     */
    void set_priority(int priority) {
        this->priority = priority;
    }

    MatchKey get_match_key() override ;
};


class hyper4_intrinsic_match: public hyper4_match {
private:
    ActionData * set_std_action;
    MatchKey * match_key;
    string match_table_name;
public:
    /**
     *
     * @param p4info
     * @param inst_id
     * @param stage_id
     */
    hyper4_intrinsic_match(pi_p4info_t * p4info,
                           uint8_t program_id,
                           uint8_t stage_id):
            hyper4_match(p4info,
                         program_id,
                         stage_id) {

        table_id = pi_p4info_table_id_from_name(p4info,
                                                get_table_name().c_str());
        this->set_std_action = nullptr;
        this->match_key = nullptr;
    }

    string get_table_name() override {
        std::ostringstream ostr;
        ostr << "t";
        ostr << (uint) stage_id;
        ostr << "_stdmeta_exact";
        return ostr.str();
    }

    string get_ingress_port_table_name() {
        std::ostringstream ostr;
        ostr << "t";
        ostr << (uint) stage_id;
        ostr << "_stdmeta_ingress_port";
        return ostr.str();
    }

    string get_egress_spec_table_name() {
        std::ostringstream ostr;
        ostr << "t";
        ostr << (uint) stage_id;
        ostr << "_stdmeta_egress_spec";
        return ostr.str();
    }

    void install(hyper4_controller * controller,
                 pi_dev_id_t dev_id) override;

    void match_ingress_port(uint16_t port);

    void match_egress_spec(uint16_t port);

    MatchKey get_match_key() override ;

};


#endif //CONTROLLER_HYPER4_MATCH_H
