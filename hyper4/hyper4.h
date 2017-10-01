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


#ifndef CONTROLLER_HYPER4_H
#define CONTROLLER_HYPER4_H

// extracted.data field width
#define EXTRACTED_SIZE	800

// value for sm.egress_spec indicating virt net
#define VIRT_NET 65

// parse_ctrl.next_action
#define PROCEED	     (uint8_t) 0
#define INSPECT_SEB		1
#define INSPECT_20_29 2
#define INSPECT_30_39 3
#define INSPECT_40_49 4
#define INSPECT_50_59 5
#define INSPECT_60_69 6
#define INSPECT_70_79 7
#define INSPECT_80_89 8
#define INSPECT_90_99 9
#define EXTRACT_MORE	10

// meta_ctrl.stage
#define INIT	0
#define NORM	1

// meta_ctrl.next_stage_id
#define DONE            (uint8_t)0
#define EXTRACTED_EXACT	(uint8_t)1
#define METADATA_EXACT	(uint8_t)2
#define STDMETA_EXACT	(uint8_t)3
#define EXTRACTED_VALID (uint8_t)4

// meta_ctrl.stage_state
#define COMPLETE    (uint8_t)1
#define CONTINUE	(uint8_t)2

// meta_ctrl.stdmeta_ID
#define STDMETA_INGRESS_PORT	(uint8_t) 1
#define STDMETA_PACKET_LENGTH	(uint8_t) 2
#define STDMETA_INSTANCE_TYPE	(uint8_t) 3
#define STDMETA_PARSERSTAT	    (uint8_t) 4
#define STDMETA_PARSERERROR	    (uint8_t) 5
#define STDMETA_EGRESS_SPEC	    (uint8_t) 6

// meta_primitive_state.primitive
#define A_MODIFY_FIELD	      	0
#define A_ADD_HEADER		        1
#define A_COPY_HEADER		        2
#define A_REMOVE_HEADER		      3
#define A_MODIFY_FIELD_WITH_HBO	4
#define A_TRUNCATE		          5
#define A_DROP			            6
#define A_NO_OP			            7
#define A_PUSH			            8
#define A_POP			              9
#define A_COUNT			            10
#define A_METER			            11
#define A_GENERATE_DIGEST	      12
#define A_RECIRCULATE		        13
#define A_RESUBMIT		          14
#define A_CLONE_INGRESS_INGRESS	15
#define A_CLONE_EGRESS_INGRESS	16
#define A_CLONE_INGRESS_EGRESS	17
#define A_CLONE_EGRESS_EGRESS	  18
#define A_MULTICAST		          19
#define A_MATH_ON_FIELD		      20

#define MOD_EGRESS_SPEC  0
#define MOD_HEADER       1
#define MOD_META         2

#define MATH_SUB_HEADER      0

#define STAGE_NUM 9
#define CONST_STAGE_1				1
#define	CONST_STAGE_2				2
#define	CONST_STAGE_3				3
#define	CONST_STAGE_4				4
#define CONST_STAGE_5				5
#define	CONST_STAGE_6				6
#define	CONST_STAGE_7				7
#define	CONST_STAGE_8				8
#define	CONST_STAGE_9				9

#define NEXT_STAGE(x) (uint8_t)((x) + 1)


#define ETH_DST_BYTE_OFFSET  0
#define ETH_SRC_BYTE_OFFSET  6
#define ETH_TYPE_BYTE_OFFSET 12
#define TTL_BYTE_OFFSET      22
#define IP_PROTO_BYTE_OFFSET 23
#define IP_SRC_BYTE_OFFSET   26
#define IP_DST_BYTE_OFFSET   30

#define END_INST 0xFF
#define END_STAGE 0

#endif //CONTROLLER_HYPER4_H
