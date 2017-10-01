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

#ifndef CONTROLLER_HYPER4_PRIMITIVE_H
#define CONTROLLER_HYPER4_PRIMITIVE_H


#include <cstdint>
#include <vector>
#include <sstream>

#include "hyper4_controller.h"
#include "hyper4.h"

using namespace std;

class hyper4_primitive {
protected:
    uint8_t program_id;
    uint8_t subtype;
    uint8_t stage_id;
    uint8_t action_id;
    uint8_t primitive_index;
    uint8_t next_primitive_index;
    uint32_t match_id;
    uint8_t primitive;
    uint8_t next_stage_id;
    ActionData * action;
    hyper4_controller * controller;
public:
    hyper4_primitive(uint8_t program_id,
                     uint8_t stage_id,
                     uint8_t action_id,
                     uint32_t match_id,
                     uint8_t next_stage_id,
                     hyper4_controller * controller) {
        this->program_id = program_id;
        this->action_id = action_id;
        this->primitive_index = primitive_index;
        this->stage_id = stage_id;
        this->controller = controller;
        this->match_id = match_id;
        this->next_stage_id = next_stage_id;
        this->action = nullptr;
        this->next_primitive_index = 0;
        this->subtype = 0;
        this->primitive = 0;
    }

    virtual void install_primitive(pi_dev_id_t dev_id) = 0;
    void set_primitive_metadata(pi_dev_id_t dev_id);
    void set_program_state(pi_dev_id_t dev_id);

    void set_next_index(uint8_t index);

    void set_index(uint8_t index);

    virtual string get_primitive_table_name() = 0;
    string get_primitive_metadata_table_name() {
        std::ostringstream ostr;
        ostr << "set_primitive_metadata_";
        ostr << (int) stage_id;
        ostr << (int) primitive_index;
        return ostr.str();
    }

    string get_program_state_table_name() {
        std::ostringstream ostr;
        ostr << "set_program_state_";
        ostr << (int) stage_id;
        ostr << (int) primitive_index;
        return ostr.str();
    }

    ~hyper4_primitive() {
        delete action;
    }
};


class do_multicast : public hyper4_primitive {
public:
    do_multicast(uint8_t program_id,
                 uint8_t stage_id,
                 uint8_t action_id,
                 uint32_t match_id,
                 uint8_t subtype,
                 uint8_t next_stage_id,
                 hyper4_controller * controller) :
            hyper4_primitive(program_id,
                             stage_id,
                             action_id,
                             match_id,
                             next_stage_id,
                             controller) {
        this->subtype = subtype;
        this->primitive = A_MULTICAST;
    }

    void install_primitive(pi_dev_id_t dev_id) override;
    string get_primitive_table_name() override;

    void set_multicast(uint8_t seq_id, uint16_t port);
};

class do_modify_field : public hyper4_primitive {
public:
    do_modify_field(uint8_t program_id,
                    uint8_t stage_id,
                    uint8_t action_id,
                    uint32_t match_id,
                    uint8_t subtype,
                    uint8_t next_stage_id,
                    hyper4_controller * controller) :
            hyper4_primitive(program_id,
                             stage_id,
                             action_id,
                             match_id,
                             next_stage_id,
                             controller) {
        this->subtype = subtype;
        this->primitive = A_MODIFY_FIELD;
    }

    void install_primitive(pi_dev_id_t dev_id) override;

    string get_primitive_table_name() override;

    void set_egress_spec(uint16_t port);

    void set_eth_src(uint64_t type);

    void set_eth_dst(uint64_t type);

    void set_meta_const(char (&val)[32], char (&left_shift)[32], char (&tmeta_mask)[32]);

    void set_meta_const(uint32_t val, uint32_t left_shift, uint32_t tmeta_mask);

    void set_meta_const(uint16_t val, uint16_t left_shift, uint16_t tmeta_mask);

    void set_meta_const(uint8_t val, uint8_t left_shift, uint8_t tmeta_mask);

};

class do_math_on_field : public hyper4_primitive {
public:
    do_math_on_field(uint8_t program_id,
                    uint8_t stage_id,
                    uint8_t action_id,
                    uint32_t match_id,
                    uint8_t subtype,
                    uint8_t next_stage_id,
                    hyper4_controller * controller) :
            hyper4_primitive(program_id,
                             stage_id,
                             action_id,
                             match_id,
                             next_stage_id,
                             controller) {
        this->subtype = subtype;
        this->primitive = A_MATH_ON_FIELD;
    }

    void install_primitive(pi_dev_id_t dev_id) override;
    string get_primitive_table_name() override;

    void set_subff_extracted_const_u(char (&val)[100], char (&left_shift)[100]);
    void set_subff_extracted_const_u(uint8_t val, uint8_t  right_byte_shift);
};

class do_drop : public hyper4_primitive {
public:
    do_drop(uint8_t program_id,
                    uint8_t stage_id,
                    uint8_t action_id,
                    uint32_t match_id,
                    uint8_t subtype,
                    uint8_t next_stage_id,
                    hyper4_controller * controller) :
            hyper4_primitive(program_id,
                             stage_id,
                             action_id,
                             match_id,
                             next_stage_id,
                             controller) {
        this->subtype = subtype;
        this->primitive = A_DROP;
    }

    void install_primitive(pi_dev_id_t dev_id) override;
    string get_primitive_table_name() override;

    void set_drop();
};

class hyper4_action {
private:
    uint8_t primitive_index;
    uint8_t program_id;
    uint8_t action_id;
    uint32_t match_id;
    uint8_t stage_id;
    uint8_t next_stage_id;
    hyper4_controller * controller;
    vector<hyper4_primitive*> primitives;

public:

    hyper4_action(uint8_t program_id,
                  uint8_t stage_id,
                  uint8_t actiond_id,
                  uint32_t match_id,
                  uint8_t next_stage_id,
                  hyper4_controller * controller) {
        this->action_id = actiond_id;
        this->stage_id = stage_id;
        this->program_id = program_id;
        this->match_id = match_id;
        this->primitive_index = 0;
        this->controller = controller;
        this->next_stage_id = next_stage_id;
    }

    void add_primitive(hyper4_primitive * primitive);

    void add_forward(uint16_t port);

    void add_mod_eth_dst(uint64_t dst);

    void add_mod_eth_src(uint64_t src);

    void add_multicast(uint8_t seq_id, uint16_t port);

    void add_mod_meta_const(uint8_t val, uint8_t left_shift, uint8_t tmeta_mask);

    void add_sub_const(uint8_t val, uint8_t left_shift);

    void add_sub_ttl(uint8_t val);

    void add_no_op();

    void add_drop();

    void install(pi_dev_id_t dev_id);

};

class no_op : public hyper4_primitive {
public:
    no_op(uint8_t program_id,
                    uint8_t stage_id,
                    uint8_t action_id,
                    uint32_t match_id,
                    uint8_t subtype,
                    uint8_t next_stage_id,
                    hyper4_controller * controller) :
            hyper4_primitive(program_id,
                             stage_id,
                             action_id,
                             match_id,
                             next_stage_id,
                             controller) {
        this->subtype = subtype;
        this->primitive = A_NO_OP;
    }

    void install_primitive(pi_dev_id_t dev_id) override;
    string get_primitive_table_name() override;
};

#endif