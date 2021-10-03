# Test: 'test' 
## Report of step: 'step1' 
1. Thrust set to: 18
1. Thrust set to: 10
1. Pds status: inserted
1. Check: "($ICD_NC15_CSG_HAT_MESS_1::pp_accept == 1)" **PASSED**
1. Thrust set to: 999
1. Thrust set to: 1000
1. Pds status: inserted
## Report of step: 'step2' 
1. Check: "(($ICD_NC15_CSG_HAT_MESS_1::pp_accept == 1) || ($AER_HW::ground_speed > 180)) && ($ICD_AC15_BRD_HAT_MESS_1::config_checks > 0)" **PASSED**
## Report of step: 'step3' 
1. Thrust set to: 1000
1. Thrust set to: 18
1. Thrust set to: 5
1. Pds status: inserted
1. Check: "($ICD_NC15_CSG_HAT_MESS_1::pp_accept == 1)" **PASSED**
1. Check: "(($RIGHT_COCKPIT_CONTROL::pds_inserted == 1) && ($RIGHT_COCKPIT_CONTROL::mdlr_open == 0)) || ($AER_HW::ground_speed > 180)" **FAILED**
1. Thrust set to: 18
1. Thrust set to: 88
1. Pds status: inserted
1. Check: "($ICD_NC15_CSG_HAT_MESS_1::pp_accept == 1)" **PASSED**
1. Thrust set to: 1
1. Thrust set to: 1
1. Pds status: inserted
