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

#include "hyper4_router.h"
#include "../../hyper4_match.h"
#include "../../hyper4_action.h"

void
hyper4_router::activate() {
    pi_p4info_t * p4info = controller->get_p4info();
    uint8_t  stage_id = start_stage;
    {
        hyper4_header_match match(p4info,
                                  program_id,
                                  stage_id);
        match.set_eth_type(0x0800);
        match.set_ip_proto(0x11);

        uint8_t action_id = controller->allocate_new_action_id(program_id);
        uint32_t match_id = controller->allocate_new_match_id(program_id);

        match.init_program_state(action_id,
                                 match_id,
                                 NEXT_STAGE(stage_id));
        match.install(controller,
                      dev_id);

        hyper4_action action(program_id,
                             start_stage,
                             action_id,
                             match_id,
                             NEXT_STAGE(stage_id),
                             controller);


        action.add_no_op();

        action.install(this->dev_id);
    }

    {
        hyper4_header_match match(p4info,
                                  program_id,
                                  stage_id);
        match.set_eth_type(0x0800);
        match.set_ip_proto(0x06);

        uint8_t action_id = controller->allocate_new_action_id(program_id);
        uint32_t match_id = controller->allocate_new_match_id(program_id);

        match.init_program_state(action_id,
                                 match_id,
                                 EXTRACTED_EXACT);
        match.install(controller,
                      dev_id);

        hyper4_action action(program_id,
                             start_stage,
                             action_id,
                             match_id,
                             NEXT_STAGE(stage_id),
                             controller);


        action.add_no_op();

        action.install(this->dev_id);
    }

    {
        hyper4_header_match match(p4info,
                                  program_id,
                                  stage_id);
        match.set_eth_type(0x0800);
        match.set_ip_proto(0x01);

        uint8_t action_id = controller->allocate_new_action_id(program_id);
        uint32_t match_id = controller->allocate_new_match_id(program_id);

        match.init_program_state(action_id,
                                 match_id,
                                 EXTRACTED_EXACT);
        match.install(controller,
                      dev_id);

        hyper4_action action(program_id,
                             start_stage,
                             action_id,
                             match_id,
                             NEXT_STAGE(stage_id),
                             controller);


        action.add_no_op();

        action.install(this->dev_id);
    }


}

void
hyper4_router::install(uint32_t dst_ip,
                       uint64_t dst_mac,
                       uint64_t src_mac,
                       uint16_t port) {

    pi_p4info_t * p4info = controller->get_p4info();

    auto  stage_id = NEXT_STAGE(start_stage);

    {
        hyper4_header_match match(p4info,
                                  program_id,
                                  stage_id);
        match.set_ip_dst(dst_ip);

        uint8_t action_id = controller->allocate_new_action_id(program_id);
        uint32_t match_id = controller->allocate_new_match_id(program_id);

        match.init_program_state(action_id,
                                 match_id,
                                 STDMETA_EXACT);

        match.install(controller,
                      dev_id);

        hyper4_action action(program_id,
                             stage_id,
                             action_id,
                             match_id,
                             NEXT_STAGE(stage_id),
                             controller);
        action.add_sub_ttl(1);

        action.add_forward(port);

        action.add_mod_eth_dst(dst_mac);

        action.install(dev_id);

    }

    stage_id = NEXT_STAGE(stage_id);
    {
        hyper4_intrinsic_match match(p4info,
                                     program_id,
                                     stage_id);
        match.match_egress_spec(port);

        uint8_t action_id = controller->allocate_new_action_id(program_id);
        uint32_t match_id = controller->allocate_new_match_id(program_id);

        match.init_program_state(action_id,
                                 match_id,
                                 next_stage_type);

        match.install(controller,
                      dev_id);


        hyper4_action action(program_id,
                             stage_id,
                             action_id,
                             match_id,
                             next_stage_id,
                             controller);

        action.add_mod_eth_src(src_mac);

        action.install(dev_id);
    }
}


void
hyper4_router::deactivate() {

}
