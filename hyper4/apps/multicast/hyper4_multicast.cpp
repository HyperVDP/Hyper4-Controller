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

#include "hyper4_multicast.h"
#include "../../hyper4_match.h"
#include  "../../hyper4_action.h"
void
hyper4_multicast::activate() {

}


void
hyper4_multicast::install(uint16_t dst_mac,
                          uint8_t seq_id,
                          uint16_t port) {

    hyper4_header_match match(controller->get_p4info(),
                              this->program_id,
                              this->start_stage);

    match.set_eth_dst(dst_mac);

    uint8_t action_id = controller->allocate_new_action_id(program_id);
    uint32_t match_id = controller->allocate_new_match_id(program_id);

    match.init_program_state(action_id,
                             match_id,
                             next_stage_type);

    match.install(controller,
                  dev_id);

    hyper4_action action(program_id,
                         start_stage,
                         action_id,
                         match_id,
                         next_stage_id,
                         controller);
    action.add_multicast(seq_id,
                         port);

    action.install(this->dev_id);
}

void
hyper4_multicast::deactivate() {

}