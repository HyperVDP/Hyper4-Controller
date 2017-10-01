SOURCE_FILES= hyper4_controller.cpp \
              hyper4_setup.cpp \
              hyper4_action.cpp \
              hyper4.cpp \
              hyper4_match.cpp \
              hyper4_utils.cpp \
              main.cpp \
              apps/switch/hyper4_switch.cpp \
              apps/hyper4_app.cpp \
              apps/router/hyper4_router.cpp \
              apps/qos/hyper4_qos.cpp \
              apps/multicast/hyper4_multicast.cpp \
              apps/learning/hyper4_learning.cpp \
              apps/ipsg/hyper4_ipsg.cpp \
              apps/hitter/hyper4_hitter.cpp \
              apps/firewall/hyper4_firewall.cpp



LIBS=    -lpi \
         -lpip4info \
         -lpifecpp \
         -lpi_bmv2 -lthrift -lruntimestubs -lsimpleswitch_thrift \
         -lboost_system

compile:
	@g++ -o controller $(SOURCE_FILES) $(LIBS) -std=c++11 -static-libstdc++