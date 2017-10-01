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

#include <iostream>
#include "hyper4_controller.h"
#include "apps/switch/hyper4_switch.h"
#include "apps/learning/hyper4_learning.h"
#include "apps/qos/hyper4_qos.h"
#include "apps/router/hyper4_router.h"
#include "apps/firewall/hyper4_firewall.h"
#include "apps/ipsg/hyper4_ipsg.h"
#include "apps/hitter/hyper4_hitter.h"

namespace hyper4 {
    void policy1(hyper4_controller * controller) {
        controller->assign_device((char *)"9090");
        controller->set_default_entries();
        controller->set_first_table(0,
                                    1,
                                    1,
                                    2,
                                    1,
                                    1);
        controller->set_first_table(0,
                                   2,
                                    2,
                                   1,
                                   1,
                                   1);

        hyper4_switch sw(0,
                         1,
                         1,
                         controller);
        sw.activate();
        sw.install(0x802200020002, 2);
        sw.install(0x802200010001, 1);
        sw.deactivate();
    }

    void policy2(hyper4_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();
        controller->set_first_table(0,
                                    1,
                                    1,
                                    2,
                                    1,
                                    1);
        controller->set_first_table(0,
                                    2,
                                    2,
                                    1,
                                    1,
                                    1);

        hyper4_learning learning(0,
                                 1,
                                 stage_id,
                                 controller);

        stage_id += learning.get_stage_num();
        std::cout<<(int) stage_id<<std::endl;

        hyper4_switch s(0,
                        1,
                        stage_id,
                        controller);

        learning>>s;

        // Learning
        {
            learning.activate();
            learning.install(0x802200010001);
            learning.install(0x802200020002);
        }
        {
            s.activate();
            s.install(0x802200010001, 1);
            s.install(0x802200020002, 2);
        }

    }

    void policy3(hyper4_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();
        controller->set_first_table(0,
                                    1,
                                    1,
                                    2,
                                    1,
                                    1);
        controller->set_first_table(0,
                                    2,
                                    2,
                                    1,
                                    1,
                                    1);

        hyper4_learning learning(0,
                                 1,
                                 stage_id,
                                 controller);

        stage_id += learning.get_stage_num();

        hyper4_switch l2sw(0,
                           1,
                           stage_id,
                           controller);


        stage_id += l2sw.get_stage_num();

        hyper4_qos qos(0,
                       1,
                       stage_id,
                       controller);

        learning>>l2sw>>qos;

        // Learning
        {
            learning.activate();
            learning.install(0x802200010001);
            learning.install(0x802200020002);
        }
        {
            l2sw.activate();
            l2sw.install(0x802200010001, 1);
            l2sw.install(0x802200020002, 2);
        }
        {
            qos.activate();
            qos.install(0x0a000001, 0x0a000002, 1);
            qos.install(0x0a000002, 0x0a000001, 1);
        }
    }

    void policy4(hyper4_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();
        controller->set_first_table(0,
                                    1,
                                    1,
                                    2,
                                    1,
                                    1);
        controller->set_first_table(0,
                                    2,
                                    2,
                                    1,
                                    1,
                                    1);

        hyper4_router router(0,
                             1,
                             stage_id,
                             controller);

        {
            router.activate();
            router.install(0x0a000001, 0x0001, 0x802200010002, 1);
            router.install(0x0a000002, 0x0002, 0x802200010001, 2);
        }
    }

    void policy5(hyper4_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();
        controller->set_first_table(0,
                                    1,
                                    1,
                                    2,
                                    1,
                                    1);
        controller->set_first_table(0,
                                    2,
                                    2,
                                    1,
                                    1,
                                    1);

        hyper4_router router(0,
                             1,
                             stage_id,
                             controller);

        stage_id += router.get_stage_num();

        hyper4_firewall firewall(0,
                                 1,
                                 stage_id,
                                 controller);

        router >> firewall;

        {
            router.activate();
            router.install(0x0a000001,
                           0x802200010001,
                           0x802200020002,
                           1);
            router.install(0x0a000002,
                           0x802200020002,
                           0x802200010001,
                           2);
        }


        {
            firewall.activate();
            firewall.install_white_list(0x0a000001,
                                        0x0a000002,
                                        10);
            firewall.install_white_list(0x0a000002,
                                        0x0a000001,
                                        10);
        }
    }

    void policy6(hyper4_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();
        controller->set_first_table(0,
                                    1,
                                    1,
                                    2,
                                    1,
                                    1);
        controller->set_first_table(0,
                                    2,
                                    2,
                                    1,
                                    1,
                                    1);

        hyper4_router router(0,
                             1,
                             stage_id,
                             controller);

        stage_id += router.get_stage_num();

        hyper4_ipsg ipsg(0,
                         1,
                         stage_id,
                         controller);

        stage_id += ipsg.get_stage_num();

        hyper4_firewall firewall(0,
                                 1,
                                 stage_id,
                                 controller);

        router >> ipsg >> firewall;

        {
            router.activate();
            router.install(0x0a000001,
                           0x802200010001,
                           0x802200020002,
                           1);
            router.install(0x0a000002,
                           0x802200020002,
                           0x802200010001,
                           2);
        }

        {
            ipsg.activate();
            ipsg.install(0x0a000001, 1);
            ipsg.install(0x0a000002, 2);
        }

        {
            firewall.activate();
            firewall.install_white_list(0x0a000001,
                                        0x0a000002,
                                        10);
            firewall.install_white_list(0x0a000002,
                                        0x0a000001,
                                        10);
        }
    }

    void policy7(hyper4_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();
        controller->set_first_table(0,
                                    1,
                                    1,
                                    2,
                                    1,
                                    1);
        controller->set_first_table(0,
                                    2,
                                    2,
                                    1,
                                    1,
                                    1);

        hyper4_learning learning(0,
                                 1,
                                 stage_id,
                                 controller);

        stage_id += learning.get_stage_num();

        hyper4_router router(0,
                             1,
                             stage_id,
                             controller);

        stage_id += router.get_stage_num();

        hyper4_ipsg ipsg(0,
                         1,
                         stage_id,
                         controller);

        stage_id += ipsg.get_stage_num();

        hyper4_firewall firewall(0,
                                 1,
                                 stage_id,
                                 controller);

        stage_id += firewall.get_stage_num();

        hyper4_qos qos(0,
                       1,
                       stage_id,
                       controller);

        learning>>router>>ipsg>>firewall>>qos;

        {
            learning.activate();
            learning.install(0x802200010001);
            learning.install(0x802200020002);
        }
        {
            router.activate();
            router.install(0x0a000001,
                           0x802200010001,
                           0x802200020002,
                           1);
            router.install(0x0a000002,
                           0x802200020002,
                           0x802200010001,
                           2);
        }
        {
            ipsg.activate();
            ipsg.install(0x0a000001, 1);
            ipsg.install(0x0a000002, 2);
        }
        {
            firewall.activate();
            firewall.install_white_list(0x0a000001,
                                        0x0a000002,
                                        10);
            firewall.install_white_list(0x0a000002,
                                        0x0a000001,
                                        10);
        }
        {
            qos.activate();
            qos.install(0x0a000001, 0x0a000002, 1);
            qos.install(0x0a000002, 0x0a000001, 1);
        }
    }

    uint64_t get_eth_dst(uint8_t dst) {
        auto x  = 0x802200000000ull;
        x |= dst;
        x |= (dst<<16);
        return x;
    }

    uint64_t get_eth_src(uint8_t src) {
        auto x  = 0x802200000000ull;
        x |= src;
        x |= (src<<16);
        return x;
    }

    uint32_t get_ip(uint8_t src) {
        return uint32_t (src|0x0a000000);
    }


    void intra_pod_edge(uint8_t src,
                        uint8_t dst,
                        hyper4_controller * controller) {

        auto program_id = controller->get_new_program_id();

        { // SRC_EDGE
            auto dev_id = (pi_dev_id_t)(src - 1);
            auto stage_id = (uint8_t) CONST_STAGE_1;

            controller->set_first_table(dev_id,
                                        3,
                                        src,
                                        dst,
                                        program_id,
                                        stage_id);


            hyper4_learning learning(dev_id,
                                     program_id,
                                     stage_id,
                                     controller);

            stage_id += learning.get_stage_num();

            hyper4_switch l2sw(dev_id,
                               program_id,
                               stage_id,
                               controller);

            stage_id += l2sw.get_stage_num();

            hyper4_ipsg ipsg(dev_id,
                             program_id,
                             stage_id,
                             controller);

            stage_id += ipsg.get_stage_num();

            hyper4_qos qos(dev_id,
                           program_id,
                           stage_id,
                           controller);

            learning>>l2sw>>ipsg>>qos;

            // Learning
            {
                learning.activate();
                learning.install(get_eth_src(src));
            }
            {
                l2sw.activate();
                l2sw.install(get_eth_dst(dst),
                             1);
            }
            {
                ipsg.activate();
                ipsg.install(get_ip(src),
                             3);
            }
            {
                qos.activate();
                qos.install(get_ip(src),
                            get_ip(dst),
                            1);
            }
        }
        { // DST_EDGE
            auto dev_id = (pi_dev_id_t)(dst - 1);
            auto stage_id = (uint8_t) CONST_STAGE_1;

            controller->set_first_table(dev_id,
                                        2,
                                        src,
                                        dst,
                                        program_id,
                                        stage_id);


            controller->set_first_table(dev_id,
                                        2,
                                        dst,
                                        src,
                                        program_id,
                                        stage_id);

            hyper4_learning learning(dev_id,
                                     program_id,
                                     stage_id,
                                     controller);

            stage_id += learning.get_stage_num();

            hyper4_switch l2sw(dev_id,
                               program_id,
                               stage_id,
                               controller);

            stage_id += l2sw.get_stage_num();

            hyper4_qos qos(dev_id,
                           program_id,
                           stage_id,
                           controller);

            learning>>l2sw>>qos;

            // Learning
            {
                learning.activate();
                learning.install(get_eth_src(src));
            }
            {
                l2sw.activate();
                l2sw.install(get_eth_dst(dst), 3);
            }
            {
                qos.activate();
                qos.install(get_ip(src),
                            get_ip(dst), 1);
            }
        }
    }

    void intra_pod_aggregate(uint8_t src,
                             uint8_t dst,
                             hyper4_controller * controller) {

        auto dev_id = controller->get_dev_id(src + 3);
        auto program_id = controller->get_new_program_id();
        auto stage_id = (uint8_t) CONST_STAGE_1;


        controller->set_first_table(dev_id,
                                    1,
                                    src,
                                    dst,
                                    program_id,
                                    stage_id);

        hyper4_learning learning(dev_id,
                                 program_id,
                                 stage_id,
                                 controller);

        stage_id += learning.get_stage_num();

        hyper4_switch l2sw(dev_id,
                           program_id,
                           stage_id,
                           controller);

        stage_id += l2sw.get_stage_num();

        hyper4_qos qos(dev_id,
                       program_id,
                       stage_id,
                       controller);

        learning>>l2sw>>qos;

        // Learning
        {
            learning.activate();
            learning.install(get_eth_src(src));
        }
        {
            l2sw.activate();
            l2sw.install(get_eth_dst(dst),
                         2);
        }
        {
            qos.activate();
            qos.install(get_ip(src),
                        get_ip(dst),
                        1);
        }
    }

    void intra_pod(uint8_t src, uint8_t dst, hyper4_controller * controller) {
        intra_pod_edge(src, dst, controller);
        intra_pod_aggregate(src, dst, controller);
    }

    void inter_pod_edge(uint8_t src,
                        uint8_t dst,
                        hyper4_controller * controller) {
        auto program_id = controller->get_new_program_id();
        { // SRC-EDGE
            auto dev_id = (pi_dev_id_t)(src - 1);
            auto stage_id = (uint8_t) CONST_STAGE_1;

            controller->set_first_table(dev_id,
                                        3,
                                        src,
                                        dst,
                                        program_id,
                                        stage_id);

            hyper4_switch l2sw(dev_id,
                               program_id,
                               stage_id,
                               controller);

            stage_id += l2sw.get_stage_num();

            hyper4_ipsg ipsg(dev_id,
                             program_id,
                             stage_id,
                             controller);


            l2sw>>ipsg;

            {
                l2sw.activate();
                l2sw.install(get_eth_dst(dst),
                             1);
            }
            {
                ipsg.activate();
                ipsg.install(get_ip(src),
                             3);
            }

        }

        { //DST-EDGE
            auto dev_id = (pi_dev_id_t)(dst - 1);
            auto stage_id = (uint8_t) CONST_STAGE_1;

            controller->set_first_table(dev_id,
                                        1,
                                        src,
                                        dst,
                                        program_id,
                                        stage_id);
            controller->set_first_table(dev_id,
                                        2,
                                        src,
                                        dst,
                                        program_id,
                                        stage_id);

            hyper4_switch l2sw(dev_id,
                               program_id,
                               stage_id,
                               controller);
            // Learning
            {
                l2sw.activate();
                l2sw.install(get_eth_dst(dst),
                             3);
            }
        }

    }
    void inter_pod_aggregate(uint8_t src,
                             uint8_t dst,
                             hyper4_controller * controller) {
        auto program_id = controller->get_new_program_id();
        { // SRC-AGG


            auto dev_id = (pi_dev_id_t)(src + 3);
            auto stage_id = (uint8_t) CONST_STAGE_1;

            controller->set_first_table(dev_id,
                                        1,
                                        src,
                                        dst,
                                        program_id,
                                        stage_id);

            hyper4_switch l2sw(dev_id,
                               program_id,
                               stage_id,
                               controller);
            // Learning
            {
                l2sw.activate();
                l2sw.install(get_eth_dst(dst),
                             3);
            }
        }
        { // DST_AGG
            {
                auto dev_id = (pi_dev_id_t) (dst + 3);
                auto stage_id = (uint8_t) CONST_STAGE_1;

                controller->set_first_table(dev_id,
                                            3,
                                            src,
                                            dst,
                                            program_id,
                                            stage_id);

                hyper4_switch l2sw(dev_id,
                                   program_id,
                                   stage_id,
                                   controller);
                // Learning
                {
                    l2sw.activate();
                    l2sw.install(get_eth_dst(dst), 1);
                }
            }

            {
                if (dst % 2 == 1) {
                    auto dev_id = (pi_dev_id_t) (dst + 4);
                    auto stage_id = (uint8_t) CONST_STAGE_1;

                    controller->set_first_table(dev_id,
                                                3,
                                                src,
                                                dst,
                                                program_id,
                                                stage_id);

                    hyper4_switch l2sw(dev_id,
                                       program_id,
                                       stage_id,
                                       controller);
                    // Learning
                    {
                        l2sw.activate();
                        l2sw.install(get_eth_dst(dst), 2);
                    }
                } else {
                    auto dev_id = (pi_dev_id_t) (dst + 2);
                    auto stage_id = (uint8_t) CONST_STAGE_1;


                    controller->set_first_table(dev_id,
                                                3,
                                                src,
                                                dst,
                                                program_id,
                                                stage_id);

                    hyper4_switch l2sw(dev_id,
                                       program_id,
                                       stage_id,
                                       controller);
                    // Learning
                    {
                        l2sw.activate();
                        l2sw.install(get_eth_dst(dst), 2);
                    }
                }

            }


        }
    }
    void inter_pod_core(uint8_t src,
                        uint8_t dst,
                        hyper4_controller * controller) {
        auto dev_id = pi_dev_id_t(8);
        auto port = uint16_t(dst > src ? 2 : 1);
        for (; dev_id < 10; dev_id ++) {

            auto program_id = (uint8_t) (dst + 16);

            uint8_t stage_id = CONST_STAGE_1;

            controller->set_first_table(dev_id,
                                        2,
                                        src,
                                        dst,
                                        program_id,
                                        stage_id);
            controller->set_first_table(dev_id,
                                        1,
                                        src,
                                        dst,
                                        program_id,
                                        stage_id);

            hyper4_router router(dev_id,
                                 program_id,
                                 stage_id,
                                 controller);

            stage_id += router.get_stage_num();

            hyper4_firewall firewall(dev_id,
                                     program_id,
                                     stage_id,
                                     controller);

            stage_id += firewall.get_stage_num();


            hyper4_hitter hitter(dev_id,
                                 program_id,
                                 stage_id,
                                 controller);
            stage_id += hitter.get_stage_num();

            hyper4_qos qos(dev_id,
                           program_id,
                           stage_id,
                           controller);

            router >> firewall >> hitter >> qos;

            {
                router.activate();
                router.install(get_ip(dst),
                               get_eth_dst(dst),
                               get_eth_src(src),
                               port);
            }
            {
                firewall.activate();
                firewall.install_white_list(get_ip(src),
                                            get_ip(dst),
                                            10);
            }
            {
                hitter.activate();
                hitter.install(get_ip(src),
                               get_ip(dst));
            }

            {
                qos.activate();
                qos.install(get_ip(src),
                            get_ip(dst), 1);
            }
        }
    }

    void inter_pod(uint8_t src,
                   uint8_t dst,
                   hyper4_controller * controller) {
        inter_pod_edge(src, dst, controller);
        inter_pod_aggregate(src, dst, controller);
        inter_pod_core(src, dst, controller);
    }


    void fat_tree(hyper4_controller * controller) {
        controller->assign_device((char *) "9090");
        controller->assign_device((char *) "9091");
        controller->assign_device((char *) "9092");
        controller->assign_device((char *) "9093");
        controller->assign_device((char *) "9094");
        controller->assign_device((char *) "9095");
        controller->assign_device((char *) "9096");
        controller->assign_device((char *) "9097");
        controller->assign_device((char *) "9098");
        controller->assign_device((char *) "9099");
        controller->set_default_entries();
        {
            intra_pod(1, 2, controller);
            intra_pod(2, 1, controller);
            intra_pod(3, 4, controller);
            intra_pod(4, 3, controller);
        }
	
        {
            inter_pod(1, 3, controller);
            inter_pod(1, 4, controller);
            inter_pod(2, 3, controller);
            inter_pod(2, 4, controller);
            inter_pod(3, 1, controller);
            inter_pod(3, 2, controller);
            inter_pod(4, 1, controller);
            inter_pod(4, 2, controller);
        }
    }
}


int main() {
    pi_init(256, nullptr);  // 256 max devices
    pi_p4info_t *p4info;
    pi_add_config_from_file("../build/hyper4.json",
                            PI_CONFIG_TYPE_BMV2_JSON,
                            &p4info);

    hyper4_controller controller(p4info);

    hyper4::fat_tree(&controller);

}
