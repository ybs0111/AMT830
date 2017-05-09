#ifndef _VARIABLE_h__
#define _VARIABLE_h__

#include "Alg_Mysql.h"
#include "ComizoaPublic.h"	//20120514 유성준
#include "CtlBd_Variable.h"

#define MAX_DELAY_IO	20

#define BCR_STEPBYSTEP	0	
#define BCR_FLYING		1

#define BCR_STEP_PULS	4000
#define BCR_READON	8881	//barcode read start
#define BCR_READOFF	8882	//barcode read finish
#define BCR_ECHO	8883	//barcode parsing echo
#define BCR_NONE	"A"

#define BCR_MAX_CNT 2		//barcode read max count
#define BCR_SND_WAIT 10000
#define BCR_RCV_WAIT 50000

#define DEVELOPER_PC			0
#define BARCODE_USED			1

#define SIMULATION_MODE		1

#define MAXMOTOR		40

/////////////////////////////////// 20120620
#define LABEL_PRINT_MAX_CNT 3	
#define ML_MAX_CNT 11	
/////////////////////////////////// 
//////////////////////////////////// 20120704
#define TRAY_LD_COL			    30
#define TRAY_HS_LEFT_COL    	30
#define TRAY_HS_RIGHT_COL		30
#define TRAY_CLIP1_COL			100
#define TRAY_CLIP2_COL			100
#define TRAY_ULD_COL			30
#define TRAY_REJECT_COL			30

#define MOT_LOADER_TRAY		0
#define MOT_HS_LEFT_TRAY	1
#define MOT_HS_RIGHT_TRAY	2
#define MOT_CLIP1_TRAY	    3
#define MOT_CLIP2_TRAY	    4
#define MOT_UNLOADER_TRAY	5
#define MOT_REJECT_TRAY		6

/////////////////////////////////// 20120707
#define MOT_LOADER_PICKER	0
#define MOT_HS_LEFT_PICKER	1
#define MOT_HS_RIGHT_PICKER	2
#define MOT_CLIP1_PICKER	3
#define MOT_CLIP2_PICKER	4
#define MOT_UNLOADER_PICKER	5
#define MOT_SORTER_PICKER	6

///////////////////////////////////

////// Elevator 상태
#define TRAY_NONE		0		//트레이가 없는 카세트
#define TRAY_READY		1		//작업중인 카세트를 표시
#define TRAY_FULL		2		//언로딩 트레이 (작업이 끝남)
#define TRAY_EMPTY		3		//트레이에 자재 없음
#define TRAY_CHANGE     4
#define TRAY_INFO       5

////////////////////////////// 20120707
#define PICKER_EMPTY	    0
#define PICKER_CHECK1	    1
#define PICKER_CHECK2	    2
#define PICKER_FULL 	    3
////////////////////////////////////

#define DATA_INFO			5 //[0]:dvc 유/무, [1]:빈정보, [2]:retest cnt, [3]:screen 빈 정보, [4]:현재 디바이스 유/무 상태 임시 저장, 또는 보완정보 확인  
#define PICKER_PARA			2
#define LDULD_PARA			1

/////////////////////////////////////////////////////////
//DLL 구조로 변경예정 //2011.1007 추가 
/////////////////////////////////////////////////////////
#define BD_NO		0  //어떤 조건이 클리어된 상태 
#define BD_YES		1  //어떤 조건을 요청한 상태

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//motor, i/O 보드등의 리턴값은 이 것만 사용한다  
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BD_ERROR    -1
#define BD_PROCEED  0
#define BD_GOOD		1
#define BD_RETRY	2
#define BD_SAFETY	3

#define TRAY_X                  0
#define TRAY_Y                  1

#define M_ROBOT_XY_INDEX		0

#define POS_OUT					0
#define POS_IN					1

#define BINCHECK_SKIP           0    // BIN 데이터 검사하지 않음 

#define SOCKET_NUM	20
#define PICKER_NUM	2
#define RETEST_NUM	40
#define LOB_NUM		180
#define REJECT_NUM	30

#define INFO_DATA	4

#define T_PICK_UPDN				0
#define T_PICK_ONOFF			1
#define T_RJT_BUF_CLMAP			2
#define T_RJT_BUF_SHIFT			3
#define T_GRIPPER_LENGTH		4
#define T_LD_CONVEYOR_STOPPER	5
#define T_LD_CONVEYOR_TRAY_CHK	6
#define T_LD_TRAY_CLAMP			7
#define T_LD_TRAY_CHANGE		8
#define T_LD_TRAY_MOVE			9
#define T_LD_TRAY_PUSHER		10
#define T_LD_TRAY_SEPERATE		11
#define T_LD_TRAY_BASE_RAIL		12
#define T_LD_TRAY_HOOK			13
#define T_AIR_BLOW_SHUTTER		14
#define T_AIR_BLOW_MOTOR		15
#define T_ULD_TRAY_BASE_RAIL	16
#define T_ULD_TRAY_CLAMP		17
#define T_ULD_TRAY_STOPPER		18
#define T_ULD_CONVEYOR_STOPPER	19
#define T_ULD_CONVEYOR_CHANGE	20

#define CYLINDER_ON		1
#define CYLINDER_OFF	0
#define CYLINDER_ERROR	2

#define MAX_WAIT_TIME_NUM				95//40

#define INIT_TRAY_SUPPLY		0
#define INIT_TRAY_TRANSFER		1
#define INIT_ROBOT				2
#define INIT_AIRBLOW			3
#define INIT_TRAY_UNLOADER		4

///////////////////////////////////////
// 모터 이름 및 위치
///////////////////////////////////////
#define M_LDM_STACKER_1				0
#define M_LDM_STACKER_2				1
#define M_LDM_STACKER_MOVE			2
#define M_HS_F_STACKER_1			3
#define M_HS_F_STACKER_2			4
#define M_HS_F_STACKER_MOVE			5
#define M_HS_B_STACKER_1			6
#define M_HS_B_STACKER_2			7
#define M_HS_B_STACKER_MOVE			8
#define M_ULDM_STACKER_2			9
#define M_ULDM_STACKER_1			10
#define M_ULDM_STACKER_MOVE			11
#define M_M_RBT_Y					12
#define M_M_RBT_Z					13
#define M_HS_F_RBT_Y				14
#define M_HS_F_RBT_Z				15
#define M_HS_B_RBT_Y				16
#define M_HS_B_RBT_Z				17
#define M_CLIP_BUFFER				18
#define M_WORK_BUFFER_1				19
#define M_WORK_BUFFER_2				20
#define M_WORK_BUFFER_3				21
#define M_WORK_BUFFER_4				22
#define M_M_CLAMP_RBT_GRIPPER		23
#define M_M_CLAMP_RBT_Y				24
#define M_M_CLAMP_RBT_Z				25
#define M_CLIP_INSERT_Y				26
#define M_CLIP_INSERT_Z				27
#define M_RBT_UNLOAD_X				28
#define M_RBT_UNLOAD_Y				29
#define M_RBT_UNLOAD_Z				30
#define M_RBT_SORTER_Z				31
#define M_RBT_SORTER_X				32
#define M_RBT_SORTER_Y				33
#define M_M_CLIP_CLAMP				34
#define M_LABEL_ATTACH_Z			35
#define M_VISION1_Y					36
#define M_VISION1_Z					37
#define M_VISION2_Y					38
#define M_VISION2_Z					39


///////////////////////////////////////

#define BACK_CAL_POS		700		//각정정보를 통해 계산되어질 위치다.!
#define BACK_SET_POS		800		//해당 위치를 티칭한 데이터다..

#define IDC_CURSOR_HAND                 900

#define INDEXNUM_ROBOT		0


//LD stackerteach pos
#define PICKING_GAP					2
#define PICKERSTATUS_CHECK_TIME		500 // 20140226

#define TRAYROW_ON					1
#define TRAYROW_OFF					4		// 트레이의 row을 사용하지 않는다는 의미로 사용함

///// test bin  result
#define BIN_CLEAR					-1
#define BIN_NOTUESD					0
#define BIN_LDBUFFERBIN				1
#define BIN_LEFTSINKBIN				2
#define BIN_RIGHTSINKBIN			3
#define BIN_CLIPBIN					4
#define BIN_ULDBUFFERBIN			5
#define BIN_GOODBIN					6
#define BIN_REJECTBIN				7

#define WORK_BUFFER_1				0//st_sync.n_module_buffer_action_site[0]
#define WORK_BUFFER_2				1
#define WORK_BUFFER_3				2
#define WORK_BUFFER_4				3

#define LDMODULE_SITE				0
#define LEFTSINK_SITE				1
#define RIGHTSINK_SITE				2
#define LDCLIP_SITE					3
#define SORTER_SITE					4
#define WORK_BUFFER_SITE			5
#define UNLOADER_SITE				6
#define REJECT_SITE					7
#define VISION_SITE					8

#define LDRBTMODULE_SITE			0
#define LEFTRBTSINK_SITE			1
#define RIGHTRBTSINK_SITE			2
#define LDRBTCLIPBUFFER_SITE		3
#define LDRBTCLIPINSERT_SITE		4
#define LDRBTCLIPCLAMP_SITE			5
#define SORTERRBT_SITE				6
#define TOPVISION_SITE				7
#define BTMVISION_SITE				8
#define UNLOADRBT_SITE				9
#define REJECTRBT_SITE				10
#define WORK_BUFFER1_SITE			11
#define WORK_BUFFER2_SITE			12
#define WORK_BUFFER3_SITE			13
#define WORK_BUFFER4_SITE			14
#define MODULESTACKER_SITE			15
#define LEFTHSTACKER_SITE			16
#define RIGHTHSTACKER_SITE			17
#define UNLOADSTACKER_SITE			18


#define SORTER_MOT_MAP				0
#define UNLOAD_MOT_MAP				1

#define BUFF_EMPTY					0 //디바이스 없다 
#define BUFF_DVC_LOAD				1
#define BUFF_LHEAT_LOAD				2
#define BUFF_RLHEAT_LOAD			3
#define BUFF_CLIP_LOAD				4
#define BUFF_DVC_UNLOAD				5
#define BUFF_DVC_SKIP				6 //이런 경우 없다~~

//Work_buffer
#define BUFF_SAFETY					0
#define BUFF_LD_MODULE				1
#define BUFF_LD_LEFTHEAT			2
#define BUFF_LD_RIGHTHEAT			3
#define BUFF_LD_CLIP				4
#define BUFF_ULD_MODULE				5


//M Stacker
#define P_STOCKER_SAFETY			0
#define P_STOCKER_DOWN				1
#define P_STOCKER_UP				2
#define P_STOCKER_UP_OFFSET			3
#define P_STOCKER_DN_OFFSET			4
#define P_STOCKER_P_LIMIT			5

//WORK
#define WORK_LD_SAFETY				0
#define WORK_LD_MODULE				1
#define WORK_LD_LEFTHEAT			2
#define WORK_LD_RIGHTHEAT			3
#define WORK_LD_CLIP				4
#define WORK_ULD_MODULE				5


//MOdule RBT
//TRAY
#define Y_LD_SAFETY					0		// Y AXIS의 로더 트레이 처음 자재 위치
#define Y_LD_TRAY_START				1		// Y AXIS의 로더 트레이 마지막 자재 위치
#define Y_LD_TRAY_END				2		// Y AXIS의 왼쪽 버퍼의 처음 위치
#define Y_LD_WORK_PLACE1			3
#define Y_LD_WORK_PLACE2			4
#define Y_LD_WORK_PLACE3			5
#define Y_LD_WORK_PLACE4			6


#define Z_LD_SAFETY_UP				0		// Z AXIS의 ROBOT가 이동 가능할 높이
#define Z_LD_TRAY_PICK				1		// Z AXIS의 로더 트레이의 자재를 집는 높이
#define Z_LD_BUFFER_PLACE1			2		// Z AXIS의 로더 트레이에서 자재를 놓는 높이 (리젝 트레이에 놓는 경우가 있음)
#define Z_LD_BUFFER_PLACE2			3		// Z AXIS의 로더 트레이에서 자재를 놓는 높이 (리젝 트레이에 놓는 경우가 있음)
#define Z_LD_BUFFER_PLACE3			4		// Z AXIS의 로더 트레이에서 자재를 놓는 높이 (리젝 트레이에 놓는 경우가 있음)
#define Z_LD_BUFFER_PLACE4			5		// Z AXIS의 로더 트레이에서 자재를 놓는 높이 (리젝 트레이에 놓는 경우가 있음)
#define Z_LD_VISION_PLACE			6
#define Z_LD_DIRECTION_CHECK		6

//CLIP
#define Y_CLIP_SAFETY				0		// Y AXIS의 로더 트레이 처음 자재 위치
#define Y_CLIP_PICKPOS				1		// Y AXIS의 로더 트레이 마지막 자재 위치
#define Y_CLIP_WORK_PLACE1			2
#define Y_CLIP_WORK_PLACE2			3
#define Y_CLIP_WORK_PLACE3			4
#define Y_CLIP_WORK_PLACE4			5
#define Y_CLIP_REMOVE				6//AXIS의 로더 트레이 처음 자재 위치
#define Y_CLIP_PLACE1_REMOVE		7//AXIS의 로더 트레이 처음 자재 위치

//CLAMP
#define Y_CLAMP_SAFETY				0		// Y AXIS의 로더 트레이 처음 자재 위치
#define Y_CLAMP_WORK_PLACE1			1
#define Y_CLAMP_WORK_PLACE2			2
#define Y_CLAMP_WORK_PLACE3			3
#define Y_CLAMP_WORK_PLACE4			4


//SORT_UNLOAD X축
#define X_SORT_SAFETY				0		// Y AXIS의 로더 트레이 처음 자재 위치
#define X_SORT_WORK_PICK1			1
#define X_SORT_WORK_PICK2			2
#define X_SORT_WORK_PICK3			3
#define X_SORT_WORK_PICK4			4
#define X_SORT_VISION_BUFFER		5
#define X_ULD_TRAY_PLACE			6
#define X_ULD_ETRAY_PLACE			7
#define X_SORT_VISION_BUFFER2		8

#define Z_SORT_SAFETY_UP			0		// Z AXIS의 ROBOT가 이동 가능할 높이
#define Z_SORT_BUFFER_PLACE1		1		// Z AXIS의 로더 트레이에서 자재를 놓는 높이 (리젝 트레이에 놓는 경우가 있음)
#define Z_SORT_BUFFER_PLACE2		2		// Z AXIS의 로더 트레이에서 자재를 놓는 높이 (리젝 트레이에 놓는 경우가 있음)
#define Z_SORT_BUFFER_PLACE3		3		// Z AXIS의 로더 트레이에서 자재를 놓는 높이 (리젝 트레이에 놓는 경우가 있음)
#define Z_SORT_BUFFER_PLACE4		4		// Z AXIS의 로더 트레이에서 자재를 놓는 높이 (리젝 트레이에 놓는 경우가 있음)
#define Z_SORT_VISION_PLACE			5
#define Z_SORT_VISION_PLACE2		6

#define Y_SORT_SAFETY				0		// Y AXIS의 로더 트레이 처음 자재 위치
#define Y_SORT_WORK_PICK1			1
#define Y_SORT_WORK_PICK2			2
#define Y_SORT_WORK_PICK3			3
#define Y_SORT_WORK_PICK4			4
#define Y_SORT_VISION_BUFFER		5
#define Y_SORT_TRAY_START			6		// Y AXIS의 로더 트레이 마지막 자재 위치
#define Y_SORT_TRAY_END				7		// Y AXIS의 왼쪽 버퍼의 처음 위치
#define Y_SORT_ETRAY_START			8		// Y AXIS의 로더 트레이 마지막 자재 위치
#define Y_SORT_ETRAY_END			9		// Y AXIS의 왼쪽 버퍼의 처음 위치
#define Y_SORT_VISION_BUFFER2		10


//Sorter_Unloader
// #define XY_SAFETY					0
// #define XY_WORK_PLACE1				1
// #define XY_WORK_PLACE2				2
// #define XY_WORK_PLACE3				3
// #define XY_WORK_PLACE4				4
// #define XY_LD_VISION1				5
// #define XY_LD_VISION2				6

#define Z_SAFETY					0
#define Z_WORK_PLACE1				1
#define Z_WORK_PLACE2				2
#define Z_WORK_PLACE3				3
#define Z_WORK_PLACE4				4
#define Z_LD_VISION1				5
#define Z_LD_VISION2				6
#define Z_ULD_TRAY_PLACE			7
#define Z_ERR_TRAY_PLACE			8

#define LD_CLIP_SAFETY				0
#define LD_CLIP_POS					1
#define ULD_CLIP_POS				2
//#define REMOVE_CLIP_POS				3

//20131202
#define CLIP_CLAMP					0
#define CLIP_MIDCLAMP				1
#define CLIP_UNCLAMP				2

#define GLIPPER_SAFETY				0
#define GLIPPER_READY_POS			1
#define GLIPPER_READY_POS2			2
#define GLIPPER_READY_POS3			3
#define GLIPPER_READY_POS4			4
#define GLIPPER_CLAMP_POS			5
//

//Glipper Z Axis
#define Z_GLIPPER_SAFETY			0
#define Z_GLIPPER_WORK_POS			1

#define Z_LABEL_ATTACH_SAFETY		0
#define Z_LABEL_PICK_POS			1
#define Z_LABEL_ATTACH_POS1			2
#define Z_LABEL_ATTACH_POS2			3

#define WORK_SAFETY					0
#define WORK_LDMODULE				1
#define WORK_LDLEFTHS				2
#define WORK_LDRIGHTTHS				3
#define WORK_LDCLIP					4
#define WORK_ULDMODULE				5

#define Y_LABEL_SAFETY				0
#define Y_LABEL_POS					1
#define Y_LDMODULE_POS				2
#define Y_VISION_POS				3
#define Y_ULDMODULE_POS				4

#define Z_VISION_SAFETY				0
#define Z_VISION_LABEL				1
#define Z_VISION_LDMODULE			2
#define Z_VISION_CHK				3
#define Z_VISION_ULDMODULE			4


//ULD,LD Stacker MOVE pos
#define P_MSTACKER_STFETY_POS		0
#define P_MSTACKER_LD_POS	 		1
#define P_MSTACKER_ULD_POS	 		2


#define P_NOWPOS					99
///////////////////////////////////////

////// Tray or Buffer Data 관리용
#define MODULE_SITE				0
#define MAX_SITE_INFO			11

#define INIT_RBT_MODULE					0
#define INIT_WORK_BUFFER				1
#define INIT_RBT_LEFT_SINK				2
#define INIT_RBT_RIGHT_SINK				3
#define INIT_RBT_CLIP_MODULE			4
#define INIT_RBT_SORTER					5
#define INIT_LD_STACKER					6
#define INIT_LEFTHS_STACKER				7
#define INIT_RIGHTHS_STACKER			8
#define INIT_CLIP_STACKER				9
#define INIT_LD_CLIP_TRANSFER			10
#define INIT_ULD_STACKER				11
#define INIT_LAVEL_VISION				12

#define INIT_LD_TRAY_TRANSFER			13
#define INIT_LD_LEFTHS_TRAY_TRANSFER	14
#define INIT_LD_RIGHTTHS_TRAY_TRANSFER	15


#define COUNT_NO                0    // 초기 자재 갯수 및 공간 갯수 검사 
#define COUNT_YES               1    // 전체 갯수 및 공간 갯수 검사 

#define INFO_SKIP               0    // 해당 데이터 셋팅하지 않음 
#define INFO_SETTING            1    // 해당 데이터 셋팅함 
#define INFO_ADJUST			    2    // Adjust용.

#define PICKDVC                 0
#define PLACEDVC                1
#define ADJUSTDVC               2

#define DVC_EMPTY               0

#define TOPBUFFER				0
#define BTMBUFFER				1

#define SITE_UPPER_BUFFER		0
#define SITE_LOWER_BUFFER		1

#define SITE_SORTER				0
#define SITE_LABELBUFFER		1
#define SITE_VISCHECK			2
#define SITE_ULDBUFFER			3

#define BIN_CLEARBIN			-1
#define BIN_PASS				1
#define BIN_REJECT				2
#define BIN_HTO					3
#define BIN_NTO					4
#define BIN_BTO					5
#define BIN_RETEST				6
#define BIN_BP					7
#define BIN_ALL					10

#define COUNT_TEST				0

#define RET_PICKERNOTFIND       6		// BIN에 해당되는 결과 미존재 
#define RET_NOWHERE             -1		// 해당 데이터 미존재 
#define RET_NOSPACE             -2		// 공간 미존재 
#define RET_LOADTRAYSKIP        -100	// LOADER OFF / 강제 LOT END 


#define PICKER_DVC_CNT          7
#define PICKER_ENABLE_INFO      8		// 피커 정보 확인 

#define MAIN_BUFFER_DSP			34
#define MAIN_RETEST_DSP			35
#define MAIN_REJECT_DSP			36
#define MAIN_LOB_TRAY_DSP		37
#define MAIN_TRAY_DSP			38
#define MAIN_PICKER_INFO_DSP	39
#define MAIN_CUSTOMER_DSP		40
#define MAIN_AIR_BLOW			67		

#define LOT_A					0
#define LOT_B					1
#define LOT_C					2
//////////////////////////////////////// 20120514 유성준
#define MOVE_TRAY_POS			9	
#define	CLEAN_PARA				4
#define	DRY_PARA				3
#define MAIN_RCV_BARCODE		2312

//////////////////////////////////////// 
//////////////////////////////////////// 
#define MAIN_MYSQL_ALARM_WRITE		1	//20120711
#define	MAIN_MYSQL_LOTALARM_WRITE	2
#define MAIN_MYSQL_LOTWORK_WRITE	3
#define MAIN_LOTEND_READY			4

////////////////////////////////////////////
#define  WORKBUFFERPLACE1 10
#define  WORKBUFFERPLACE2 11
#define  WORKBUFFERPLACE3 12
#define  WORKBUFFERPLACE4 13

enum LOG_TYPE
{ 
	LOG_OPERATION = 0,
	LOG_TESTINTERFACE,
	LOG_TCPIP,
	LOG_MACHINE,
	LOG_DEBUG,
	LOG_BARCODE,
	LOG_TIME,
	LOG_AUTOSOCKETOFF,
	LOG_WORK,
	LOG_ADJUSTMENT,
	LOG_HISTORY,
	LOG_PICK_FAIL,
	LOG_RUN_STOP,
	LOG_MOTOR,
	LOG_DISABLE,
	LOG_TEST,
	LOG_VISION,
	LOG_UPH,//2015.0129
	LOG_UPH_TOT,
	LOG_TOTAL = 99
};

// Work Buffer Supply Init
enum 
{ 
	SUPPLY_INIT_VARIABLE_INIT = -1,
	SUPPLY_LOAD_MODULE_MOVE,
	SUPPLY_LOAD_LEFT_HS_MOVE,
	SUPPLY_LOAD_RIGHT_HS_MOVE,
	SUPPLY_LOAD_CLIP_MOVE,
	SUPPLY_LOAD_END,
	SUPPLY_MOVING
};	

enum 
{ 
	SUPPLY_RUN_VARIABLE_INIT = -1,
	SUPPLY_RUN_LD_ELEVATOR_BASE_MOVE,
	SUPPLY_RUN_ELEVATOR_TRAY_CHECK,
	SUPPLY_RUN_LD_CONVEYOR_TRAY_CHANGE_SET,
	SUPPLY_RUN_LD_CONVEYOR_TRAY_CHANGE_GET,
	SUPPLY_RUN_LD_CONVEYOR_TRAY_CHECK_1_1,
	SUPPLY_RUN_LD_CONVEYOR_TRAY_CHECK_1_0,
	SUPPLY_RUN_LD_CONVEYOR_TRAY_CHECK_0_1,
	SUPPLY_RUN_LD_CONVEYOR_TRAY_CHECK_0_0,
	SUPPLY_RUN_LD_CONVEYOR_MOTOR_ON,
	SUPPLY_RUN_LD_CONVEYOR_TRAY_ARRIVAL_CHECK,
	SUPPLY_RUN_LD_CONVEYOR_MOTOR_OFF,
	SUPPLY_RUN_LD_CONVEYOR_STOPPER_SET,
	SUPPLY_RUN_LD_CONVEYOR_STOPPER_GET,
	SUPPLY_RUN_LD_ELEVATOR_READY_POS,
	SUPPLY_RUN_TRANSFER_TRAY_REQUEST,
	SUPPLY_RUN_TRANSFER_LOT_END,
	SUPPLY_RUN_CONVEYOR_WORK_TRAY_CHECK,
	SUPPLY_RUN_CONVEYOR_WORK_TRAY_NONE_CHECK,
	SUPPLY_RUN_LD_ELEVATOR_BASE_DOWN,
	SUPPLY_RUN_LD_TRAY_UP_POSSIBLE_CHECK,
	SUPPLY_RUN_LD_TRAY_UP_POSSIBLE_CHECK_ERROR,
	SUPPLY_RUN_LD_ELEVATOR_WORK_POS,
	SUPPLY_RUN_LD_ELEVATOR_WORK_POS_EMPTY,
	SUPPLY_RUN_LD_ELEVATOR_TRAY_SUPPLY_WORK_END,
	SUPPLY_RUN_LD_ELEVATOR_BASE_MOVE_LOT_END
};	

// Tray Transfer Init
enum 
{ 
	TRANSFER_INIT_VARIABLE_INIT = -1,
	TRANSFER_INIT_MOTOR_HOME_MOVE,
	TRANSFER_INIT_HOME_END
};	

enum 
{ 
	TRANSFER_RUN_VARIABLE_INIT = -1,
	TRANSFER_RUN_ROBOT_TRAY_REQUEST,
	TRANSFER_RUN_ROBOT_LOT_END,
	TRANSFER_RUN_TRAY_TO_AIRBLOW_REQUEST,
	TRANSFER_RUN_TRAY_FROM_SUPPLY_REQUEST,
	TRANSFER_RUN_TRAY_REQUEST_WORK_END,
	TRANSFER_RUN_TRAY_TO_2ZONE_START,
	TRANSFER_RUN_CLAMP_OFF_SET,
	TRANSFER_RUN_CLAMP_OFF_GET,
	TRANSFER_RUN_HOOK_ON_SET,
	TRANSFER_RUN_HOOK_ON_GET,
	TRANSFER_RUN_TRAY_MOVE_SET,
	TRANSFER_RUN_TRAY_MOVE_GET,
	TRANSFER_RUN_TRAY_MOVE_CHECK,
	TRANSFER_RUN_TRAY_MOVE_DATA_SHIFT,


	TRANSFER_RUN_SUPPLY_REQUEST = 1000,
	TRANSFER_RUN_SUPPLY_TRAY_EMPTY_CHECK,
	TRANSFER_RUN_SUPPLY_CYLINDER_READY_SET,
	TRANSFER_RUN_SUPPLY_CYLINDER_READY_GET,
	TRANSFER_RUN_SUPPLY_TO_ELEVATOR_REQ,
	TRANSFER_RUN_SUPPLY_TO_ELEVATOR_WORK_END_CHECK,
	TRANSFER_RUN_SUPPLY_LD_ZONE_1_TRAY_CHECK,
	TRANSFER_RUN_SUPPLY_REQUEST_WORK_END,

	TRANSFER_RUN_AIRBLOW_REQUEST,
	TRANSFER_RUN_AIRBLOW_BOX_EMPTY_CHECK,
	TRANSFER_RUN_AIRBLOW_TRAY_IN_REQUEST,
	TRANSFER_RUN_AIRBLOW_TRAY_IN_REQUEST_WORK_END,
	TRANSFER_RUN_AIRBLOW_TRAY_HOOK_ON_SET,
	TRANSFER_RUN_AIRBLOW_TRAY_HOOK_ON_GET,
	TRANSFER_RUN_AIRBLOW_TRAY_MOVE,
	TRANSFER_RUN_AIRBLOW_DATA_SHIFT,
	TRANSFER_RUN_AIRBLOW_TRAY_MOVE_HOOK_DOWN,
	TRANSFER_RUN_AIRBLOW_TRAY_HOOK_OFF_SET,
	TRANSFER_RUN_AIRBLOW_TRAY_HOOK_OFF_GET,
	TRANSFER_RUN_AIRBLOW_TRAY_MOVE_READY,
	TRANSFER_RUN_AIRBLOW_REQUEST_WORK_END
};

// Airblow Box Init
enum 
{ 
	AIRBLOW_INIT_VARIABLE_INIT = -1,
	AIRBLOW_INIT_MOTOR_HOME_MOVE,
	AIRBLOW_INIT_HOME_END
};	

enum 
{ 
	AIRBLOW_RUN_VARIABLE_INIT = -1,
	AIRBLOW_RUN_TRANSFER_REQUEST,
	AIRBLOW_RUN_TRANSFER_LOT_END,
	AIRBLOW_RUN_IN_SHUTTER_OFF_SET,
	AIRBLOW_RUN_IN_SHUTTER_OFF_GET,
	AIRBLOW_RUN_TRANSFER_CHANGE,
	AIRBLOW_RUN_IN_SHUTTER_ON_SET,
	AIRBLOW_RUN_IN_SHUTTER_ON_GET,
	AIRBLOW_RUN_SHUTTER_CHECK,
	AIRBLOW_RUN_DUST_COLLECTOR_ON,
	AIRBLOW_RUN_CW_MOVE_START,
	AIRBLOW_RUN_CW_MOVE_END,
	AIRBLOW_RUN_CCW_MOVE_START,
	AIRBLOW_RUN_CCW_MOVE_END,
	AIRBLOW_RUN_MOVE_CNT_OUT,
	AIRBLOW_RUN_MOTOR_OFF,
	AIRBLOW_RUN_OUT_SHUTTER_OFF_SET,
	AIRBLOW_RUN_OUT_SHUTTER_OFF_GET,
	AIRBLOW_RUN_EJECTOR_REQUEST,
	AIRBLOW_RUN_EJECTOR_REQUEST_READY,
	AIRBLOW_RUN_PUSHER_CW_MOVE_START,
	AIRBLOW_RUN_PUSHER_CW_MOVE_END,
	AIRBLOW_RUN_EJECTOR_CHANGE_REQUEST,
	AIRBLOW_RUN_PUSHER_CCW_MOVE_START,
	AIRBLOW_RUN_PUSHER_CCW_MOVE_END,
	AIRBLOW_RUN_OUT_SHUTTER_ON_SET,
	AIRBLOW_RUN_OUT_SHUTTER_ON_GET
};


enum { BLOT_MAX = 9, };



// *****************************************************************************
// 사용자 정의 메시지 ID 선언                                                   
// *****************************************************************************
#define WM_FORM_CHANGE				WM_USER+200   // 화면이 전환된 때 해당 전환 상태 정보를 전송하기 위한 사용자 정의 메시지 
#define WM_STATUS_CHANGE			WM_USER+201   // 타이틀 바 화면으로 전송된 메시지 출력시키기 위한 사용자 정의 메시지 추가 
#define WM_LIST_DATA				WM_USER + 6   // 통신 메시지 출력 리스트 대화 상자에 

#define WM_DATA_SEND				WM_USER + 7   // Data를 전송하기 위한 화면에서 정보를 전송하기 위해 정의한 사용자 정의 함수
#define WM_WORK_END					WM_USER + 8   // 테스트 작업 완료 시 정보를 메인 화면에 출력시키기 위한 사용자 정의 메시지 추가 
#define WM_SCRAP_DSP				WM_USER + 9

#define WM_BASIC_APPLY				WM_USER + 10  // BASIC DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가 
#define WM_MAINTENANCE_APPLY		WM_USER + 11  // Maintenance DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가
#define WM_TESTREFERENCE_APPLY		WM_USER + 12
#define WM_INTERFACE_APPLY			WM_USER + 13
#define	WM_WAITTIME_APPLY			WM_USER + 14  // Wait Time DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가 

#define	WM_MOTORTEACH_APPLY			WM_USER + 15
#define	WM_SHIFTER_APPLY			WM_USER + 16
#define	WM_MOTORSPEED_APPLY			WM_USER + 17

#define WM_SERIAL_PORT				WM_USER + 18
#define WM_NETWORK_MSG				WM_USER + 19

#define WM_MAINFRAME_WORK			WM_USER + 20
#define WM_WORK_MSG					WM_USER + 21

#define DIAGNOSE_START				WM_USER + 22
#define DIAGNOSE_END				WM_USER + 23

#define SELF_DIAGNOSE_END			WM_USER + 25
#define	WM_MAIN_EVENT				WM_USER + 26
#define WM_TRACKOUT_DSP				WM_USER + 27
#define	WM_A_STATION_MESSAGE		WM_USER + 28
#define	WM_B_STATION_MESSAGE		WM_USER + 29
#define	WM_C_STATION_MESSAGE		WM_USER + 30
#define	WM_D_STATION_MESSAGE		WM_USER + 31
#define	WM_E_STATION_MESSAGE		WM_USER + 32
#define	WM_F_STATION_MESSAGE		WM_USER + 33
#define	WM_G_STATION_MESSAGE		WM_USER + 34
#define	WM_H_STATION_MESSAGE		WM_USER + 35

#define WM_RECEIVED_NEW_LOT			WM_USER + 36
#define WM_DRAW_LOT_TIME			WM_USER + 37
#define WM_DRAW_WORK_SITE			WM_USER + 38

#define WM_TRAYROW_SET			    WM_USER + 40
#define WM_BUFFER_SET	    		WM_USER + 41

#define WM_ANI_WORK					WM_USER + 42  //20120704
#define WM_BCR_ML_END               WM_USER + 43  //20130103



////////////////////////////////////////////// 20120514 유성준
#define WM_DATA_CHANGE          WM_USER + 301 // apply버튼시 저장까지, 해당 화면의 데이터 적용(Init)
#define WM_WORK_DISPLAY			WM_USER + 400
#define WM_WORK_COMMAND			WM_USER + 401
//////////////////////////////////////////////

#define WM_CLIENT_MSG			WM_USER + 500
#define WM_CLIENT_MSG_1			WM_USER + 500
#define WM_CLIENT_MSG_2			WM_USER + 501
#define WM_CLIENT_MSG_3			WM_USER + 502
#define WM_CLIENT_MSG_4			WM_USER + 503
#define WM_CLIENT_MSG_5			WM_USER + 504
#define WM_CLIENT_MSG_6			WM_USER + 505
#define WM_CLIENT_MSG_7			WM_USER + 506
#define WM_CLIENT_MSG_8			WM_USER + 507
#define WM_CLIENT_MSG_9			WM_USER + 508
#define WM_CLIENT_MSG_10		WM_USER + 509

#define WM_SERVER_MSG			WM_USER + 550
#define WM_SERVER_MSG_1			WM_USER + 550
#define WM_SERVER_MSG_2			WM_USER + 551
#define WM_SERVER_MSG_3			WM_USER + 552
#define WM_SERVER_MSG_4			WM_USER + 553
#define WM_SERVER_MSG_5			WM_USER + 554
#define WM_SERVER_MSG_6			WM_USER + 555
#define WM_SERVER_MSG_7			WM_USER + 556
#define WM_SERVER_MSG_8			WM_USER + 557
#define WM_SERVER_MSG_9			WM_USER + 558
#define WM_SERVER_MSG_10		WM_USER + 559

// *****************************************************************************
//  Main Screen 출력용 메세지 정의                                              
// *****************************************************************************
#define MAIN_INDEXTIME		1
#define MAIN_LOADERSET		2
#define MAIN_LOADINFO		3
#define MAIN_UNLOADINFO		4
#define MAIN_BUFFERINFO		5
#define MAIN_RETESTINFO		6
#define MAIN_REJECTINFO		7
#define MAIN_PICKERINFO		8
#define MAIN_TIMEINFO		9
#define MAIN_COUNTINFO		10
#define MAIN_BININFO		11
#define MAIN_TRAYINFO		14
#define MAIN_LOADERCONTI	15
#define MAIN_LOTINFO		21
#define MAIN_CYCLETIME		22
#define MAIN_BUFFER_INFO	25
#define MAIN_REJECT_TRAY	26
#define MAIN_LOT_RESTART	27
#define MAIN_EQP_LOTEND		28
////////////// 20121002
#define MAIN_VISION_CNT_RESET 29
//////////////
#define MAIN_CAP_REMOVE_CHECK 30

#define MAIN_POWER_DISPLAY	100
#define MAIN_STATUS_DISPLAY 101

#define MAIN_BCR_DISPLAY	102	//20120524
#define INTERFACE_BCR_DISPLAY 103	//20120608

#define BARCODE_HS_DISPLAY  104	//20120625
#define BASIC_BCR_DISPLAY   105 //20130103

#define BARCODE_CREATE_BUTTON 106 //2014,0411

#define MAIN_MODULE_STOCKERINFO_L		30
#define MAIN_MODULE_STOCKERINFO_U		31
#define MAIN_MODULE_COUNTINFO			32
#define MAIN_MODULE_PICKERINFO_L		33
#define MAIN_MODULE_PICKERINFO_U		34
#define MAIN_MODULE_TABLEINFO			35
#define MAIN_MODULE_TRAYTRANSFERINFO_L	36		// Tray Transfer
#define MAIN_MODULE_TRAYTRANSFERINFO_U	37		// Tray Transfer
#define MAIN_MODULE_VCRINFO				38

// *****************************************************************************
#define TIMECOUNT			5
#define VARIABLE_INIT		0
#define LANGUAGE_KOREA		0		// 한글 OS
#define LANGUAGE_ENGLISH	1		// 영어 OS


#define MAXSITE				13		// 구분 되어진 사이트 갯수. (초기화는 사이트 별로 구분한다.)
#define MAX_PORT			7

#define COM_ERROR			-1		// 에러 메시지
#define COM_READY			-2	// 

// #define BCR_PORT1			2
// #define BCR_PORT2			3
// #define BCR_PORT3			4
// #define BCR_PORT4			5
// 
#define GMS_PORT		             0
#define LABEL_PRINTER_PORT	         1	//20120514 유성준
#define VISION_PORT					 2 //BARCODE_PORT                 4
#define MODULE_LOAD_PORT             3
#define HEATSINK_LEFT_PORT           4
#define HEATSINK_RIGHT_PORT          5
#define CLIP_PORT                    6


////////////////////////////////////////////// 20120608
#define MAIN_VISION_SEND	     	 0
#define VISION_CJ			     	 1
#define VISION_TKIN				     2
#define VISION_TKOUT			     3
#define VISION_YIELD			     4


#define HEATSINK_CJ			     	 5
#define HEATSINK_TKIN			     6
#define HEATSINK_TKOUT			     7
#define HEATSINK_YIELD			     8

#define VISION_ML        	         9
#define HEATSINK_ML 	    	     10
#define VISION_ML_YES    	         13
#define HEATSINK_ML_YES	    	     14

#define VISION_YIELD_YES 	         11
#define HEATSINK_YIELD_YES		     12

#define VISION_TRIGGER			     20
#define HEATSINK_TRIGGER			 21

#define VISION_TRIGGER_RESET	     22//20120927
#define HEATSINK_TRIGGER_RESET		 23//20120927

//////////////////////////////////////////////


#define BCR_SND_WAIT		10000
#define BCR_RCV_WAIT		50000

/////////////////////////////////////// 20120514 유성준
#define COM_PORT			0
#define COM_BAUDRATE		1
#define COM_DATA			2
#define COM_STOP			3
#define COM_PARITY			4

#define NETWORK_CLIENT			1
#define NETWORK_CLIENT_1		1
#define NETWORK_CLIENT_2		2
#define NETWORK_CLIENT_3		3
#define NETWORK_CLIENT_4		4
#define NETWORK_CLIENT_5		5
#define NETWORK_CLIENT_6		6
#define NETWORK_CLIENT_7		7 
#define NETWORK_CLIENT_8		8 
#define NETWORK_CLIENT_9		9
#define NETWORK_CLIENT_10		10

///////////////////////////////////////


#define INIT_BARCODE		0
#define INIT_SHIFTER_F		1
#define INIT_SHIFTER_R		2
#define INIT_BUFFER_F		3
#define INIT_BUFFER_R		4

#define IO_OFF				0
#define IO_ON				1
#define IO_ERROR			2
#define IO_CHK_ERROR		4

#define TMR_MOT_IO_CHECK			2
#define TMR_MOT_POS_CHECK			3
#define TMR_MOT_HOME_CHECK			4
#define TMR_MOT_MOVE_CHECK			5
#define TMR_MOT_MOVE_TEST			6
#define TMR_CYL_IO_CHECK			7
#define TMR_CYL_IO_TEST				8
#define TMR_IO_CHECK				9
#define TMR_BD_ENABLE				10
#define TMR_BD_DISABLE				11

#define TMR_PCBOXQUERY			501

#define EJECTOR_DOWN		1
#define EJECTOR_UP			0
#define EJECTOR_ERROR		2
#define EJECTOR1_ERROR		3
#define EJECTOR2_ERROR		4

// 메뉴얼 화면에서 사용한다.
#define MOT_MOVE			2
#define MOT_HOME			1

#define HOMING				1
#define MOVING				2
#define MULTI_MOVING		3
#define POS_MOVING			4

// *****************************************************************************
//  메뉴에서 사용할 ID
// *****************************************************************************
#define MENU_SETTING		0
#define MENU_MOTOR			1
#define MENU_LIST			2

// ******************************************************************************
// 장비 동작 제어 시 필요한 메시지 정의                                          
// ******************************************************************************
#define dSTOP			0
#define dRUN			1
#define dJAM			2
#define dLOTEND			3
#define dINIT			4
#define dWARNING		5
#define dLOCK			6
#define dSELFCHECK		7
#define dMAINT			8
#define dREINSTATE		9	//복귀동작중
#define dLOTREADY		10	

#define eWARNING		0
#define eRUNNING		1
#define eSTOP			2
#define eERROR			3

// *****************************************************************************
//  Title Bar 출력용 메세지 정의                                                
// *****************************************************************************
#define FORM_NAME			1
#define MACHINE_STATUS		2
#define RETEST_MODE			3
#define DEVICE_MODE			4
#define LEVEL_MODE			5
#define FRONT_KEY_MODE		6
#define REAR_KEY_MODE		7
#define SKIP_MODE			8	//20120525
#define BCR_MODE			9
#define DIRECTION_MODE		10


// *****************************************************************************
//  List Box 출력용 메세지 정의                                                 
// *****************************************************************************
#define RECEIVE_MSG			1
#define SEND_MSG			2
#define NORMAL_MSG			3
#define ABNORMAL_MSG		4

#define MACHINE_INFO		5
#define DEVICE_INFO			6

#define TCPIP_MSG			7
#define BARCODE_MSG			8

#define RECEIVE_MSG			1
#define SEND_MSG			2
#define NORMAL_MSG			3
#define ABNORMAL_MSG		4

#define CLIENT_CONNECT		0
#define CLIENT_CLOSE		1
#define CLIENT_SEND			2
#define CLIENT_REV			3

#define SERVER_CONNECT		0
#define SERVER_CLOSE		1
#define SERVER_SEND			2
#define SERVER_REV			3

// ******************************************************************************
// I/O 모듈 갯수 및 포트 최대 갯수 선언한다                                      
// ******************************************************************************
#define MAX_MODULE		37
#define MAX_MAP			32
#define MAX_IN			4016
#define MAX_IO			4016
// ******************************************************************************

#define ELEC_PORT		5

// ******************************************************************************
//  MOTOR 보드 관련 변수 선언한다                                                
// ******************************************************************************
#define MOT_GOOD		1
#define MOT_PROGRESS	2
#define MOT_RETRY		3
#define MOT_ERROR		4

#define PLUS			1
#define MINUS			0
#define RUN_MODE		1
#define MANUAL_MODE		2

#define AMPOFF			0
#define AMPON			1

#define ELM				1
#define ELP				2
#define HOME			0

#define MOT_ACC			0
#define MOT_DEC			1
#define MOT_JOG			2
#define MOT_ORG			3
#define MOT_FEEDRATE	4
#define MOT_INTERACC	5

#define MOT_FAST		10
#define MOT_SLOW		20


////////////////////////////////////////// 20120514 유성준 - 사용안함 
#define MOT_MIDDLE_PUSH				10


//////////////////////////////////////////

// ******************************************************************************
// 자재 테스트 및 소팅 관련 갯수 선언한다                                        
// ******************************************************************************
#define BINNUM			16
#define TESTNUM			1
#define MAX_PARA		10
#define TESTSIZE		10

#define BIN_1			1
#define BIN_2			2
#define BIN_3			3
#define BIN_4			4
#define BIN_5			5
#define BIN_6			6
#define BIN_7			7
#define BIN_8			8
#define BIN_9			9
#define BIN_10			10
#define BIN_11			11
#define BIN_12			12
#define BIN_13			13
#define BIN_14			14
#define BIN_15			15
#define BIN_16			16

#define NO_DEVICE		99
// ******************************************************************************

enum eSocketState
{
	SOCKET_STATE_NONE		= -1,
	SOCKET_STATE_ON			= 1,
	SOCKET_STATE_MANUAL_OFF	= 2,
	SOCKET_STATE_AUTO_C_OFF = 3,
	SOCKET_STATE_AUTO_R_OFF = 4,
	SOCKET_STATE_AUTO_A_OFF = 5
};

#define SOCKET_ON			1		// Socket이 살아있다.
#define MANUAL_OFF			2		// 메뉴얼로 Socket Off를 시켰다.
#define AUTO_C_OFF			3		// 연속 Fail로 Auto Socket Off가 됐다.
#define AUTO_R_OFF			4		// 상대 Fail로 Auto Socket Off가 됐다.
#define AUTO_A_OFF			5		// 절대 Fail로 Auto Socket Off가 됐다.

#define T_NOTTEST			0
#define T_PRIMETEST			1
#define T_RETEST			2

// SOCKET DATA관련.
#define LOT_DATA			0	// 해당 LOT에만...
#define DAY_DATA			1	// 당일
#define MONTH_DATA			2	// 당월
#define SHIFT_DATA			3	// 당 Shift

#define NO					0
#define YES					1

#define READY               2  //20121208

#define DVC_NO				0
#define DVC_YES				1
#define PICKERSTATUS		10
#define PICKERSTATUS_ON		11
#define PICKERSTATUS_OFF	12

#define TRAYCOL				0
#define TRAYROW				1

#define LOADTRAY_ROW		30
#define LOADTRAY_COL		3
#define UNLOADTRAY_ROW		30
#define UNLOADTRAY_COL		3
#define REJECTTRAY_ROW		30
#define REJECTTRAY_COL		3

#define BUFFERMAXSIZE		20		// 버퍼 각각의 등급 구분 정보[0][x] 및 모듈 유/무 정보[1][x]를 가진다 
#define BUFFERGRADE			10		// 0: buffer 구분 등급설정, 1:해당 소켓의 Module 유/무 정보 

#define BINCATEGORY			5
#define BIN_GOOD			1
#define BIN_REJECT			2

#define TESTSTART			1
#define TESTEND				2
#define TESTING				3


#define MAX_SOCKET_POS		160
#define TEST_END_OK			1000

#define P_F_BUFFER			0
#define P_R_BUFFER			1
#define P_LOB				2
#define P_1_LOB				2
#define P_2_LOB				3
#define P_3_LOB				4
#define P_1_TRAY			5
#define P_2_TRAY			6
#define P_RETEST_BUFFER		7
#define P_REJECT_BUFFER		8
#define P_CUSTOMER_BUFFER	9

//MOT Speed Setting  
#define	MOT_SPD_VEL			0
#define	MOT_SPD_ACC			1
#define MOT_SPD_DEC			2
#define MOT_SPD_JOG			3
#define MOT_SPD_ORG			4
#define MOT_SPD_FEEDRATE	5
#define MOT_SPD_INTERACC	6

//motor sensor 
#define MOT_SENS_HOME		0
#define MOT_SENS_ELM		1
#define MOT_SENS_ELP		2
#define MOT_SENS_SD			3 //slow down sensor 

//모터 방향 정의 
#define MOT_PLUS			1
#define MOT_MINUS			0

// FRONT/REAR
#define POS_REAR			st_sync.mn_pos_rear
#define POS_FRONT			st_sync.mn_pos_front

enum
{
	POS_NONE = -1,
	POS_COUNT = 2,
};

enum { SOCKET_COUNT = 20 };
enum { BOARD_COUNT = 10 };

#define MOT_RTY_CNT			4	//파스텍 제어기 관련 문제시 재 시도하는 횟수

#define MOT_MOVE_LIMITTIME	60000	//이동 후 3초이내동작 완료할 것 	 
#define MOT_TIMEOUT			100000   //1분의 time out를 가지고 동작한다  //홈 체크시 사용 

//에러등 메세지 지정시사용 
#define MOT_NORMAL_MSG		0
#define MOT_ERR_MSG			1
#define MOT_ERR_CODE		2

#define MOT_LmMAPINDEX			0	//모터 리스트 모션 정의 
#define MOT_IxMAPINDEX			1	//모터 직선 보간 정의 

///////////////////////////////////
// IO Board Define
//////////////////////////////////
// ==============================================================
// handler 동작 관련 피킹 / 자재 정보 저장 플래그
#define D_INFOSIZE		4		// 몇가지의 정보를 사용할건지..
#define D_EXIST			0		// 자재 유무 관리
#define D_BIN			1		// 빈 등급 관리
#define D_RE_CNT		2		// 리테스트 횟수 관리
#define D_ARRAY			3		// ARRAY 관리
#define D_SCR_BIN		3		// SCREEN 표시

// ==============================================================

#define APCT_READY		-1
#define APCT_SITE1		0
#define APCT_SITE2		1
#define APCT_SITE3		2
#define APCT_SITE4		3
#define APCT_SITE5		4
#define APCT_SITE6		5
#define APCT_SITE7		6
#define APCT_SITE8		7
#define APCT_BYPASS		8

#define RET_PROCEED		0
#define RET_GOOD		1
#define RET_ERROR		2
#define RET_RETRY		3
#define RET_DOOROPEN	4		// Door Open이 되어 있음. 2K6/06/20/ViboX

#define WORK_INIT				-1
#define	WORK_READY				0
#define	WORK_START				1
#define	WORK_END				2
#define	WORK_MOVE				3
#define	PGM_START				4
#define	WORK_PROCEED			5
#define	WORK_OFFLINE_START		6
#define	WORK_OFFLINE_END		7
#define	WORK_OFFLINE_PROCEED	8

///////////////////////////////////////////
// Control Board(IO, Motor) Board Define
///////////////////////////////////////////

//091010 I/O 동작시 Out/In 동작 체크시 패턴을 아래와 같이 정의하여 동작 완료 여부를 정확히 확인하자 
#define CTL_IOCHK_CLEAR				0 //I/O 동작을 시작하는 위치(Output 시그널 동작시 사용)
#define CTL_IOCHK_START				1 //I/O 아웃풋 동작을 완료하여 기다리는 시간이 시작하는 위치(Input 체크 조건이 처음 만족 하는 위치) 
#define CTL_IOCHK_END				2 //I/O 동작을 input 대기 시간까지 모두 완료하여 끝난 상태
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define HANDLER_BACKUP_DATA_RW	29
#define DATA_READ    0
#define DATA_WRITE   1

// 칼라 정의
#define SKY_C					RGB(187, 250, 255)
#define SKY_D					RGB(0, 255, 255)
#define SKY_L					RGB(128, 255, 255)
#define NOR_C					RGB(164, 164, 164)
#define NOR_L					RGB(190, 190, 190)
#define RED_C					RGB(255, 0, 0)
#define RED_L					RGB(255, 150, 150)
#define RED_D					RGB(80, 20, 0)
#define RED_A					RGB(200, 0, 0)
#define BLUE_C					RGB(0, 0, 255)
#define WHITE_C					RGB(255, 255, 255)
#define BLACK_C					RGB(25, 1, 1)
#define BLACK_L					RGB(62, 55, 55)
#define YELLOW_C				RGB(255,255,0)
#define YELLOW_D				RGB(128, 128, 0)
#define YELLOW_L				RGB(255, 255, 128)
#define GREEN_C					RGB(0,255,0)
#define GREEN_B					RGB(0,255,64)
#define GREEN_L					RGB(150,255,150)
#define GREEN_D					RGB(0,100,20)
#define DSERBLE_BC				RGB(210,210,230)
#define TEXT_BC					RGB(220,230,210)
#define OK_BC					RGB(0, 0, 255)
#define NG_BC					RGB(255, 0, 0)
#define RESULT_BC				RGB(40, 1, 1)
#define ORANGE_C				RGB(0xFF, 0x68, 0x20)
#define ORANGE_L				RGB(255, 200, 128)
#define BLACK					RGB(0, 0, 0)
#define BLUE_D					RGB(0, 0, 128)
#define BLUE				    RGB(0, 0, 255)
#define GRAY_C					RGB(192, 192, 192)
//#define BLUE_L					RGB(150, 150, 255)
#define BLUE_L					RGB(124, 139, 252)
#define ON_C					RGB(0, 255, 0)
#define OFF_C					RGB(0, 150, 0)
#define NG_C					0x0000FF
#define OK_C					0xFF00
#define BLACK_GC				0x00
#define YELLOW_GC				0xFFFF
#define TEST_BC					RGB(194, 231, 150)
#define IN_BC					RGB(204, 153, 255)
#define PINK_D					RGB(252, 233, 218)

#define DVC_READY_C				RGB(153, 153, 255)
#define DVC_LOAD_C				RGB(239, 214, 198)
#define DVC_PGM_C				RGB(255, 255, 0)
#define DVC_UNLOAD_C			RGB(255, 255, 204)
#define DVC_NONE_C				RGB(150, 150, 150)
#define DVC_START_C				RGB(150, 255, 150)
#define DVC_NO_C				RGB(128, 128, 0)

//Grid 정의 
#define DATE					0
#define EDIT					1
#define COMBO					2
#define BUTTON					3
#define STATIC					4
#define TIME					5
#define PERCENT					6
#define CHECK1					7
#define NUMBER					8
#define NUMBER1					9

#define IO_IN					1
#define IO_OUT					0
#define IO_NONE					2

#define CALPELLA_A_PORT			WM_USER+2000
#define CALPELLA_B_PORT			WM_USER+2001
#define CALPELLA_C_PORT			WM_USER+2002
#define CALPELLA_D_PORT			WM_USER+2003
#define CALPELLA_E_PORT			WM_USER+2004
#define CALPELLA_F_PORT			WM_USER+2005
#define CALPELLA_G_PORT			WM_USER+2012
#define CALPELLA_H_PORT			WM_USER+2013
#define KVM_PORT				WM_USER+2006

#define MAIN_POWER_OFF_CHK		WM_USER+2007
#define MAIN_NOVIDEO_CHK		WM_USER+2008
#define MAIN_BOARD_VGA			WM_USER+2009
#define MAIN_TEMP				WM_USER+2010
#define HEATER_WRITE			WM_USER+2011

// *****************************************************************************

// *****************************************************************************
//  네트웍 메세지 정의 장비쪽에서 BPC로 보내는 메세지.
// *****************************************************************************
#define NW_TRACK_IN						95
#define NW_PCBOX_TIMEOUT				96
#define NW_PCBOX_OFF					12
#define NW_TRACK_OUT					97
#define NW_BPC_BUFFER_READY				151
#define NW_BPC_BUFFER_IN				152
// *****************************************************************************

////////////////////////////////////////////////////////////////////////////////
// AMT8480 전용
////////////////////////////////////////////////////////////////////////////////
#define NW_LOT_DISPLAY					25		// 서버에 해당 LOT의 정보를 요청하여, 이를 바탕으로 Test 기분 적용 및 수량 정보 활용
#define NW_LOT_DISPLAY_RPY				46		
#define NW_ONLINE_END					94		// 불량 정보를 서버에 전송하여, 최종 Track-Out시 Scrap 수량에 반영 함.
#define NW_ONLINE_END_RPY				205		
#define NW_SCRAP_INFO					107		// 서버에 저장된 불량 정보를 받아 옴.
#define NW_SCRAP_INFO_RPY				108
#define NW_DELETE_SCRAP					109		// 서버에 저장된 불량 정보를 삭제 함.
#define NW_DELETE_SCRAP_RPY				110
#define NW_SORTER_LOT_END				111		// Sorter에서 서버에 Lot End를 보내 전산 처리를 함.
#define NW_SORTER_LOT_END_RPY			112
// *****************************************************************************
//  네트웍 메세지 정의 BPC쪽에서 장비로 보내는 메세지.
// *****************************************************************************
#define NW_TEMP_CONTROL					93
#define NW_PCBOX_START					31
#define NW_PGM_START					60
#define NW_PCBOX_END					32
// *****************************************************************************
// *****************************************************************************
//  네트웍 메세지 정의 BPC<->EPC 서로 주고받는 메세지.
// *****************************************************************************
#define NW_PCBOX_STATUS					98
#define NW_PCBOX_DOWN					99
#define NW_LOT_CANCEL					30

#define NW_PCBOX_STATUS_SEND			102
#define NW_PCBOX_DOWN_SEND				103
#define NW_LOT_CANCEL_SEND				104
// *****************************************************************************

// *****************************************************************************
//  네트웍 메세지 정의. 장비간 통신.
// *****************************************************************************
#define NW_BUFFER_READY					201
#define NW_BUFFER_IN					202
#define NW_RETEST_REQ					219

#define NW_SOCKET_CHK_SEND				214
#define NW_BUFFER_READY_SEND			215
#define NW_BUFFER_IN_SEND				216

#define NW_BUFFER_IN_RPY				217
#define NW_BUFFER_READY_RPY				218
#define NW_RETEST_REQ_RPY				220
// *****************************************************************************

#define NW_TRACK_IN_RPY					203
#define NW_PCBOX_TIMEOUT_RPY			204
#define NW_PCBOX_OFF_RPY				206
#define NW_LAST_RETEST_RPY				207
#define NW_TRACK_OUT_RPY				208		// 여기까진 EPC의 답변

#define NW_PCBOX_STATUS_RPY				209
#define NW_PCBOX_DOWN_RPY				210
#define NW_LOT_CANCEL_RPY				211
#define NW_VIDEO_SELECT_RPY				213		// 여기까진 서로 보내는 부분.

#define NW_APCTSOCKETINFO				214
#define NW_APCTSOCKETINFO_RPY			215

#define NW_BOOT_OK						221
#define NW_NEW_LOT_IN					222
#define NW_NEW_LOT_IN_SEND				223
#define NW_NEW_LOT_IN_RPY				224

#define NW_LOT_NAME						225
#define NW_LOT_NAME_RPY					226

#define NW_BLOT_COUNT					231
#define NW_BLOT_COUNT_RPY				232

#define NW_BPC_BUFFER_READY_RPY			233	
#define NW_BPC_BUFFER_IN_RPY			234

#define NW_MAT_CHECK					241
#define NW_MAT_CHECK_RPY				242

#define NW_VERSION_UPDATE_RPY			990
#define NW_INVALID_FUNCTION				999

// 이하는 VLCA/320/420에 썼던 프로토콜들.
// *****************************************************************************
//  네트웍 메세지 정의 장비쪽에서 BPC로 보내는 메세지.
// *****************************************************************************
#define NW_LOT_START_RPY				1
//#define NW_LOT_CANCEL_RPY				2
#define NW_PCBOX_START_RPY				3
#define NW_PCBOX_END_RPY				4
#define NW_PCBOX_CHANGE_RPY				5
#define NW_ENABLE_RPY					6
#define NW_DISABLE_RPY					7
#define NW_SLOT_MASK_RPY				8
#define NW_LOT_START_ENABLE_RPY			9
#define NW_PCBOX_REMOVE_RPY				10
#define NW_PCBOX_ERROR_RPY				11
#define NW_UNLOAD_COMPT					13
#define NW_CANCEL_END					14
#define NW_LOT_END						15
#define NW_PCBOX_DISABLE				16
#define NW_VIDEO_TIMEOUT				17
#define NW_HOLD_TIMEOUT					18
#define NW_PCBOX_LOT_INFO				19
#define NW_FAIL_INFO_SEND				20
#define NW_FAIL_REGNUMBER_REQUEST		21
#define NW_FAIL_INFO_SERACH				22
#define NW_FAIL_INFO_SAVE				23
#define NW_FAIL_INFO_DELETE				24
#define NW_EQP_LOT_START				26
#define NW_EQP_LOT_CANCEL				27
#define NW_EQP_LOT_END					28
#define NW_PCBOX_OFF_CHECK				100		// 새로 추가됨. 2K11/03/02/ViboX
#define NW_POWER_OFF_CHECK_RPY			101
#define NW_EQP_ENABLE_RPY				200
#define NW_EQP_DISABLE_RPY				201
#define NW_EQP_ENABLE					200
#define NW_EQP_DISABLE					201
#define NW_LOT_START					29
#define NW_PCBOX_CHANGE					33
#define NW_ENABLE						34
#define NW_DISABLE						35
#define NW_SLOT_MASK					36
#define NW_LOT_START_ENABLE				37
#define NW_PCBOX_REMOVE					38
#define NW_PCBOX_ERROR					39
#define NW_PCBOX_LOT_INFO_RPY			40
#define NW_FAIL_INFO_SEND_RPY			41
#define NW_FAIL_REGNUMBER_REQUEST_RPY	42
#define NW_FAIL_INFO_SERACH_RPY			43
#define NW_FAIL_INFO_SAVE_RPY			44
#define NW_FAIL_INFO_DELETE_RPY			45
#define NW_EQP_LOT_START_RPY			47
#define NW_EQP_LOT_CANCEL_RPY			48
#define NW_EQP_LOT_END_RPY				49
#define NW_PCBOX_ENABLE					50
#define NW_POWER_CONTROL_ON				71
#define NW_POWER_CONTROL_OFF			72
#define NW_POWER_OFF					82
#define NW_VIDEO_TIMEOUT_RPY			91
#define NW_POWER_OFF_RPY				92

#define NW_LABEL_INFO					301		// 서버에 해당 LOT의 정보를 요청하여, 이를 바탕으로 Test 기분 적용 및 수량 정보 활용
#define NW_LABEL_INFO_RPY				302		// 서버에 해당 LOT의 정보를 요청하여, 이를 바탕으로 Test 기분 적용 및 수량 정보 활용
#define NW_LABEL_FAIL_INFO_SEND			303
#define NW_LABEL_LOT_END_RPY			304
#define NW_LABEL_SAMPLE_INFO			305
#define NW_LABEL_SAMPLE_INFO_RPY		306

#define NEW_LOT_START					307	// 20130115
#define NEW_LOT_START_RPY				308	// 20130115
#define NEW_LOT_END						309	// 20130115
#define NEW_LOT_END_RPY					310	// 20130115
#define NEW_LOT_CANCEL					311	// 20130115
#define NEW_LOT_CANCEL_RPY				312	// 20130115
#define NEW_MAT_CHECK					313	// 20130115
#define NEW_MAT_CHECK_RPY				314	// 20130115

// *****************************************************************************
// 화면 동작 관련 기본 데이터 저장 구조체 변수 선언                             
// *****************************************************************************
struct st_handler_param
{
	int mn_menu_num;				// 각 화면에 설정된 화면 번호 저장 변수 
	int mn_alarm_screen;			// 현재 알람 화면이 출력되어져 있는지에 대한 정보 저장 변수 
	int mn_language;				// 어떤 언어의 OS냐.

	BOOL b_program_exit;			// 프로그램을 빠져나가는건지 확인한다.
	
	// *************************************************************************
	// 원래 위치 복귀 기능 사용 시 사용되는 변수 선언                           
	// *************************************************************************
	int mn_homing_request[MOT_MAXMOTOR];	// 강제 Home Check 요청 플래그 설정 변수 
	double md_cur_cmd_pos[MOT_MAXMOTOR];	// 현재 모터 위치 정보 저장 변수 
	// *************************************************************************

	// *************************************************************************
	// 화면에 대한 핸들 정보 저장 변수 선언                                     
	// *************************************************************************
	HWND hWnd;						// 메인 프레임 핸들 저장 변수
	CWnd *cwnd_title;				// 타이틀 출력 화면 핸들 저장 변수
	CWnd *cwnd_list;				// 리스트 출력 화면 핸들 저장 변수

	CWnd *cwnd_main;				// 메인 화면 핸들 저장 변수
	CWnd *cwnd_basic;				// BASIC 화면 핸들 저장 변수
	CWnd *cwnd_maintenance;			// Maintenance 화면 핸들 저장 변수
	CWnd *cwnd_testreference;		// Test Reference 화면 핸들 저장 변수
	CWnd *cwnd_interface;			// Interface 화면 핸들 저장 변수
	CWnd *cwnd_waittime;			// Wait Time 화면 핸들 저장 변수
	CWnd *cwnd_skip;	//20120525

	CWnd *cwnd_barcode_print;	//20120620

	CWnd *cwnd_rack_info;

	CWnd *cwnd_motorteach;			// motor teaching 화면 핸들 저장 변수
	CWnd *cwnd_motorspeed;			// Motor Speed 화면 핸들 저장 변수
	CWnd *cwnd_diagnose;

	CWnd *cwnd_motor;
	CWnd *cwnd_scrap_info;
	CWnd *cwnd_track_out;
	CWnd *cwnd_add_fail;

	CWnd *cwnd_trayrow_setdlg;		// tray의 row on/off 설정 대화상자 핸들 저장변수

	// *************************************************************************		

	// *************************************************************************
	// 장비 상태 정보 설정 변수 선언                                            
	// *************************************************************************
	int mn_initial;					// 초기화 작업 완료 플래그
	int mn_manual;

	int mn_menu_lock;				// 메뉴 사용 가능 상태 플래그 
	int mn_system_lock;				// SYSTEM LOCK 상태 플래그
	int mn_switch_lock;				// Dialog가 떠 있는 상태 플래그 (Start Switch Lock)

	CString mstr_pass_level;
	int mn_level_maint;				// 메인트 레벨(Level 1) 설정 플래그
	int mn_level_teach;				// 티칭 레벨(Level 2) 설정 플래그
	int mn_level_speed;				// 스피드 레벨(Level 3) 설정 플래그
	int mn_level_admin;				// Admin 레벨(Level 4) 설정 플래그

	int mn_mot_brd_initial;			// 모션 보드 초기화 작업 완료 플래그
	int mn_io_board_initial;

	int mn_home_state[MOT_MAXMOTOR];	// 모터 HOMING 완료 플래그
	int mn_init_state[MOT_MAXMOTOR];		// 초기화 완료 플래그
	int mn_lotend_state[MOT_MAXMOTOR];		// 초기화 완료 플래그
	int mn_initial_error;

	double md_run_speed;			// 장비 동작시 속도비
	double md_manual_speed;			// 메뉴얼 동작시 속도비


	CString mstr_handler_name;		// 핸들러 이름.
	CString mstr_handler_version;	// 핸들러 버젼
	CString mstr_handler_date;		// 핸들러 버젼 수정 날짜.

	CString mstr_last_alarm;
	int mn_status;					// 장비가 가동중이냐
	int mn_reinstatement;

	int mn_safety;

	int mn_board_onoff_dialog;

	CTimeSpan m_tR;		//Run Time
	CTimeSpan m_tJ;		//Jam Time
	CTimeSpan m_tS;		//Stop Time
	CTimeSpan m_tM;		//Maint Time
	CTimeSpan m_tW;		//Waring Time
	CTimeSpan m_tT;		//Test Time
	CTimeSpan m_tPT;	//Prev Test Time
	CTimeSpan m_tIT;	//Index Time
	CTimeSpan m_tPIT;	//Prev Index Time
	CTimeSpan m_tCT;	//Cycle Time
	CTimeSpan m_tPCT;	//Prev Cycle Time
	CTime m_tRef;

	CTimeSpan m_tDR;	//Run Time
	CTimeSpan m_tDRB;	//Run Time
	CTimeSpan m_tDRW;	//Run Time
	CTimeSpan m_tDJ;	//Jam Time
	CTimeSpan m_tDS;	//Stop Time
	CTimeSpan m_tDM;	//Maint Time
	CTimeSpan m_tDW;	//Waring Time
    CTimeSpan m_tRN; //2014,0408

	long lTimeTable[3];
	
	int mn_run_time_check;			// Run Time으로 해야 할것이냐 아니냐.
	int mn_runtime_dll_check;		// 장비 저장용 변수를 한번만 세팅하기 위해서... 2K7/03/15/ViboX

	int mn_motor_init_check;
	int mn_initiailze_end_flag;
	int n_initiailze;

	int mnSerial_Port_Creating[MAX_PORT];

	int mn_tempsetting;				// 온도 세팅중이면 온도를 읽지 말자.

	int mn_machine_pos;				// 장비의 위치. 앞쪽이냐 뒷쪽이냐.
	int mn_temp_set_address;	
	int mn_temp_set_temperature;
	CString mstrTemp[4];

	CString mstrSelectMessage;
	int mnSelectMessage;

	int nScrapInfoView;
	int nTrackOutView;

	int mn_barcode_safety;			// 쉬프터의 위치에 상관없이 바코드 동작을 할것이냐. 초기화 때랑 메뉴얼 동작시에 적용.

	int mn_selfcheck;

	int mn_lot_cancel_mode;
	int mn_lot_label_info;
	int mn_lot_end;

	CString mstr_last_module;
	int mn_last_module_chk[3];
	int mn_last_module_chk_chk[3][2];
	CString mstr_last_module_lotname;

	int mn_last_module_retest_chk;

	CString mstr_front_lot_name;
	CString mstr_lot_name[3];		// 현재 장비 안에 있는 LOT NAME과 BUFFER_READY에서 날라오는 LOT NAME과 비교한다.
	int mn_lot_name_chk;
	int mn_comm_lot_name_chk;
	int mn_lot_shift;

	CString mstr_lot_start_time[3];
	CString mstr_save_lot_name;
	CString mstr_save_lot_start_time;
	int mn_lot_start_time_check[3];

	int mn_remain_count;
	int mn_retest_count;
	int mn_leave_count;

	int mn_back_mode;
	int mn_debug_mode;
	int mn_barcode_mode;
	int mn_reverse_mode;
	int mn_virtual_mode;

	int mn_eject_count;
	int mn_pick_count;

	int mn_blot_count;
	int mn_blot_max;

	int mn_retest_check;			// 리테스트를 받을수 없는 쪽에 LAST MODULE이 갔을 경우에 반대쪽 LANE에서 리테스트를 할수 있게 한다. 2K11/08/15/ViboX
	int mn_retest_check_remain_count;

	CString str_main_lot;
	CString str_main_part;
	CString str_main_last;
	CString str_main_bypass;
	CString str_main_cycle;

	////////////////////////// 20120509 유성준
	CString mstr_bcr_id;	
	CString mstr_bcr_id_1;	
	CString mstr_bcr_id2;	
	CString mstr_bcr_id2_1;	
	CString mstr_bcr_id3;	
	CString mstr_bcr_id3_1;	
	CString mstr_bcr_id4;	
	CString mstr_bcr_id4_1;	
	////////////////////////// 

	////////////////////////// 20120608 유성준
	int mn_vision_yield_yes[1];
	int mn_hs_yield_yes[1];

	////////////////////////// 20120620
	int mn_vision_ml_yes[100];
	int mn_hs_ml_yes[100];
	CString str_vision_ml_yes[100];
	CString str_hs_ml_yes[100];

	////////////////////////// 

	int mn_vision_ml_cnt;	//20120625
	int mn_hs_ml_cnt;	//20120625

	CString str_printer_hs_list;	//20120625
	CString str_printer_hs_list2;	//20120625
	CString str_printer_hs_list3;	//20120625

	int n_uld_convayor;

	int n_nto_fail;
	int n_hto_fail;
	int n_bto_fail;

	//ybs
	int i_checking_load_continue;	// loader부의 lot end진행 여부를 물어 보고 있음을 구분
	int i_chk_load_continue;		// lot 진행또는 LOT END 선택결과를 저장하는 변수

	int i_vision1_init;
	int i_vision2_init;

	BOOL b_main_tray_display;	//20120710


	int n_module_skip;
	int n_fronths_skip;
	int n_rearhs_skip;
	int n_clipbuffer_skip;
	int n_cliprbtpicker_skip;
	int n_clamprbt_skip;
	int n_sorter_skip;
	int n_unload_skip;
	int n_vision_skip;
	int n_workbuff_skip[4];

	int n_motor_init[MOT_MAXMOTOR];

	int n_Work_Num;
	BOOL mb_bcr_read;

	BOOL n_bSendBcr;
	BOOL n_bSendHeatSink;

	int mn_receive;
	int mn_jobComplete;
	int nBootFlagCheck;
	int n_Init_Stacker;

	int n_lotend_ready;
	int n_module_retest[4];//Module,front,back,uld

	int n_more_lefths;
	int n_more_righths;
	int n_more_uld_tray;

	int n_vision_reset;
	int n_hs1_fix;
	int n_hs2_fix;
	int n_hs3_fix;
	int n_hs4_fix;

	int n_plate_fwdbwd_onoff;

	int n_loadClip;
	int n_unloadClip;
	int n_loadCenterChk;
	int n_unloadCenterChk;

	int n_sync_reinstate;	// 복귀동작 진행상황 확인 변수 20121120

} ;
extern  st_handler_param  st_handler;
// *****************************************************************************

#define SPD_HOME		0		// 홈첵 이동 속도
#define SPD_JOG			1		// 조그 이동 속도
#define SPD_N_MOVE		2		// 노말 이동 속도
#define SPD_S_MOVE		3		// 느린 이동 속도
#define SPD_F_MOVE		4		// 빠른 이동 속도

// *****************************************************************************
//  셋팅 관련 변수
// *****************************************************************************
struct st_setting_param
{
	double md_temp_sv[8];		// 온도 세팅 값.
	double md_temp_p[8];		// P값
	double md_temp_i[8];		// i값
	double md_temp_d[8];		// d값

	int mn_text_r[2][2];
	int mn_text_g[2][2];
	int mn_text_b[2][2];
	
	int mn_grid_r[2][2];
	int mn_grid_g[2][2];
	int mn_grid_b[2][2];
};
extern  st_setting_param  st_set;
// *****************************************************************************

// *****************************************************************************
//  장비 부위간 Sync 관련 변수
// *****************************************************************************

enum visionsite_workpos //Y축 기준 
{ 
	SITE_READY_POS = 0,		//vision 밑의 새로운 작업을 위해 대기하는 Y축 및 Z축 대기 위치 
	SITE_LDRBT_POS,	//1		//loading 자재 받는 위치 및 Label 작업을 하는 위치 
	SITE_LABEL_POS,	//2		//label 작업 
	SITE_VISION_POS,//3			//vision 테스트 작업을 하는 위치 
	SITE_ULDRBT_POS,//4			//작업이 끝난 자재를 언로딩하기 위해 작업하는 요청 

	SITE_MAX_POS = 5, //이동하여 작업하는 위치 사이트는 총 5개이다 
};

enum visionrbt_num //Y축 기준 
{ 
	VISION_SITE_CLEAR = -1,
	VISION_1_SITE = 0,
	VISION_2_SITE
};

enum vision_motor_pos_y
{
	VISION_Y_READY_POS	=			0,	//작업이 끝난후 모터 대기 위치 
	VISION_Y_LDRBT_POS,			//1		//ldrbt 작업 위치 
	VISION_Y_LABEL_POS,         //2
	VISION_Y_VISION_POS,        //3
	VISION_Y_ULDRBT_POS,        //4

};

enum vision_motor_pos_z
{	
	VISION_Z_DOWN_READY = 0,//0
	VISION_Z_LDMODULE,//1
	VISION_Z_LABEL,	//2
	VISION_Z_VISION,//3
	VISION_Z_ULDMODULE,//4

	VISION_Z_UP_SAFETY, //5 //사용하지 않는다 	
};

// *****************************************************************************
//  장비 부위간 Sync 관련 변수
// *****************************************************************************
struct st_sync_param
{
///////////////////////////////////////////////////////////////////////////////////////////////////
	//2012.0629 james 추가 
	//#define CTL_REQ					1 //해당 동작을 요청한 상태 
	//#define CTL_READY					2 //해당 동작의 요청에 따라 동작을 완료하여 준비된 상태 - 응대한 상태
	//#define CTL_FREE					3 //요청한 측에서 동작을 모두 완료한 상태로 간섭등을 벗어났을때 상태측이 동작할 수 있도록 자류롭게 데이터를 놓아준다.
	//#define CTL_CHANGE				4 //목표물을 교체를 요청할때 사용한다(Remove Buffer 교체를 요청할때 사용한다)

	//////////////////////////////////////////
	int n_visionrbt_working_site[2]; //[*]:vision1,2 정보=>0:SITE_READY_POS 대기위치(vision밑부분), 1:SITE_LDRBT_POS, 2:SITE_LABEL_POS, 2:vision work, 3:SITE_VISION_POS, 4: SITE_ULDRBT_POS, 
	int n_visionrbt_past_working_site[2]; //바로전 작업한 사이트 위치 정보를 저장한다 
	int n_visionrbt_next_working_site[2]; //[*]:vision1,2 정보=>0:SITE_READY_POS 대기위치(vision밑부분), 1:SITE_LDRBT_POS, 2:SITE_LABEL_POS, 2:vision work, 3:SITE_VISION_POS, 4: SITE_ULDRBT_POS, 

 
	int n_ldrbt_visionrbt_req[2]; //[0]:req(rbt)->ready(vision)->free(rbt place),change(vision)
	int n_visionrbt_label_req[2]; //[0]:vision1, [1]:vision2
	int n_visionrbt_visiontest_req[2]; //[0]:vision1, [1]:vision2
	int n_visionrbt_uldrbt_req[2]; //[0]:vision1, [1]:vision2

    int n_visionrbt_dvc_info[2][2]; //[*][0]:디바이스 유/무 정보, [*][1]:빈 정보

	int n_visionrbt_workend_flag[2]; //각 사이트 작업이 각각 완료 되었는지 확인하는 플레그 

	////2014,0303
	int n_visionrbt_load_flag[2];
	int n_visionrbt_unload_flag[2];
	////

	

///////////////////////////////////////////////////////////////////////////////////////////////////

	int n_transfer_to_elevator;		// Transfer가 Elevator에게 Tray를 공급해 달라는 동기화 메세지.

	int n_robot_to_transfer;		// Robor이 Transfer에게 Tray를 공급해 달라는 동기화 메세지.
	int n_transfer_site_to_robot;	// Transfer가 Robot에게 언로딩할 Site를 알려준다.
	
	int n_transfer_to_airblow;		// Transfer가 Airblow에게 Tray를 넣겠다는 동기화 메세지.
	int n_airblow_to_ejector;		// Airblow가 Ejector쪽으로 Tray를 보내겠다는 동기화 메세지.

	int mn_rbuffer_to_robot[2];		// receive buffer와 robot 간의 동기화.

	int n_airblowdoor_to_transfer;

	int n_length_change;

	int n_scrap_serial;



	int mn_rbuffer_to_barcode[2];	// receive buffer와 barcode 간의 동기화.
	int mn_barcode_to_shifter[2];	// 바코드가 쉬프터 한테 전달하는 동기화 메세지.
	int mn_shifter_to_barcode[2];	// 쉬프터가 바코드 한테 전달하는 동기화 메세지.
	int mn_shifter_to_sbuffer[2];	// 쉬프터가 버퍼 한테 전달하는 동기화 메세지.
	int mn_sbuffer_to_shifter[2];	// send buffer와 shifter 간의 동기화.

	int mn_[2];	// send buffer와 shifter 간의 동기화.

	int mn_pcbox_to_shifter[2][4];	// PCBOX가 쉬프터한테 전달하는 동기화 메세지.
	int mn_retest_check[2];

	int mn_rbuffer_work[2];			// Receive Buffer에 할일이 생겼다.
	int mn_rbuffer_work_id[2];		// Receive Buffer에 할일은?

	int mn_rbuffer_apctinfo[2];		// APCT의 소켓 정보를 물어본다.
	CString mstr_rbuffer_station[2];// APCT의 소켓 정보를 물어본다.
	int mn_rbuffer_station[2];		// APCT의 소켓 정보를 물어본다.
	int mn_rbuffer_apc_id[2];

//	int mn_shifter_to_pcbox_start[2][4];
	int mn_shifter_to_pcbox_start[8];

	CString mstr_buffer_in[2];
	CString mstr_buffer_ready[2];
	CString mstr_retest_req[2];
	CString mstr_new_lot_in;
	CString mstr_scrap_info;
	CString mstr_online_end;
	CString mstr_track_out;
	CString mstr_track_out_msg;
	CString mstr_lot_display;
	CString mstr_delete_scrap;

	int mn_add_fail;

	int mn_rbuffer_shifter_work[2];

	int mn_buffer_work;			// 뒷장비일 경우 한쪽에서 사용중이면 다른 한쪽은 대기 하자.
	int mn_manual_button[8];

	int mn_front_machine_msg;
	int mn_lot_name_send;

	int mn_pos_front;
	int mn_pos_rear;
	bool mb_reverse_mode;

	//Buffer 동작 
	int n_module_ldrbt_to_work[5][4][2]; //[*][0][0]:* 사이트 로봇위치(LD,SINK,CLIP,ULD)	
										//[0][*][0]:* 버퍼순사 버퍼 순서 WORK_BUFFER_1								
	                                    //[0][0][*]: * :0 CTL_REQ 1: CTL_READY

	int n_module_buffer_action_site[5][2]; //사이트 버퍼위치(LD,SINK,CLIP,ULD)
	
	int n_module_site_status[5][4][2];  //[*][0][0]:* 사이트 버퍼위치(LD,SINK12,CLIP,ULD)
										//[0][*][0]:* 버퍼순사 버퍼 순서 WORK_BUFFER_1
	                                    //[0][0][*]: * :0 디바이스 유무 1: Bin
	                               


	//Module 동작
	//[0]:REQ:tray요청, READY:tray 준비완료, FREE:작업완료
	//[1]:tray 종류구분(0:ldtray_1, 1:ldtray_2) -> tray에서 ready를 주면서 해당정보 전달함
	//[2]:bin  
	//[3]:bin  
	int n_module_ldrbt_tray_req[4]; 

	int mn_ld_module_tray_change[2];		// Loader Tray 교체.
	int mn_ld_module_tray_pos;			// 교체할 Tray 위치.
	int mn_ld_modulet_ray_empty_chk;

	int mn_ld_module_tray_supply[2];		// Stocker에 Tray 공급 요청.	여기선 빈트레이를 빼가는걸 의미한다.
	int mn_ld_module_tray_empty_chk;
	int n_ld_module_work_pos;			// 교체할 Tray 위치.



	//Left HS
	int n_lefths_ldrbt_tray_req[4]; 

	int mn_ld_lefths_tray_change[2];
	int n_ld_lefths_tray_supply[2];
	int n_ld_lefths_tray_change;

	//Right HS
	int n_righths_ldrbt_tray_req[4]; 

	int mn_ld_righths_tray_change[2];
	int n_ld_righths_tray_supply[2];
	int n_ld_righths_tray_change;

	//CLIP
	int n_module_ldrbt_clipbuffer_req[4];
	int n_module_clip_req[4];
	int n_module_ldrbt_clamp_req[4];

	//Sorter_Uld
	int n_module_ldrbt_sorter_req[4];

	//Vision
	int n_vision_attach_req[2];
	int n_vision_buffer_req[4][2];  //[0][*]:rbt  [1][*]    [*][1]: site Top Bottom

	int nUpperBuffer_LoadingZone_Priority;
	int nUpperBuffer_LabelingZone_Priority;
	int nUpperBuffer_VisionZone_Priority;
	int nUpperBuffer_UnloadingZone_Priority;
	
	//Unload
	int n_module_uldrbt_tray_req[4]; 
	int mn_ld_module_sortertray_change[2];		// UNloader Tray 교체.
	int n_rejectbin_tray_change_flag;

	int mn_uld_module_sortertray_supply[2];		// Stocker에 Tray 공급 요청.	여기선 빈트레이를 빼가는걸 의미한다.


	//Lot end
	int n_lot_reready[4];//
	int n_module_guide_tray_stacker[2];  //[0] : stacker->movestacf req
	int n_lotend_module_ldstacker_site;
	int n_lotend_module_uldstacker_site;
	int n_lotend_module_tray_transfer;
	int n_lotend_module_ldtray_site; //로딩 트레이 사이트 lot end
	int n_lotend_module_uldtray_site; 
	int n_lotend_module_movingtray_site; 
	int n_lotend_module_ldrbt;

	int n_lefths_guide_tray_stacker[2];  //[0] : stacker->movestacf req
	int n_lotend_lefths_ldtray_site;
	int n_lotend_lefths_tray_transfer;
	int n_lotend_lefths_ldstacker_site;
	int n_lotend_lefths_uldstacker_site;
	int n_lotend_lefths_movingtray_site;
	int n_lotend_lefths_ldrbt;

	int n_righths_guide_tray_stacker[2];  //[0] : stacker->movestacf req
	int n_lotend_righths_ldtray_site;
	int n_lotend_righths_tray_transfer;
	int n_lotend_righths_ldstacker_site;
	int n_lotend_righths_uldstacker_site;
	int n_lotend_righths_movingtray_site;
	int n_lotend_righths_ldrbt;


	int n_ld_clip_tray_supply;
	int n_lotend_clip_tray_transfer;
	int n_lotend_clip_ldtray_site;
	int n_lotend_clip_ldstacker_site;
	int n_lotend_clip_ldrbt;
	int n_lotend_clamp_ldrbt;
	int n_lotend_clipbuffer_ldrbt;
	int n_lotend_clip_Lot;

	//Uld stacker
//	int n_lotend_module_uldsortstacker_site[2];
//	int n_lotend_module_uldsorttray_site; 
//	int n_lotend_module_uldtray_transfer;

	int n_uld_module_guide_tray_stacker[2];  //[0] : stacker->movestacf req
	int n_lotend_module_ldsorterstacker_site;
	int n_lotend_module_uldsorterstacker_site;
	int n_lotend_module_sortertray_transfer;
	int n_lotend_module_ldsortertray_site; //로딩 트레이 사이트 lot end
	int n_lotend_module_uldsortertray_site; 
	int n_lotend_module_uldmovingtray_site;
	int n_lotend_module_sorter_ldrbt;

	BOOL b_lotcancel_use;//20130130

	int n_lotend_modulebuffer_site;

	int n_lotend_sorter_site;
	int n_lotend_unload_site;
	int n_lotend_module_vision[2];

	int i_lot_end_check[50];

	int i_WorkClamp_Clip[4]; //Clamp Clip 동작 Site
	int i_WorkBuffer_Clip[4];

	int i_remove_clip_bin;
	int i_load_clip;
	int i_unload_clip;
	int i_pickup_clip;

	int n_RejectTrayChange;

	int n_WorkBufferPlace[4];

	int n_InsertFlag;

	int n_Start_Stop_flag;

};
extern  st_sync_param  st_sync;
// *****************************************************************************

// *****************************************************************************
//  메세지 관련
// *****************************************************************************
struct st_message_param
{
	CString mstr_infor_msg;
	
	int mn_pad_dialog;
	int mn_dot_use;
	CString mstr_pad_high_limit;
	CString mstr_pad_low_limit;
	CString mstr_keypad_val;

	CString mstr_keypad_msg;

	CString str_fallacy_msg;		// 오류 메세지.
	CString mstr_confirm_msg;		// 확인 메세지
	CString mstr_typing_msg;		// 키보드 메세지

	CString mstr_parent_menu;

	CString mstr_password;

// 	CString mstr_normal_msg;
//	CString mstr_abnormal_msg;
	CString mstr_barcode;

	// List에 출력 하다가 죽는 현상을 막기 위해서... 2K11/09/01/ViboX
	char c_normal_msg[5000];
	char c_abnormal_msg[5000];

	CString mstr_normal_prev_msg;
	CString mstr_abnormal_prev_msg;

	CString mstr_send;
	CString mstr_recive;

	CString mstr_rpy_msg[10];

	int mn_event_msg_list;
	CString mstr_event_msg[3];

	CString str_step_savefile_name;
	CString str_step_savefile_path;

	char n_select_buffer;
} ;
extern  st_message_param  st_msg;
// *****************************************************************************

// ******************************************************************************
// 파일 경로명 저장 구조체 변수 선언                                             
// ******************************************************************************
struct st_path_param 
{
	CString mstr_basic;			// 기본 셋팅 정보 저장 폴더+파일명 설정 변수
	CString mstr_basic_folder;	// 기본 셋팅 정보 저장 폴더+파일명 설정 변수
	CString mstr_datalogger;	// Data Logger의 폴더+파일명 설정 변수
		
	CString mstr_path_dvc;		// 디바이스별 티칭 정보 저장 파일 생성 폴더 설정 변수
	CString mstr_socket_lot;	// 소켓별...
	CString mstr_socket_day;	// 소켓별...
	CString mstr_socket_month;	// 소켓별...
	CString mstr_socket_shift;	// 소켓별...
	CString mstr_path_alarm;	// 출력 알람 정보 존재 폴더 설정 변수
	CString mstr_path_op;		// 오퍼레이터 정보 저장 폴더 설정 변수
	CString mstr_path_tray;		// Tray별 정보 저장 파일 생성 폴더 설정 변수
	CString mstr_path_serial;	// Socket 정보 저장 파일 생성 폴더 설정 변수
	
	CString mstr_path_file;		// 파일 생성 폴더 설정 변수
	CString mstr_path_back;		// BACKUP 파일 생성 폴더 설정 변수
	
	CString mstr_path_back_1;	//20120701

	CString mstr_file_basic;	// 기본 셋팅 정보 저장 파일명 설정 변수
	CString mstr_file_wait;		// 대기 시간 정보 저장 파일명 설정 변수
	
	CString mstr_operation;		// Operation Log 파일 폴더 설정.
	CString mstr_interface;		// Interface Log 파일 폴더 설정.
	CString mstr_machine;		// Machine Log 파일 폴더 설정.
	CString mstr_barcode;		// Barcode Log 파일 폴더 설정.
	CString mstr_vision;        // vision Log 파일 폴더 설정.//2014,0303
	CString mstr_gpib;			// Total Log 파일 폴더 설정.
	CString mstr_total;			// Total Log 파일 폴더 설정.
	CString mstr_galmg;
	CString str_log_path;
	CString str_lot_data_path;
	
	CString mstr_Excel;//2014,0408

	CString mstr_io_map;		// IO MAP 파일 위치 폴더.
	CString mstr_motor;
	
	CString mstr_handler;

	CString mstr_io_part_map;
	CString mstr_motor_part_map;
	CString mstr_motor_axis_map;
	CString mstr_wait_time_map;

	CString mstr_interface_folder;	//20120516
	CString mstr_label_folder;	//20120530

	CString mstr_part_folder;   //20121220
	CString mstr_part;          //20121220

	//2015.0129
	CString mstr_uph;
	CString mstr_uph_total;

} ;
extern  st_path_param  st_path;
// ******************************************************************************

// ******************************************************************************
// I/O 동작 대기 시간 및 리미트 시간 저장 구조체 선언                            
// ******************************************************************************
struct st_time_param 
{
	// Module Eject Wait Time
	int mn_module_eject_w[2];
	int mn_module_eject_l;

	// Barcode Up/Down Wait Time
	int mn_barcode_updn_w[2];
	int mn_barcode_updn_l;

	// Hifix Guide In/Out Wait Time
	int mn_hifix_guide_inout_w[2];
	int mn_hifix_guide_inout_l;

	int mn_barcode_stabilization_w;

	int mn_module_press_w[2];

	int mn_network_msg_l[2];

	int n_wait_time[MAX_WAIT_TIME_NUM][2];
	int n_limit_time[MAX_WAIT_TIME_NUM];
} ;
extern  st_time_param  st_time;
// ******************************************************************************

// *****************************************************************************
//  Tower Lamp 화면에 대한 정보 저장 구조체 변수 선언                           
// *****************************************************************************
struct st_lamp_param
{
	int mn_lamp_r[8];			// RED 램프 상태 저장 변수
	int mn_lamp_y[8];			// YELLOW 램프 상태 저장 변수
	int mn_lamp_g[8];			// GREEN 램프 상태 저장 변수
	
	int mn_interest;			// BLUE 램프 상태 저장 변수
	
	CString mstr_equip_no;		// 장비 호기 저장 변수
	CString mstr_equip_id;	// 장비 코드 저장 변수
	
	int mn_buzzer_mode;			// 부저 사용 모드 저장 변수
	int mn_front_mode;
	int mn_rear_mode;

	
	int mn_lamp_wait_time;		// 램프 FLACKER 시간 저장 변수
	int mn_lamp_change_time;	// 램프 변경 시간 저장 변수
} ;
extern  st_lamp_param  st_lamp;
// *****************************************************************************

// ******************************************************************************
// 베이직 화면 환경 설정 정보 저장 구조체 선언                                   
// ******************************************************************************
struct st_basic_param 
{
	int nManualSpeed;
	int nRunSpeed;
	//////////////////////////////// 20120523
	int nJogSpeed;
	int nBarcodeSpeed;
	//////////////////////////////// 

	int n_tray_size;			// Tray Size가 크냐 작냐 결정. SODIMM은 크고, RDIMM은 작다. 이름보고 판단할수도 있음.
	int n_tray_x;
	int n_tray_y;

	int n_mode_device;
	int n_mode_run;
	int n_retry_cnt;
	int n_count_pick_retry;

	int mn_mode_run;			// 장비 동작 모드 저장 변수		(1:ON LINE	0:OFF LINE)
	int mn_mode_device;
	
	int mn_bypass_mode;

	int mn_mlct_mode;

	int mn_device_type;
	
	int mn_count_retest;		// Retest 횟수 저장 변수
	int n_day_start_time;		// Data 기록용.. 하루의 시작 시간...
	int n_day_shift_time;		// Data 기록용.. Shift 시간...
			
	CString mstr_device_name;	// 선택된 디바이스 종류 저장 변수
///////////////////////////////// 20120530
	CString mstr_label_name;	// 선택된 디바이스 종류 저장 변수
	int n_mode_label;
	int mn_mode_label;
	int mn_label_type;

/////////////////////////////////	
	int mn_alarm_delay_time;
	
	int mn_electrostatic_check_gap;
	double md_electrostatic_value;

	int mn_bd_live_check_gap;

	int mn_nto_time;				// NO VIDEO 대기 시간
	int mn_bto_time;				// BOOTING 대기 시간
	int mn_hto_time;				// HOLD 대기 시간
	int mn_hto_max_time;

	int mn_press_count;
	int mn_board_count_max;

	int mn_network_wait_time;		// Network 대기 시간
	int mn_network_retry_cnt;		// Network Retry 횟수

	int mn_Label_Print_Error_Cnt;
	int mn_Dvc_Repick_Cnt;//20120731

	//////////////////////////////// 20120522
	int mn_lotset_module_load;
	int mn_lotset_hs_left;
	int mn_lotset_hs_right;	
	int mn_lotset_clip;
	int mn_lotset_unloader;//20120707
	int mn_lotset_reject;//20210707

	///////////////////////////////////////////// 20120601
	int mn_labelset_sd;
	int	mn_labelset_lt;
	int	mn_labelset_lh1;
	int	mn_labelset_lh2;

	/////////////////////////// 20120906
	double	md_labelset_x1_pos;
	double	md_labelset_x2_pos;
	double	md_labelset_x3_pos;
	double	md_labelset_x4_pos;
	double	md_labelset_x5_pos;
	double	md_labelset_x6_pos;
	double	md_labelset_y1_pos;
	double	md_labelset_y2_pos;
	double	md_labelset_y3_pos;
	double	md_labelset_y4_pos;
	double	md_labelset_y5_pos;
	double	md_labelset_y6_pos;
	double	md_labelset_x1_width;
	double	md_labelset_x2_width;
	double	md_labelset_x3_width;
	double	md_labelset_x4_width;
	double	md_labelset_y1_height;
	double	md_labelset_y2_height;
	double	md_labelset_y3_height;
	double	md_labelset_y4_height;
	///////////////////////////


	int mn_labelset_func_paper;	//20120625
	int mn_label_bin_print;	//20140224

	int	mn_label_vision_paper;


	double md_picker_gap_module;
	double md_picker_gap_hs_left;
	double md_picker_gap_hs_right;


	CString mstr_ftp_path;
		
	long l_handler_recovery_flag[4];
	//[0]=>복구모드 저장 모드(0:저장되지 않은 상태, 1:복구모드가 저장 상태(이때만 초기화시 복구여부를 물어볼 수  있다) 
	//[1]=>초기화시 복구모드 선택 유/무 확인 (0:복구모드 선택안함, 1:복구모드 선택)
	//[2]=>복구 모드 저장 시간 저장 
	//[3]=>초기화시 복구모드 선택시간  

	BOOL mb_controlpanel_enable;

	// TEMP - COOL & HOT
	int mn_cool_max;
	int mn_hot_min;

	int mn_data_change_list;	//20120608
	int mn_vis_pos;
	int mn_pick_label;
	int n_mode_use_bin;
	int n_mode_use_door;
	int n_mode_use_sorter_picker;
	int n_mode_clip_alarm;
	int n_mode_cap_remove; // 20140220  0 : 스킵  1 : 사용 
	
	CString mstr_barcode_data[4];	//20120628
	int n_mode_m_direction;
	int n_mode_hs_direction;
	int n_mode_bcr;
	BOOL n_mode_module_clip_swing;

	int n_clamp_clip_bin;
	int n_clip_bin;
	int n_curr_clamp_clip_bin;
	int n_curr_clip_bin;

///////////////////////////////// 20121220 
	CString mstr_part_number[100];   // 선택된 디바이스 종류 저장 변수
	CString mstr_part_number_Check[100];
	CString mstr_mode_part_Check[100];
	int mn_mode_part;

/////////////////////////////////	

	////2014.0818
	int n_ld_node;
	int n_hs_front_node;
	int n_hs_back_node;
	int n_clip_node;
	int n_uld_node;
	////

	//2015.0129
	int n_mode_use_vis_cont_err;
	int n_vis_cont_err;
	int n_vis_tot_err;

};
extern  st_basic_param  st_basic;
// ******************************************************************************

// ******************************************************************************
// 발생한 알람 관련 데이터 저장 구조체 선언                                      
// ******************************************************************************
struct st_alarm_param 
{
	CStringList mstr_list_jam;	// 발생한 Jam Code의 리스트
	CStringList mstr_list_jam_time;	//alarm 발생시간 저장 리스트
	CString mstr_code;			// 발생한 알람 코드 저장 변수
	CString str_lastcode;			// 발생한 알람 코드 저장 변수
	char mc_code[10];
	CString mstr_pcode;			// 발생한 알람 코드 저장 변수
	int mn_count_mode;			// 알람 카운트 플래그 (0:알람 카운트 미진행    1:알람 카운트 진행)
	int mn_type_mode;			// 알람 종류 저장 변수 (0:경고성    1:진행성    2:장비 정지)
	
	long stl_cur_alarm_time;	// 알람 발생 시간 저장 변수
	int stl_prev_code;			// 바로 전 발생한 알람 코드 저장 변수
	long stl_prev_alarm_time;	// 바로 전 발생한 알람 발생 시간 저장 변수
	long stl_alarm_start_time;	// 시작 버튼이 눌렸을때의 발생 시간 저장 변수
	
	int mn_alarm_occured;		// 알람 발생 횟수 저장 변수
	int mn_alarm_occured_day;	// 알람 발생 횟수 저장 변수

	int mn_reset_status;		// Alarm Reset Status
	int mn_barcode_read;		// Barcode Read시 발생한 에러인지 확인한다. 2K4/11/25/ViboX

	int mn_alarm_assign_section;
	int mn_picking_error;
	int n_rejecttrayfull;
	int n_area_alarm;
	int n_area_ready_alarm[2]; //[0] : stacker [1] Unloader
	int n_emo_alarm;
} ;
extern  st_alarm_param  alarm;
// ******************************************************************************

// ******************************************************************************
// 알람 데이터 저장 구조체 선언                                                  
// ******************************************************************************
struct st_dsp_alarm_param 
{
	// **************************************************************************
	// 알람 부위 표시 마크 [출력 위치], [크기], [칼라] 저장 변수                 
	// **************************************************************************
	int mn_Offset;
	int mn_Cy;
	int mn_Cx;
	int mn_LineWidth;
	int mn_R;
	int mn_G;
	int mn_B;
	// **************************************************************************
	
	// **************************************************************************
	// 화면에 출력할 알람 정보 저장 변수                                         
	// **************************************************************************
	int mn_action_cnt;            // 출력할 알람 조치 사항 갯수 저장 변수
	CString mstr_style;           // 알람 출력 형태 저장 변수
	CString mstr_bmp_file;        // 알람 표시 그림 파일 저장 변수
	CString mstr_repair_msg;      // 알람 조치 사항 저장 변수
	// **************************************************************************
	
	// **************************************************************************
	// 파일에 저장된 알람 정보 저장 변수                                         
	// **************************************************************************
	CString mstr_e_content[2000];  // 파일에서 로딩한 한 라인 정보 저장 변수
	CString mstr_e_code[2000];     // 알람 코드 저장 변수
	CString mstr_e_state[2000];    // 알람 상태 저장 변수
	CString mstr_e_part[2000];     // 알람 발생 부위 저장 변수
	CString mstr_e_msg[2000];      // 공백 제거 알람 메시지 저장 변수
	// **************************************************************************
	
	int mn_e_count[2000];
	int mn_e_list;
	
	// **************************************************************************
	// 현재 발생한 알람 저장 변수                                                
	// **************************************************************************
	int mn_cur_state;				// 알람 저장 여부 플래그
	CString mstr_cur_state;			// 알람 등급
	CString mstr_cur_msg;			// 발생한 알람 메시지 저장 변수
	// **************************************************************************

	int n_picking_error;
} ;
extern  st_dsp_alarm_param  st_alarm;
// ******************************************************************************

// ******************************************************************************
// 장비 동작 제어 플래그 정보 저장 구조체 선언                                   
// ******************************************************************************
struct st_work_param 
{
	// **************************************************************************
	int mn_run_status;  // 장비 동작 상태 정보 저장 변수 (O:STOP   1:RUN    2:ALARM    3:MAINT    4:INIT)
	// **************************************************************************

	int n_total_load_count;
	int n_pass_count;
	int n_fail_count;
	float n_fyiled;	
	int n_load_count[2];

	int nFirstPicker_num[8];
	int nFirst_X_num[8];
	int nFirst_Y_num[8];

	int nRetestReq;
	int nRetestCnt;
	int nRetestReqMode;
								//      0,   1    0,     1
	int nMdlInputCount[2][2];	// [Prime, Cum][Lot, Daily]
	int nMdlPassCount[2][2];
	int nMdlFailCount[2][2];
	int nMdlHtoCount[2][2];
	int nMdlBtoCount[2][2];
	int nMdlNtoCount[2][2];
	int nMdlRejectCount[2][2];
	int nMdlScrapTotal[2][2]; // 20130121
	float fMdlyield[2][2];
	

	////2014,0305
	int nMdlInputTotalCount;
	int nMdlPassTotalCount;
	int nMdlRejectTotalCount;
	////

	CStringList strMdlReject;
	CStringList strMdlRejectCode;

	CStringList strMdlRejectRouterSorter;
	CStringList strMdlRejectCodeRouterSorter;

	CStringList strMdlRejectFCT;
	CStringList strMdlRejectCodeFCT;

	CStringList strMdlRejectPCTSorter;
	CStringList strMdlRejectCodePCTSorter;

	CStringList strMdlRejectAdd;
	CStringList strMdlRejectAddCode;

	CStringList strlabelFormat;
	CStringList strlabelFormatCode;

	CStringList strsamplelabelFormat;
	CStringList strsamplelabelFormatCode;

	int mn_daily_fail_count;

	long l_cycle_time[3];
	CString str_cycle_time;

	// **************************************************************************
	// 장비 동작 중 발생하는 시간 정보 저장 관련 멤버 변수                       
	// **************************************************************************
	long MachineTime[4];  // 장비 동작 및 정지 시간 정보 저장 변수 (0:STOP TIME    1:RUN TIME    2:ALARM TIME    3:MAINT TIME)
	// **************************************************************************

	int mn_elec_serial_work;
	int mnFtpWork;
	int mn_bin_cnt[3][BINNUM];					// BIN별 테스트 완료 자재 갯수 저장 변수
	int n_data_save_time;						// Data 저장 시간
	// **************************************************************************

	////복귀동작 관리  // 0:Front Shifter, 1:Rear Shifter, 2:Front Buffer, 3:Read Buffer
	// 0:복귀클리&완료 , 1:stop전환후//백업후 , 2:스타트 키 정보 저장, 3:복귀중 ...., 복귀완료후 0 그리고 런 하자 
	int mn_reinstatement_mode[10];
	int mn_reinstatement_Ysafety[10];
	int mn_reinstatement_Buffsafety[4];
	int mn_reinstatement_BuffClampsafety[4];
	double md_reinstatement_pos[MOT_MAXMOTOR];
	int mn_PickerUpDnStatusBackup[10][2];
	int mn_FingerStatusBackup[10][2];
	int mn_DviceStatusBackup[10][2];
	int mn_TrayClampStatusBackup[10][2];
	int mn_ClipBufferStatusBackup[2];
	int mn_ClipInsertStatusBackup[2];
	int mn_ModuleClampStatusBackup[4];
	int mn_HsClampStatusBackup[4];
	int mn_BufVacuumStatusBackup[4];
	int mn_EjectStatusBackup[4];
	int mn_VisClampStatusBackup[2][2]; //[2][*] [2] : 사이트 
	int mn_VisRotateStatusBackup[2][2];
	double md_reinstatement_Pos[2][MOT_MAXMOTOR];

	int mn_VisVlpClampStatusBackup[2][2];//20121205
	int mn_VisAttVacuumStatusBackup[2][2];//20121205
	int mn_VisAttUnitFwdStatusBackup[2][2];//20121205
	int mn_VisAttUnitBwdStatusBackup[2][2];//20121205
	int mn_VisAttEjectStatusBackup[2][2];//20121205
	
	int mn_back_type[7];
	int mn_back_site[7];

	int n_back_site;		//이동했던 위치
	int n_back_type;		//이동방식이 계산을 통한 위치인지, teach데이터로 이동인지
	int n_back_picker;		//이동했던 로봇의 피커지정
	int n_back_x_pos;		//이동했던 위치의 row
	int n_back_y_pos;		//이동했던 위치의 cols
	int n_back_z_movepos;	//이동했던 위치에서 z axis가 down인지 up동작인지 구분

	double d_reinstatement_x_pos;		//복귀될 계산된 x 위치값
	double d_reinstatement_y_pos;		//복귀될 계산된 y 위치값
	double d_reinstatement_z_pos;		//복귀될 계산된 z 위치값

	double mdReadElectData[5];

	long ml_test_index[3];
	int mn_test_index;

	BOOL	bdiagnose_emg;				// 자가 진단 비상 정지...	

	int mn_tl_status;					// 타워램프 상태 정보 저장 변수 (O:STOP 1:RUN(IN PCB) 2:ALARM 3:MAINT 4:INIT 5:RUN(NO PCB))

	CString mstr_part_number;
	CString mstr_lot_number;
	CString mstr_lot_count;

	int mn_client_accept[3];
	int	mn_client_ready[3];
	int mn_client_rpy[3];
	CString mstr_client_rev_msg[3];
	CString mstr_client_send_msg[3];
	CString mstr_client_send_bpc;

	CString mstr_bcr_readdata;

	int mn_shifter_work_site[2];		// 쉬프터가 작업을 할 위치를 ++ 시켜가면서 찾는다.
	int mn_shifter_prev_work_site[2];

	int mn_client_socket;

	CString mstr_delete_lot_no;
	CString mstr_delete_serial;
	CString mstr_lot_no;
	CString mstr_part_no;
	CString mstr_op_id;
	CString mstr_pcbox_id;
	CString mstr_e_pcbox_id;

	CString mstr_pcbox_cnt;
	CString mstr_pcbox_info;

	CString mstr_step_no;
	CString mstr_lot_no_check;
	CString mstr_step_no_check;
	CString mstr_lot_qty_check;
	CString mstr_cycle_id_check;
	CString mstr_part_id_check;
	CString mstr_oper_cnt_check;
	CString mstr_oper_info_check;
	CString mstr_format_retry;
	CString mstr_pcbox_cnt_check;
	CString mstr_mode;
	CString mstr_bin_info;
	CString mstr_scrap_cnt;
	CString mstr_scrap_info;

	CString mstr_station;
	CString mstr_last_module;
	CString mstr_module_cnt;
	CString mstr_module_data[20];
	CString mstr_value;
	CString mstr_user_id;
	CString mstr_status;
	CString	mstr_desc;

	int mn_PCBOX_Status[160];			// 150장의 PCBOX의 상태를 저장한다.
	CString mstr_PCBOX_Result[160];		// 150장의 PCBOX 결과를 저장한다.

	int		mn_prev_step[100];			// 이전 step (변경 되었는지 보기 위함)
	CString	mstr_history_step[100];		// step_history 약 50개에 해당하는 step을 저장
	CString	mstr_history_step_back_up[100];		// step_history 약 50개에 해당하는 step을 저장
	CString mstr_last_change_date[100];	// 최종 step으로 변경되는 시점의 시간

	int		m_iFailCount_OnlineEnd;
	int		m_iFailCount_TrackIn;

	long ml_last_shifter_move_time;

	int n_sorter_retest;
	int n_lotdisply_qty;
	int n_scrap_info_fail_qty;
	int n_label_info_fail_qty;
	
	////////////////////////////// 20120521
	int		n_text_r[2][2];
	int		n_text_g[2][2];
	int		n_text_b[2][2];

	int		n_grid_r[2][2];
	int		n_grid_g[2][2];
	int		n_grid_b[2][2];

	CString		str_barcode_tag;

	////////////////////////////// 20120608
	CString str_vision_msg;
	int      n_bcrNojob;//바코드 job 번호
	int      n_hsNojob;//비전 job 번호 //20120831

	//////////////////////////////

	int n_reinstate_flag[MOT_MAXMOTOR];			//모터별 복귀동작 상태 저장 0:복귀를 해야한다, 1:복귀를 했다
	double d_reinstate_pos[2][MOT_MAXMOTOR];		//0:모터 이전위치, 1:현재위치
	int n_reinstate_ldm_picker[2][2];				//[0][.]:1번피커 [1][.]:2번피커
	int n_reinstate_ldm_dvcpicker[2][2];
	int n_reinstate_hsleft_picker[2][2];//20121126				//[0][.]:1번피커 [1][.]:2번피커
	int n_reinstate_hsleft_dvcpicker[2][2];//20121126
	int n_reinstate_hsright_picker[2][2];//20121126				//[0][.]:1번피커 [1][.]:2번피커
	int n_reinstate_hsright_dvcpicker[2][2];//20121126

	int mn_lot_start;


	int mn_module_tray[2];
	int i_FirstPickerNum[10];

	int i_robot_device_loading;

	double n_PickerPitch[4];
	int i_sort_good_cnt;
	int i_sort_reject_cnt;

	////////////////////////////////////////// 20120704
	int i_tray_ld_status[TRAY_LD_COL];		//Elevator Status
	int i_tray_hs_left_status[TRAY_HS_LEFT_COL];		//Elevator Status
	int i_tray_hs_right_status[TRAY_HS_RIGHT_COL];		//Elevator Status
	int i_tray_clip1_status[TRAY_CLIP1_COL];		//Elevator Status
	int i_tray_clip2_status[TRAY_CLIP2_COL];		//Elevator Status
	int i_tray_uld_status[TRAY_ULD_COL];		//Elevator Status
	int i_tray_reject_status[TRAY_REJECT_COL];		//Elevator Status
	//////////////////////////////////////////

	int n_loadlot_count[4];  //LDMODULE_SITE,LEFTSINK_SITE,RIGHTSINK_SITE,LDCLIP_SITE
	int n_lot_count;
	int n_slash_lot_count;

	int n_lotend;//LD Module

	int n_eqp_lotend;
	int n_multilot_start;

	////////////////// 20120711
	CTime t_jamcode_start;
	CTime t_jamcode_end;
	CString str_worker_name;
	CString str_worker_crem;

	int				mndb;	
	COleDateTime	date;	
	CString			m_strlot_no;
	CString			m_strpart_no;
	
	int				n_Time_Section;
	int				n_Time_Time;
	int				n_Mode;	
	int				n_SectionBuffer;	//////////////////

	int				n_jamcode_flag;

	///////////////////////////////// 20121126
	int mn_LdModuleRobot_Reinstatement_Ok;
	int mn_LdModuleStacker_Reinstatement_Ok;
	int mn_LeftHsRobot_Reinstatement_Ok;
	int mn_LeftHsStacker_Reinstatement_Ok;
	int mn_RightHsRobot_Reinstatement_Ok;
	int mn_RightHsRobotStacker_Reinstatement_Ok;
	int mn_ClipRobot_Reinstatement_Ok;
	int mn_SorterRobot_Reinstatement_Ok;
	int mn_UnloadRobot_Reinstatement_Ok;
	int mn_UnloadStacker_Reinstatement_Ok;
	int mn_VisionRobot_Reinstatement_Ok;
	int mn_WorkBufferRobot_Reinstatement_Ok;

	int mn_LdModuleRobot_Reinstatement_Ready;
	int mn_LdModuleStacker_Reinstatement_Ready;
	int mn_LeftHsRobot_Reinstatement_Ready;
	int mn_LeftHsStacker_Reinstatement_Ready;
	int mn_RightHsRobot_Reinstatement_Ready;
	int mn_RightHsRobotStacker_Reinstatement_Ready;
	int mn_ClipRobot_Reinstatement_Ready;
	int mn_SorterRobot_Reinstatement_Ready;
	int mn_UnloadRobot_Reinstatement_Ready;
	int mn_UnloadStacker_Reinstatement_Ready;
	int mn_VisionRobot_Reinstatement_Ready;
	int mn_WorkBufferRobot_Reinstatement_Ready;

	//////////////////////////////////
	BOOL b_Reinstatement[10];

	int n_NodeCnt;

	BOOL b_run_slmulation_md;

	int mn_stacker_num;

	BOOL b_Manual_Label_Print;
	int mn_Barcode_OutPrint;//20140214

	CString m_strBarcode_OutPirnt[500];
	CString m_strLotNoChk[500];//2014,0218

	int Alarm_Cnt;

	////2014,0303
	CString mstr_vision_lotno;
	CString mstr_Vision_Read_LotNo[500];
	////

	////2014,0406
	BOOL b_Auto_Model_change;
	////
	int m_nAlarmCnt;

	////2014,0408
	CString mstr_Run_time;
	CString mstr_Start_time;
	CString mstr_End_time;
	CString mstr_Excel_Date[1000];
	CString mstr_Excel_Model[1000];
	CString mstr_Excel_LotNo[1000];
	
	int     m_nExcel_Input[1000];
	int     m_nExcel_Pass[1000];
	int     m_nExcel_Fail[1000];
	int     m_nExcel_Cnt;
	
	CString mstr_Excel_StartTime[1000];
	CString mstr_Excel_EndTime[1000];
	CString mstr_Excel_RunTime[1000];
	////

	////2014,0411
	BOOL b_Barcode_Create_Btn_Use;
	////

	int m_nSorterCurRetry;//2014,1210
	DWORD m_dLotStartTime;
	long mn_output_dvc;

	int m_nMdlTotal;
	int m_nCboSelModel;
	CString m_strModelName[100];
	CString m_strCurModel;
	int m_nNmModel;//ahepfaud
	CStringList m_strModel[50][200];//[m_nNmModel][Partnum]

} ;
extern  st_work_param  st_work;

struct st_picker_param 
{
	int n_module_ldrbt_enable[PICKER_PARA];
	int n_heat_leftldrbt_enable[PICKER_PARA];
	int n_heat_rightldrbt_enable[PICKER_PARA];
	int n_heat_clipldrbt_enable[LDULD_PARA];
	int n_picker_ldrbt_enable[LDULD_PARA];
	int n_ldrbt[DATA_INFO][LDULD_PARA]; //[PICKER_PARA]
	int n_uldrbt[DATA_INFO][LDULD_PARA]; //사이즈 확장 [PICKER_PARA];	

	int n_label_enable;	//20120625
	int n_vision_eable[2];	//[0]: Label Vis [1]: Bcr Vis

	int n_work_buffer_enable[4];	//20120625


};
extern  st_picker_param  st_picker;

// ======================================================================
//  TRAY & BUFFER 설정 정보 저장 구조체 선언
// ======================================================================
struct st_traybuffer_param 
{
	int i_loader_row;
	int i_loader_col;

	int i_unloader_row;
	int i_unloader_col;

	int i_reject_row;
	int i_reject_col;

	int i_buf_left_row;
	int i_buf_right_row;
} ;
extern  st_traybuffer_param  st_traybuffer[MAX_SITE_INFO];
// ======================================================================


////2014,1210
struct st_thread_Run_Rbt_Load_Clip_param 
{
	int PrevStep;
	int RunInsertStep;
	int RunClampStep;
	int MoveStep;
	int InitStep;
	int RotatorStep;
	int RunBufferStep;
	int ClipBufferStep;
	int ClipPickStep;
	int ClipPlaceStep;
	int ClampRbtStep;
	int ReinstateStep;
	int mn_NextPos;
	int InsertStep;
	int DropStep;
	int LdClipStep;
	int ClipLotStep;
	int PickClipStep;
	int UldClipStep;
	
	int mn_retry;
	int mn_Clipretry;
	int mn_pre_move_flag;
	int mn_pre_insert_flag;
	int mn_pre_clamp_flag;
	
	int ActionPicker[D_INFOSIZE];
	int mn_Result;
	long l_PickerWaitTime[3];
	double md_ClipAbsPos;
	long l_UntilWaitTime[3];
	double md_CurrentPos;
	long l_ClipWaitTime[3];
	double md_clampPos;
	
	long l_ReinStateWaitTime[3];
	
	int HeadVal;
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	double dClipCurPos[10];//20121126
	
	int Clip_remove_Cnt;//2014,0228
} ;
extern  st_thread_Run_Rbt_Load_Clip_param  th_reco_Run_Rbt_Load_Clip;

struct st_thread_Run_Rbt_Load_LeftHeat_param 
{
	int RunStep;
	int MoveStep;
	int InitStep;
	int ReinstateStep;
	int SafetyZone_Step;
	int Tray_Pick_Step;
	int Module_Place_Step;
	int Before_Module_Place_Step;
	int mn_posok;
	int TrayPlaceStep;
	
	int mn_pre_move_flag;
	int mn_Pickerenable_cnt;
	int mn_FirstPicker_Y_Pos;
	int mn_FirstTray_Y_Pos;
	int mn_NextPos;
	
	int ActionPicker[D_INFOSIZE][PICKER_NUM];
	int mTempPicker[D_INFOSIZE][PICKER_NUM];
	int FingerInfo[D_INFOSIZE][PICKER_NUM];
	long l_PickerWaitTime[PICKER_NUM][3];
	long l_FingerWaitTime[PICKER_NUM][3];
	int mCurrent_RowPos[10];
	int mn_pickerenable_cnt;
	
	int mn_FirstTray_X_Pos;
	int mn_FirstPicker_X_Pos;
	int i_DVCRepickCnt;
	int mResult[PICKER_NUM];
	char mc_alarmcode[10];
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	int mn_Clipretry;//20121126
	double dHSLeftCurPos[10];//20121126
	
	long l_until_wait_time[3];
	long l_ReinStateWaitTime[3];
	
	double md_AbsPos;
	double md_SafetyPos;
	double md_CurrentPos;
	double md_Robot_PlacePos;
	double md_Robot_Pos;
	double md_Robot_TrayPlacePos;
	
	int HeadVal;	
} ;
extern  st_thread_Run_Rbt_Load_LeftHeat_param  th_reco_Run_Rbt_Load_LeftHeat;

struct st_thread_Run_Rbt_Load_Module_param 
{
	int mn_posok;
	int PrevStep;
	int RunStep;
	int MoveStep;
	int InitStep;
	int SafetyZone_Step;
	int ReinstateStep;
	int TrayBackStep;
	int TrayMoveStep;
	int TrayPlaceStep;
	int Tray_Pick_Step;
	int Module_Place_Step;
	int Before_Module_Place_Step;
	int mn_pre_move_flag;
	int mn_pickerenable_cnt;
	int mn_FirstPicker_Y_Pos;
	int mn_FirstTray_X_Pos;
	int mn_FirstTray_Y_Pos;
	int mCurrent_RowPos[10];
	int mn_FirstPicker_X_Pos;
	int i_PartialPickCnt;
	
	double md_CurrentPos;
	int i_DVCRepickCnt;
	int mn_NextPos;
	
	int MDHeadVal;
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	double dLDModuleCurPos[10];//20121126
	int mn_Clipretry;//20121126
	
	int mMultiAxis[2];
	double md_AbsPos[2];
	double md_Velocity[2];
	double md_Robot_Pos;
	double md_Robot_TrayPlacePos;
	double md_Robot_PlacePos;
	double md_SafetyPos;
	
	int n_pickeroff_bufferoff_flag[PICKER_NUM];
	int ActionPicker[D_INFOSIZE][PICKER_NUM];
	int FingerInfo[D_INFOSIZE][PICKER_NUM];
	int mTempPicker[D_INFOSIZE][PICKER_NUM];
	long l_PickerWaitTime[PICKER_NUM][3];
	long l_FingerWaitTime[PICKER_NUM][3];
	int mResult[PICKER_NUM];
	int mn_PickLoad;
	
	long l_until_wait_time[3];
	long l_ReinStateWaitTime[3];
	
	int HeadVal;
	
} ;
extern  st_thread_Run_Rbt_Load_Module_param  th_reco_Run_Rbt_Load_Module;

struct st_thread_Run_Rbt_Load_RightHeat_param 
{
	int RunStep;
	int MoveStep;
	int InitStep;
	int ReinstateStep;
	int SafetyZone_Step;
	int Tray_Pick_Step;
	int Module_Place_Step;
	int Before_Module_Place_Step;
	int TrayPlaceStep;
	
	int mn_pre_move_flag;
	int mn_Pickerenable_cnt;
	int mn_FirstPicker_Y_Pos;
	int mn_FirstTray_Y_Pos;
	
	double md_AbsPos;
	double md_SafetyPos;
	double md_CurrentPos;
	double md_Robot_PlacePos;
	double md_Robot_Pos;
	double md_Robot_TrayPlacePos;
	int mn_NextPos;
	
	int mn_FirstTray_X_Pos;
	int mn_FirstPicker_X_Pos;
	int i_DVCRepickCnt;
	int mResult[PICKER_NUM];
	
	int RHSHeadVal;
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	int mn_Clipretry;//20121126
	double dHSRightCurPos[10];//20121126
	
	long l_until_wait_time[3];
	int mCurrent_RowPos[10];
	int mn_pickerenable_cnt;
	
	long l_ReinStateWaitTime[3];
	
	int HeadVal;
	
	int mTempPicker[D_INFOSIZE][PICKER_NUM];
	int ActionPicker[D_INFOSIZE][PICKER_NUM];
	int FingerInfo[D_INFOSIZE][PICKER_NUM];
	long l_PickerWaitTime[PICKER_NUM][3];
	long l_FingerWaitTime[PICKER_NUM][3];
	
} ;
extern  st_thread_Run_Rbt_Load_RightHeat_param  th_reco_Run_Rbt_Load_RightHeat;

struct st_thread_Run_Rbt_Sorter_param 
{
	int PrevStep;
	int RunSortStep;
	int RunUldStep;
	int MoveStep;
	int InitStep;
	int SafetyZone_Step;
	int UldSafetyZone_Step;
	int ReinstateStep;
	int ReinstateStep1;//20121126
	int Module_Pick_Step;
	int TrayPlace_Step;
	int UldModule_Pick_Step;
	int Module_Place_Step;
	int mn_StackerPos;
	int mn_NextPos;
	
	//	int mn_FirstPicker_X_Pos;
	int mn_retry;
	int mn_uldretry;
	int i_DVCSortRepickCnt;
	int i_DVCSortReplaceCnt;
	int i_DVCUldRepickCnt;
	int i_DVCUldReplaceCnt;
	int mResult[LDULD_PARA];
	int mUldResult[LDULD_PARA];
	int ActionPicker[2][D_INFOSIZE];
	int TempPicker[D_INFOSIZE];
	int mTempPicker[D_INFOSIZE];
	int UldTempPicker[D_INFOSIZE];
	int mUldTempPicker[D_INFOSIZE];
	long l_PickerWaitTime[2][3]; //[0][] : sorter [1][] : unloader
	long l_FingerWaitTime[2][3];
	int mn_stacker_updn_cyliner[2];
	int mn_pre_move_flag[2];
	int	mn_bcr_goodbin_cnt;
	int mn_bcr_rejectbin_cnt;
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	int mn_Clipretry;//20121126
	double dUldCurPos[10];//20121126
	double dSorterCurPos[10];//20121126
	
	long l_until_wait_time[2][3];
	long l_ReinStateWaitTime[3];
	
	double dAbsPos[2];
	double dUldAbsPos[2];
	double md_Robot_X_Pos;
	double md_Robot_Y_Pos;
	double md_Robot_Z_Pos;
	double md_UldRobot_X_Pos;
	double md_UldRobot_Y_Pos;
	double md_UldRobot_Z_Pos;
	double m_dTargetPos;
	
	int mn_FirstTray_Y_Pos;
	double mCurrent_RowPos;
	double md_CurrentPos;
	
	int HeadVal;
	
	long	mlp_AxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
	long	msp_AxisNum[4];
	
	double md_SortAbsPos[2];
	double md_UldAbsPos[2];	
} ;
extern  st_thread_Run_Rbt_Sorter_param  th_reco_Run_Rbt_Sorter;

struct st_thread_Run_Rbt_VisionLabel_param 
{
	int mn_VisionStep[2];//[0]:vision 1, [1]:vision 2
	int mn_VisionRbt_WorkingPos_Site[2];
		
	int PrevStep;
	int Run_labelStep;
	int Run_Vision1;
	int Run_Vision2;
	int MoveStep;
	int InitStep;
	int ReinstateStep;
	
	int mn_retry;
	int m_nTrargetPos[2];
	int m_nZPos[2];
	int m_nSite;
	int m_nLabelNum;
	int m_nVisChkNum;
	int mn_vis_result[100];
	int m_nPick_Label;
	
	int mn_printnum;
	int m_nOutPrint;
	int m_inspect;
	int m_nPick;
	int m_nOrder;
	
	double m_dTargetLabel;
	double m_dMovePos_1[2];
	double m_dMovePos_2[2];
	
	long l_PickerWaitTime[3];
	int mn_Safety_Flag;
	
	int m_buffPos[2];
	int m_buffStatus[2];
	int mn_attach_err;
	long md_offset;
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	int mn_visionretry;//20121126
	double dVisionCurPos[10];//20121126
	
	long ml_dvc_clamp_wait[2][3];
	
	long ml_dvc_rotator_wait[2][3];
	
	int n_bcrlot_step;
	long ml_untilwait_time[3];
	
	long NetworkConnectTime[3];
	long NetworkingTime[3];
	
	int mn_singleprint;
	int mn_emptyprint;
	int mn_labelprint;
	
} ;
extern  st_thread_Run_Rbt_VisionLabel_param  th_reco_Run_Rbt_VisionLabel;

struct st_thread_Run_Rbt_Work_Buffer_param 
{
	int MoveStep;
	int InitStep;
	int ReinstateStep;
	int RunBuff1Step;
	int RunBuff2Step;
	int RunBuff3Step;
	int RunBuff4Step;
	int HS1Step;
	int HS2Step;
	int HS3Step;
	int HS4Step;
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	int mn_Clipretry;//20121126
	double dWorkBufferCurPos[10];//20121126
	
	long l_ReinStateWaitTime[3];
	
	int mn_retry;
	int mn_clipretry[4];
	long l_Unitil_WaitTime[3];//20120816
	long l_ClipWaitTime[4][3];
	
	long l_Work1_WaitTime[3];
	long l_Work2_WaitTime[3];
	long l_Work3_WaitTime[3];
	long l_Work4_WaitTime[3];	
} ;
extern  st_thread_Run_Rbt_Work_Buffer_param  th_reco_Run_Rbt_Work_Buffer;

struct st_thread_Run_Stacker_Load_Clip_param 
{
	int PrevStep;
	int RunStep;
	int RunTransStep;
	int MoveStep;
	int InitStep;
	int PushStep;
	int TransUpDnStep;
	int ClipOutStep;
	int FixStep;
	int RotatorStep;
	int ReinstateStep;
	int RunInsertStep;
	int Change_step;
	int MoveTransferStep;
	int RunLdClipStep;
	int RunUldClipStep;
	
	int RunLdTubeStep; //20130204
	int RunUldTubeStep;//20130204
	
	int mn_LdTube;
	int mn_LdRotateTube;
	int mn_UnloadTubeNum;
	
	int mn_retry;
	int mn_change_clip;
	int mn_Tuberetry;
	
	long l_PickerWaitTime[3];
	long l_Until_WaitTime[3];
	long l_UntilWaitTime[3];
	
	int mn_Ldretry;
	long l_Until_Ld_WaitTime[3];
	int mn_Uldretry;
	long l_Until_Uld_WaitTime[3];
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	double dClipStackerCurPos[10];//20121126	
} ;
extern  st_thread_Run_Stacker_Load_Clip_param  th_reco_Run_Stacker_Load_Clip;

struct st_thread_Run_Stacker_Load_LeftHeat_param 
{
	int PrevStep;
	int RunS1Step;
	int RunS2Step;
	int RunMStep;
	int MoveStep;
	int InitStep;
	int RunTransStep;
	int ReinstateStep;
	int TrayMoveStep;
	int TrayBackStep;
	int TraySeperateStep;
	int ReadyPosStep[2];
	int MotorMoveStep[2];
	int mn_stacker_move_flag;
	int mb_sdstop;
	int initial_smove;
	
	int mn_StackerPos;
	int mn_retry;
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	
	double m_dTargetPos[2];
	double m_dNowPos[2];
	int mn_stacker_updn_cyliner[2];
	
	//stacker1에만 적용
	int mb_clampcylinder_flag;
	long ml_clampcylinder_Time[3];
	
	int mb_pushercylinder_flag[2];
	long ml_pushercylinder_Time[2][3];
	
	int mb_updncylinder_flag[2];
	long ml_undncylinder_Time[2][3];
	
	long ml_latchcylinder_Time[3];
	
	long ml_pusherupdncylinder_Time[3];
	
	long ml_cylinder_Time[3];
	
	long ml_until_waittime[3];
	
	//add
	int init_stacker_move;
	int init_stacker_1;
	int init_stacker_2;
	
	int mn_retry1;
	int mn_retry2;
	int mn_ms_retry; //Moving Stacker Retry
	int mn_stacker1_init_chk;
	int mn_stacker2_init_chk;
	int mn_Moving_stacker;
	int mn_m_uld_stacker;
	
	int mn_LeakM_LotEnd[4]; //[0] : stacker1 [1] : Transfer [2] : Stacker2 [3] : Stacker Move
	
} ;
extern  st_thread_Run_Stacker_Load_LeftHeat_param  th_reco_Run_Stacker_Load_LeftHeat;

struct st_thread_Run_Stacker_Load_Module_param 
{
	int RunS1Step;
	int RunS2Step;
	int RunMStep;
	int MoveStep;
	int InitStep;
	int TrayBackStep;
	int ReadyPosStep[2];
	int MotorMoveStep[2];
	int ReinstateStep;
	int RunTransStep;
	int TrayMoveStep;
	int mn_stacker_move_flag;
	int mn_stacker_Up;
	int init_stacker_move;
	int init_stacker_1;
	int init_stacker_2;
	int initial_smove;
	int mn_LeakM_LotEnd[4];
	
	int mn_stacker_updn_cyliner[2];
	long ml_StackerTrayCheck_Wait[4][3];
	
	long ml_latchcylinder_Time[3];
	
	double m_dCurPos;
	double m_dTargetPos[2];
	double m_dNowPos[2];
	int m_nMotorNum[2];
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	
	
	int mb_pushercylinder_flag[2];
	long ml_pushercylinder_Time[2][3];
	
	int mb_updncylinder_flag[2];
	long ml_undncylinder_Time[2][3];
	
	//Push, Back cylinder
	int mb_cylinder_flag;
	long ml_cylinder_Time[3];
	
	int mb_pusherupdncylinder_flag;
	long ml_pusherupdncylinder_Time[3];
	
	
	//add
	int mn_retry;
	int mn_retry1;
	int mn_retry2;
	int mn_ms_retry; //Moving Stacker Retry
	int mn_stacker1_init_chk;
	int mn_stacker2_init_chk;
	int mn_Moving_stacker;
	int mn_m_uld_stacker;	
} ;
extern  st_thread_Run_Stacker_Load_Module_param  th_reco_Run_Stacker_Load_Module;

struct st_thread_Run_Stacker_Load_RightHeat_param 
{
	int PrevStep;
	int RunS1Step;
	int RunS2Step;
	int RunMStep;
	int MoveStep;
	int InitStep;
	int RunTransStep;
	int ReinstateStep;
	int TrayMoveStep;
	int TrayBackStep;
	int TraySeperateStep;
	int ReadyPosStep[2];
	int MotorMoveStep[2];
	int mn_stacker_move_flag;
	int mb_sdstop;
	int init_stacker_move;
	int init_stacker_1;
	int init_stacker_2;
	int initial_smove;
	
	int mn_StackerPos;
	int mn_retry;
	
	double m_dTargetPos[2];
	double m_dNowPos[2];
	int mn_stacker_updn_cyliner[2];
	
	//Stacker1에만 해당
	int mb_clampcylinder_flag;
	int ml_clampcylinder_Time[3];
	
	int mb_pushercylinder_flag[2];
	long ml_pushercylinder_Time[2][3];
	
	int mb_updncylinder_flag[2];
	long ml_undncylinder_Time[2][3];
	
	long ml_latchcylinder_Time[3];
	
	long ml_pusherupdncylinder_Time[3];
	
	long ml_cylinder_Time[3];
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	
	long ml_until_waittime[3];
	
	//add
	int mn_retry1;
	int mn_retry2;
	int mn_ms_retry; //Moving Stacker Retry
	int mn_stacker1_init_chk;
	int mn_stacker2_init_chk;
	int mn_Moving_stacker;
	int mn_m_uld_stacker;
	
	int mn_LeakM_LotEnd[4]; //[0] : stacker1 [1] : Transfer [2] : Stacker2
	
} ;
extern  st_thread_Run_Stacker_Load_RightHeat_param  th_reco_Run_Stacker_Load_RightHeat;

struct st_thread_Run_Stacker_Sorter_param 
{
	int RunStep;
	int RunMStep;
	int MoveStep;
	int InitStep;
	int ReinstateStep;
	int ReadyPosStep_old[2];
	int ReadyPosStep[2];
	int MotorMoveStep;
	int WorkPosStep_old;
	int WorkPosStep;
	int RunS1Step;
	int RunS2Step;
	int RunTransStep;
	int StackerReayStep;
	int TrayMoveStep;
	int TraySeperateStep;
	int TrayBackStep;
	int mn_StackerPos;
	int mn_StackerTrayReady;
	int mn_stacker_move_flag;
	int initial_smove;
	
	int mn_retry;
	long ml_pusherupdncylinder_Time[3];
	long ml_pushercylinder_Time[2][3];
	long ml_latchcylinder_Time[3];
	long l_PickerWaitTime[2][3];
	double dTargetPos;
	double m_dTargetPos[2];
	double m_dNowPos[2];
	long ml_clamp_time[3];
	long ml_pusher_wait[3];
	int mResult[1];
	int mb_updncylinder_flag[2];
	long ml_undncylinder_Time[2][3];
	int mn_stacker_updn_cyliner[2];
	long ml_pusher_Time[3];
	
	int init_stacker_move;
	int init_stacker_1;
	int init_stacker_2;
	
	
	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	
	double md_CurrentPos;	
	int mn_retry1;
	int mn_retry2;
	int mn_ms_retry; //Moving Stacker Retry
	int mn_stacker1_init_chk;
	int mn_stacker2_init_chk;
	int mn_Moving_stacker;
	int mn_m_uld_stacker;
	
	int mn_LeakM_LotEnd[4]; //[0] : stacker1 [1] : Transfer [2] : Stacker2
	
} ;
extern  st_thread_Run_Stacker_Sorter_param  th_reco_Run_Stacker_Sorter;
////





struct st_size_param 
{
	int n_picker;
	
	int n_table_x;
	int n_tray_x; 
	int n_tray_y;
	
	double d_ldrbt_tray_mm_y;
	double d_ldrbt_tray_mm_x;
	
	double d_uldrbt_tray_mm_y;
	double d_uldrbt_tray_mm_x;
};
extern  st_size_param  st_size;

struct st_tray_param 
{
	int n_dvccnt;					//디바이스 수량 
	int n_bin;						//현재 트레이 위치에 공급된 트레이의 빈 정보를 알 수 있다 
	int n_map[20][20];
	int n_first_map[20][20];
};
extern  st_tray_param  st_tray[5];//0:load1,1:load2, 2:uld1,3:uld2,4:uld3


#define NETWORK_CNT						4

#define NETWORK_ECSERVER				0
#define NETWORK_NMACHINE				1
#define NETWORK_SERVER					2
#define NETWORK_FRONT					3
#define NETWORK_APCT					2

// *************************************************************************
//  NETWORK 상태 메시지 정의
// *************************************************************************
#define N_ESTABLISHED		1
#define N_FAILED			2
#define N_ABANDONNED		3

struct st_network_param
{
	CString		mstr_ip;							// ip address
	CString		mstr_port;							// port
	CString		mstr_ldport;						// Lot Display port

	CString		mstr_server_port;

	CString		mstr_id;
	CString		mstr_pwd;

	int			b_contact_status;					// contact status (N_ESTABLISHED: 연결됨, N_FAILED: 연결 안됨)
	CString		mstr_send_msg_status;				// 장비의 상태를 사용자가 지정
	CString		mstr_send_msg;
	CString		mstr_receive_msg;
	
	CTimeSpan	m_NetworkIdle;						//network idle time (메세지 전송이 없는 시간 확인용)
	
	int			n_check_ok;
	int			n_WorkNetwork;
	
	CString		str_buffer_send_data;
	int			n_buffer_send_index;
	
	int			n_event_parameter;
	
	CString		mstr_network_return_flag;
	
	CString		mstr_send_msg_to_tester;
	CString		mstr_tester_message;

	CString		str_ec_server_message[30];

	CString		mstr_send_message_temp[24];
};
extern st_network_param st_Network[NETWORK_CNT];

// ******************************************************************************
//  TEST REFERENCE화면 환경 설정 정보 저장 구조체 선언                           
// ******************************************************************************
struct st_testreference_param 
{
	int mn_mode_socketoff[3];			// Socket Off 종류 저장 변수
	int mn_mode_stationoff[3];			// Station Off 종류 저장 변수
	
	int mn_absolute_yield;
	int mn_relative_yield;
	int mn_continuity_count;
	int mn_apply_count;

	int mn_station_yield;
	int mn_machine_yield;
	int mn_line_yield;
	int mn_station_apply_count;
	
	int mn_bin_num[BINNUM];				// Combine BIN 저장 변수
	
	int mn_ts_contact_cnt[TESTSIZE];	// 소켓 컨택 횟수 저장 변수
	
	int mn_continus_fail_cnt[TESTSIZE];	// 연속 FAIL 발생 횟수 저장 변수
	int mn_continus_fail_cnt_for_airblow[TESTSIZE];	//AirBlow용 연속 FAIL 발생 횟수 저장 변수	2K6/09/21/ViboX
	
	int mn_socket_test_cnt;		// Test 발생 횟수 저장 변수
	int mn_socket_test_cnt_for_airblow;	// AirBlow용 Test 발생 횟수 저장 변수	2K6/09/21/ViboX

	int mn_board_test_cnt[80];		// Test 발생 횟수 저장 변수
	
	long ml_contact_limit_cnt;			// 소켓 컨택 최대 허용 횟수 저장 변수
	long ml_off_time_w;					// OFF LINE 테스트 대기 시간 저장 변수
	long ml_test_eot_time_l;				// 테스트 EOT 리미트 시간 저장 변수
	int mn_offline_bin_set[8];
	
	int mn_fail_check_bin[BINNUM];		// All Fail Check Bin 2K10/01/18/ViboX
	int mn_retest_check[BINNUM];		// Retest Check Bin 2K10/12/30/ViboX

	int mn_rack_skip_1;
	int mn_rack_skip_2;
	int mn_rack_skip_3;
	int mn_rack_skip_4;

	bool mb_rack_skipable_A;
	bool mb_rack_skipable_B;
	bool mb_rack_skipable_C;
	bool mb_rack_skipable_D;
	bool mb_rack_skipable_E;
	bool mb_rack_skipable_F;
	bool mb_rack_skipable_G;
	bool mb_rack_skipable_H;

	BOOL mb_socketoff_nto;
	BOOL mb_socketoff_bto;
	BOOL mb_socketoff_hto;
	BOOL mb_socketoff_fail;


} ;
extern  st_testreference_param  st_testreference;
// ******************************************************************************

struct st_robot_picker_param
{
	int nExist;					// 디바이스 유무.
	int nTestCount;				// 테스트 카운트.
	char cSerial[20];			// 시리얼
	int nArrary;				// Array
	char cResult[10];			// 결과.
	char cLotNo[20];			// Lot No
};
extern  st_robot_picker_param  st_robot_picker[PICKER_NUM];

struct st_pct_buffer_param
{
	char cLotNo[20];
	int nLastModule;

	struct 
	{
		int nExist;					// 디바이스 유무.
		int nTestCount;				// 테스트 카운트.
		char cSerial[20];			// 시리얼
		int nArrary;				// Array
		char cResult[10];			// 결과.
		char cLotNo[20];			// Lot No
    } 
	pb_socket_data[BUFFERMAXSIZE];
};
extern  st_pct_buffer_param  st_pct_buffer;

struct st_modulemap_size
{
	int nDeviceRobotPicker[4][PICKER_NUM];			// 0:Pickerinfo, 1:bin정보, 2:retest count, 3:Array
	int nPickerEnable[PICKER_NUM];
	char cDeviceRobotPicker[3][PICKER_NUM][20];	// 0:시리얼, 1:Lot No, 2:결과

	int nRetestSocketUse[SOCKET_NUM];

	int TestSite[3][TESTSIZE];					// 0:디바이스 유/무 정보, 1:retest count 횟수 정보
	int TestSite_Gripper[3][TESTSIZE];			// 0:디바이스 유/무 정보, 1:retest count 횟수 정보
	int Buffer[4][BUFFERMAXSIZE];				// 0 = 0: bin 등급설정 1: device exist or empty , 2:retest bin일때 retest count 정보, 3:Screen Info									//BUFFERMAXSIZE : module empty=0 or Exist =1
	
	// Tray On/Off 2K6/06/19/ViboX
	int LoadTrayData[LOADTRAY_COL][LOADTRAY_ROW];
	int UnloadTrayData[UNLOADTRAY_COL][UNLOADTRAY_ROW];
	int RejectTrayData[REJECTTRAY_COL][REJECTTRAY_ROW];
	
	int LoadTrayDataTemp[LOADTRAY_COL][LOADTRAY_ROW][2];
	int UnloadTrayDataTemp[UNLOADTRAY_COL][UNLOADTRAY_ROW][2];
	int RejectTrayDataTemp[REJECTTRAY_COL][REJECTTRAY_ROW][2];
	
	CString BarcodeInformation[3][PICKER_NUM];	// 0:Picker, 1:Test Site Gripper, 2:Test Site
	
	int SocketEnable[PICKER_NUM];				// Socket Off
	int SocketOff[PICKER_NUM];					// Socket Off 이유.
		
	
	int CurrentColumns[10];						// LOADTRAY-1,UNLOADTRAY-2,REJECTTRAY-3,TESTSITE-4, BUFFERTRAY-5
	int CurrentRows[10];						// LOADTRAY-1,UNLOADTRAY-2,REJECTTRAY-3,TESTSITE-4, BUFFERTRAY-5

	// 각 사이트에 자재 유무.
	int ReceiveBufferFront[3][20];
	int ReceiveBufferRear[3][20];
	int ReceiveBuffer[2][3][20];

	int FrontShifter[3][20];
	int RearShifter[3][20];

	// 각 사이트에 자재 유무.
	int Site[2][4][3][20];		// 0:디바이스 유/무 정보, 1:retest count 횟수 정보

	int SendBufferFront[3][20];
	int SendBufferRear[3][20];
	int SendBuffer[2][3][20];

	CString BcrData_UldBuffer;	//20120514 유성준

	//Module
	int nModuleRobotPicker[D_INFOSIZE][PICKER_NUM];			// 0:Pickerinfo, 1:bin정보, 2:retest count, 3:Array
	int PickerEnable[PICKER_NUM];				// Picker Off
	int LoadTrayState[LOADTRAY_ROW];		
	int LoadTray[2][LOADTRAY_ROW];

	//Lefths
	int nLefthsRobotPicker[D_INFOSIZE][PICKER_NUM];			// 0:Pickerinfo, 1:bin정보, 2:retest count, 3:Array
	int LefthsPickerEnable[PICKER_NUM];				// Picker Off
	int LefthsTrayState[LOADTRAY_COL][LOADTRAY_ROW];		
	int LefthsTray[2][LOADTRAY_ROW];

	//Righths
	int nRighthsRobotPicker[D_INFOSIZE][PICKER_NUM];			// 0:Pickerinfo, 1:bin정보, 2:retest count, 3:Array
	int RighthsPickerEnable[PICKER_NUM];				// Picker Off
	int RighthsTrayState[LOADTRAY_COL][LOADTRAY_ROW];		
	int RighthsTray[2][LOADTRAY_ROW];

	//Clip
	int nClipRobotPicker[D_INFOSIZE];			// 0:Pickerinfo, 1:bin정보, 2:retest count, 3:Array

	int ClipTray[2][LOADTRAY_ROW*3];              //[0] : left [1] : right

	// LOADTRAY_ROW 값이 30이면 중간에 값이 -1이 들어가서 
	//*2 해주니깐 정상적으로 0값이 들어 갔습니다.




	//Sorter
	int nSortRobotPicker[D_INFOSIZE];			// 0:Pickerinfo, 1:bin정보, 2:retest count, 3:Array

	//Unloader
	int nUldRobotPicker[D_INFOSIZE];			// 0:Pickerinfo, 1:bin정보, 2:retest count, 3:Array
	int UnloadTrayState[LOADTRAY_COL][LOADTRAY_ROW];		
	int UnloadTray[2][LOADTRAY_ROW];

	int RejectTray[LOADTRAY_ROW];
	//각각의 트레이의 pocket(slot)별 on/off 상태를 저장 관리한다..
	int RejectTrayState[REJECTTRAY_COL][REJECTTRAY_ROW]; 

	int WorkBuffer[D_INFOSIZE][4][5]; //[][*][0]:* 버퍼순사 버퍼 순서 WORK_BUFFER_1
									  //[][0][*]:* 사이트 버퍼위치(LD,SINK,CLIP,ULD)
	
	int VisionBuffer[D_INFOSIZE][2];


} ;
extern  st_modulemap_size  st_modulemap;

#define DATAMAP_FRONT			0
#define DATAMAP_LDBUFFER		1
#define DATAMAP_SITE1			2
#define DATAMAP_SITE2			3
#define DATAMAP_SITE3			4
#define DATAMAP_SITE4			5
#define DATAMAP_ULDBUFFER		6
#define DATAMAP_RETEST			7
#define DATAMAP_SHIFTER			8
#define DATAMAP_SITE5			9		// 앞 장비에서만 사용
#define DATAMAP_SITE6			10
#define DATAMAP_SITE7			11
#define DATAMAP_SITE8			12
// 0: 오기전, 1: LD BUFFER, 2: SITE1, 3: SITE2, 4: SITE3, 5: SITE4, 6: ULD BUFFER, 7: RETEST 오기전, 8: SHIFTER, 9: SITE5, 10: SITE6, 11: SITE7, 12: SITE8
struct st_module_datamap_param
{
	int mnDeviceExistence[20];				// Device 존재
	int mnSocketOff[20];					// Socket Off 유무
	int mnSocketOffStatus[20];				// Socket Off 이유
	CString mstrInfo[20];					// Device 정보
	CString mstrLotNo;						// Lot 정보
	CString mstrPartNo;						// Part 정보
	CString mstrTestResult[20];				// Test 결과.
	CString mstrSerial[20];					// Serial No.
	CString mstrLot[20];					// Lot No.
	int mnArrayInfo[20];					// Array 정보
	int mnRetestCnt[20];					// Retest Count
	
	int mnBoardOnOff[10];
	int mnBoardStatus[10];

	int mnLastModule;						// 마지막 자재인가?
	int mnByPass;							// 통과 시킬 자재인가?

	CString mstrLastModule;					// 마지막 자재인가?
	CString mstrByPass;						// 통과 시킬 자재인가?

	CString mstrRetest;						// 리테스트 정보

	CString mstrRetestCheck;				// 리테스트를 받을수 있는 상태인가... (새로 추가 됨.)

	CString mstrMdlCnt;						// 자재의 갯수.

	int mnBootTest;							// 부팅 테스트만 하는것이냐.
	int mnRemain;							// 남아있는 자재가 있느냐... 2K11/05/28/ViboX

	long ml_test_count[20];

	int nPickPos;
};
extern  st_module_datamap_param  st_module_datamap[2][13];

// ******************************************************************************
//  작업 결과 정보 저장 구조체 선언                                              
// ******************************************************************************
struct st_test_param 
{
	// **************************************************************************
	// 자재 테스트 진행 갯수 저장 리스트 파일 관련 정보 저장 변수                
	//  : BINNUM [0:GOOD BIN    1-7:REJECT BIN)                                  
	//  : TESTNUM [0:소켓] -> 현재는 소켓이 1개 존재                             
	// **************************************************************************
	int cp_result_cnt[BINNUM];			// 테스트 BIN 갯수 임시 저장 변수
	int cp_retest_result_cnt[BINNUM];	// 리테스트 BIN 갯수 임시 저장 변수
	
	int cp_shift_cnt[BINNUM];			// SHIFT별 BIN 갯수 임시 저장 변수
	int cp_daily_cnt[BINNUM];			// 일별 BIN 갯수 임시 저장 변수
	int cp_monthly_cnt[BINNUM];			// 월별 BIN 갯수 임시 저장 변수
	
	int cp_total_load_cnt;
	int cp_good_cnt;					// GOOD BIN 갯수 저장 변수
	int cp_reject_cnt;					// REJECT BIN 갯수 저장 변수
	int cp_dvc_bin_retestcnt;
	int cp_dvc_retestbin_retestcnt;
	// **************************************************************************
	
	int cp_total_load_retestcnt;		// Retest Load 갯수 저장 변수
	int cp_good_retestcnt;				// Retest GOOD BIN 갯수 저장 변수
	int cp_reject_retestcnt;			// Retest REJECT BIN 갯수 저장 변수
	
	int mn_socket_good[TESTSIZE];
	int mn_socket_reject[TESTSIZE];
	int mn_socket_test[TESTSIZE];
	float f_socket_yield[TESTSIZE];
	
	int mn_socket_data[6][TESTSIZE + 1][BINNUM + 1];	// (LOT, DAY, MONTH, SHIFT별), 소켓 번호, BIN넘버.
	int mn_socket_screen_data[6][TESTSIZE + 1][BINNUM + 1];	// (LOT, DAY, MONTH, SHIFT별), 소켓 번호, BIN넘버.
	int mn_data_type;
	
	int mn_test[4];
} ;
extern  st_test_param  st_test;
// ******************************************************************************

// ******************************************************************************
// RS-232C 시리얼 통신 관련 저장 구조체 선언                                     
// ******************************************************************************
// struct st_serial_param 
// {
// 	int mn_snd_chk[MAX_PORT];         // 데이터 송신 완료 플래그
// 	int mn_rec_chk[MAX_PORT];         // 데이터 수신 완료 플래그
// 	int mn_parsing[MAX_PORT];         // 수신 데이터 파싱 상태 플래그
// 	
// 	CString mstr_port_chk[MAX_PORT];  // 시리얼 포트 생성 정보 저장 변수
// 	
// 	CString mstr_snd[MAX_PORT];       // 송신 메시지 저장 변수
// 	CString mstr_rec[MAX_PORT];       // 수신 메시지 저장 변수
// 	
// 	CString mcomm_snd[MAX_PORT];      // 리스트 화면 출력 송신 메시지 저장 변수
// 	CString mcomm_rec[MAX_PORT];      // 리스트 화면 출력 수신 메시지 저장 변수
// 	CString mcomm_err[MAX_PORT];      // 리스트 화면 출력 에러 메시지 저장 변수
// 
// 	CString mstrBarcodeDataBuffer;
// 	int mnBarcodeDataBuffer;
// } ;
// extern  st_serial_param  st_serial;
//////////////////////////////////////////////////////////////////// 20120514 유성준

struct st_serial_param 
{
	int			n_connect[10];
	int			n_snd_chk[10];         // 메시지 송신 완료 플래그
	int			n_rec_chk[10];         // 메시지 수신 완료 플래그
	
	CString		str_port_chk[10];  // 시리얼 포트 생성 정보 저장 변수

	CString		str_snd[10];       // 송신 메시지 저장 변수
	CString		str_rec[10];       // 수신 메시지 저장 변수

	CString		comm_snd[10];      // 리스트 화면 출력 송신 메시지 저장 변수
	CString		comm_rec[10];      // 리스트 화면 출력 수신 메시지 저장 변수
	CString		comm_err[10];      // 리스트 화면 출력 에러 메시지 저장 변수

// 	int			n_temp_code;
// 	int			n_temp_data;

// 	int			n_rcv_rfid_check;
// 	int			n_snd_rfid_check;

};
extern st_serial_param  st_serial;

struct st_serial_info
{
	int			n_serial_port[10];		// serial port....
	int			n_serial_baudrate[10];	// serial baudrate....
	int			n_serial_data[10];		// serial data....
	int			n_serial_parity[10];	// serial parity.....
	int			n_serial_stop[10];		// serial stop....

};
extern struct st_serial_info rs_232;

// *************************************************************************
// 기타 정보 저장 구조체 선언                                                    
// *************************************************************************
struct st_other_param 
{
	CString str_op_msg;			// 리스트 화면 출력 동작 에러 메시지 저장 변수
	CString str_normal_msg;		// 리스트 화면 출력 동작 에러 메시지 저장 변수
	CString str_abnormal_msg;	// 리스트 화면 출력 동작 에러 메시지 저장 변수
	CString str_snd_msg;		// 리스트 화면 테스터 SEND 메시지 저장 변수
	CString str_rcv_msg;		// 리스트 화면 테스터 RECIVE 메시지 저장 변수
	CString str_bcr_rcv_msg;	// 리스트 화면 바코드 RECIVE 메시지 저장 변수
	CString str_n_snd_msg;		// 리스트 화면 NETWORK SEND 메시지 저장 변수
	CString str_n_rcv_msg;		// 리스트 화면 NETWORK RECIVE 메시지 저장 변수

	CString str_fallacy_msg;	// 오류 메시지 대화 상자 타이틀 저장 변수
	CString str_confirm_msg;	// 확인 메시지 대화 상자 타이틀 저장 변수
	CString str_typing_msg;		// 키보드 대화 상자 타이틀 저장 변수

	CString str_keypad_msg;		// 키패드 대화 상자 타이틀 저장 변수
	CString str_pad_val;		// 키패드 대화 상자에 출력할 데이터 저장 변수

	CString str_pad_high_limit;
	CString str_pad_low_limit;

	CString str_pass_level;		// 암호 레벨 저장 변수
	CString str_password;		// 암호 저장 변수
	
	CString str_parent_menu;	// 주메뉴 이름 저장 변수
	
	CString m_TCP_S_Msg;		// 보낼 메세지
	CString m_TCP_R_Msg;		// 받은 메세지
	
	int n_teaching_pendant_axis;
	
	int n_Firstpicker_num[10];
	int n_dot_use;				// 소수점을 사용하느냐?
	
	int n_picker_site;
	
	int n_pad_dialog;

	char c_normal_msg[200];
	char c_abnormal_msg[200];

	char c_rev_msg[5000];
	char c_send_msg[5000];

	////////////////////////////////2K10/02/09/ViboX////////////////////////////////////////
	int		n_prev_step[100];			// 이전 step (변경 되었는지 보기 위함)
	CString	str_history_step[100];		// step_history 약 50개에 해당하는 step을 저장
	CString str_last_change_date[100];	// 최종 step으로 변경되는 시점의 시간

	BOOL b_yield_ok;	//20120620
	BOOL b_ml_ok;	//20120620
	BOOL b_m_exit;	//20120711
	BOOL b_HSRET;	//20120711

	//////////////// 20120924
	BOOL b_BCR_Pass;
	BOOL b_BCR_Fail;
	BOOL b_HS_Pass;
	BOOL b_HS_Fail;
	////////////////

	//////////////////////////////20120924
	BOOL b_BCR_Data_Reset_OK;
	BOOL b_HS_Data_Reset_OK;
	BOOL b_BCR_Data_Reset_OK1;
	BOOL b_HS_Data_Reset_OK1;
	//////////////////////////////
	
	BOOL b_SERIAL_NO_RECEIVE;//2014,0303

};
extern  st_other_param  st_other;
//////////////////////////////////////////////////////////////////// 


// ******************************************************************************

struct st_nw_param
{
	int			mn_recived;
	int			mn_work;
	int			mnID;
	int			mnRID;
	int			nmsg_cnt;
	//	CString		mstr_full_msg[2];
	CString		mstr_full_msg[10];
	CString		mstr_header_msg[2];
	CString		mstr_data_msg[2];
	CString		mstr_function[2];
	CString		mstr_lot_id[2];
	CString		mstr_lot_no[2];
	CString		mstr_eqp_id[2];
	CString		mstr_mode[2];
	CString		mstr_oper_cnt[2];
	CString		mstr_oper_id[2];
	CString		mstr_format_retry[2];
	CString		mstr_oper_info[2];
	CString		mstr_pcbox_id[2];
	CString		mstr_user_id[2];
	CString		mstr_status[2];
	CString		mstr_serial[2];
	CString		mstr_pcbox_cnt[2];
	CString		mstr_pcbox_info[2];
	CString		mstr_lot_qty[2];
	CString		mstr_slot[2];
	CString		mstr_step_cnt[2];
	CString		mstr_part_id[2];
	CString		mstr_cycle_id[2];
	CString		mstr_temp[2];
	CString		mstr_result[2];
	CString		mstr_qty[2];
	CString		mstr_test_result[2];
	CString		mstr_novideo_cnt[2];
	CString		mstr_fail_no[2];
	CString		mstr_fail_cnt[2];
	CString		mstr_fail_log[2];
	CString		mstr_fail_slot[2];
	CString		mstr_fail_type[2];
	CString		mstr_fail_desc[2];
	CString		mstr_fail_code[2];
	CString		mstr_fail_info[2];
	CString		mstr_retest_no[2];
	CString		mstr_retest_id[2];
	CString		mstr_retest_flag[2];
	CString		mstr_slot_no[2];
	CString		mstr_slot_mask[2];
	CString		mstr_test_idx[2];
	CString		mstr_fail_regnumber[2];
	CString		mstr_error_log[2];
	CString		mstr_message_id[2];
	CString		mstr_bin_info[2];
	CString		mstr_scrap_cnt[2];
	CString		mstr_scrap_info[2];
	CString		mstr_desc[2];

	CString     mstr_fmats[2];      // 20130115
	CString     mstr_bmats[2];      // 20130115


	CString		mstr_station[2];			// 온도용
	CString		mstr_temperature[2];		// 온도용
	
	CString		mstr_pcbox[2][150];
	CString		mstr_pcboxslot[2][150];
	CString		mstr_pcboxlocation[2][150];
	CString		mstr_pcboxenable[2][150];
	
	CString		mstr_recyclelotid[2];
	CString		mstr_recycleqty[2];
	
	CString		mstr_msg[2];
	
	CString		mstr_buffer_rcv_data[10][100];
	CString		mstr_buffer_send_data[10];
	int			mn_rcv_data[10];
	int			mn_find_data[10];
	
	CString		mstr_client_rev;
	CString     mstr_client_send;
	int			m_nsend_rpy;
	int			m_nready;
	
	CString		mstr_staging[2];
	CString		mstr_position[2];
	int			m_nvideo_out_rpy;
	int			m_ngroup_pwr_rpy;

	// 장비간 통신하는 프로토콜용 변수.
	CString		mstr_m_eqp_id[2];
	CString		mstr_m_buffer_no[2];
	CString		mstr_m_station[2];
	CString		mstr_m_lot_no[2];
	CString		mstr_m_part_no[2];
	CString		mstr_m_last_module[2];
	CString		mstr_m_bypass[2];
	CString		mstr_m_mode[2];
	CString		mstr_m_module_cnt[2];
	CString		mstr_m_module_data[20][2];
	CString		mstr_m_retestcheck[2];

	CString		mstr_pct_bypass[2];
	
	CString		mstr_m_socket_off[2];

	CString		mstr_m_value[2];

	CString		mstr_m_r_result[2];
	CString		mstr_m_r_socket_off[2];
	CString		mstr_m_r_lot_no[2];
	CString		mstr_m_r_part_no[2];
	CString		mstr_m_r_eqp_id[2];
	CString		mstr_m_r_buffer_no[2];
	CString		mstr_m_r_last_module[2];
	CString		mstr_m_r_bypass[2];
	CString		mstr_m_r_retest[2];
	CString		mstr_m_r_module_cnt[2];
	CString		mstr_m_r_module_data[20][2];
	CString		mstr_m_r_station[2];

	CString		mstr_new_lot_no[2];

	CString		mstr_remain[2];
	int			mn_remain[2];							// 남아있는 자재가 있느냐... 2K11/05/28/ViboX

	CString		mstr_blot_count;

	CString		mstr_bpc_buffer_ready;
	CString		mstr_bpc_buffer_in;

	bool		mb_buffer_in_received[POS_COUNT];

	CString		mstr_buffer_in_recv[POS_COUNT];
	CString		mstr_buffer_in_temp[POS_COUNT];

	int mn_invaild_function_flag;
	int mn_update_version_flag;
	int mn_scrap_info_flag;
	int mn_lot_display_flag;

	/////////////////// 20120906
	CString		mstr_FPH[99999];//2015.0126
	CString		mstr_FPH_1[99999];//2015.0126
	CString		mstr_FPH_2[99999];//2016.0616
	CString		mstr_FPH_Data[99999];//2015.0126
	CString		mstr_Recive_Data[99999];
	CString		mstr_Print_Data[99999];
	CString		mstr_Unkown1[99999];
	CString		mstr_1Position[99999];
	CString		mstr_0Contents[99999];//2016.0616
	CString		mstr_1Contents[99999];
	CString		mstr_0Position[99999];//2016.0616
	CString		mstr_2Position[99999];
	CString		mstr_2Contents[99999];
	CString		mstr_3Position[99999];
	CString		mstr_3Contents[99999];
	CString		mstr_3Position_back[99999];
	CString		mstr_3Contents_back[99999];
	CString		mstr_4Position[99999];
	CString		mstr_4Contents[99999];
	CString		mstr_2DPosition[99999];
	CString		mstr_2DContents[99999];
	CString		mstr_Unkown2[99999];
	CString		mstr_LotNoPosition[99999];
	CString		mstr_LotNoContents[99999];
	CString		mstr_Recive_PartNo[99999];

///위치저장
	CString		mstr_1PosX_1[99999];
	CString		mstr_0PosX_1[99999];//2016.0616
	CString		mstr_2PosX_1[99999];
	CString		mstr_3PosX_1[99999];
	CString		mstr_3PosX_back_1[99999];
	CString		mstr_4PosX_1[99999];
	CString		mstr_2DPosX_1[99999];
	CString		mstr_Unkown2PosX_1[99999];
	CString		mstr_LotNoPosX_1[99999];

	CString		mstr_1PosY_1[99999];
	CString		mstr_0PosY_1[99999];//2016.0616
	CString		mstr_2PosY_1[99999];
	CString		mstr_3PosY_1[99999];
	CString		mstr_3PosY_back_1[99999];
	CString		mstr_4PosY_1[99999];
	CString		mstr_2DPosY_1[99999];
	CString		mstr_Unkown2PosY_1[99999];
	CString		mstr_LotNoPosY_1[99999];
///내용
	CString		mstr_0Contents_1[99999];
	CString		mstr_1Contents_1[99999];
	CString		mstr_2Contents_1[99999];
	CString		mstr_3Contents_1[99999];
	CString		mstr_3Contents_back_1[99999];
	CString		mstr_4Contents_1[99999];
	CString		mstr_2DContents_1[99999];
	CString		mstr_LotNoContents_1[99999];
	CString		mstr_Unkown2_1[99999];

	CString		mstr_1Contents_2[99999];
	CString		mstr_2Contents_2[99999];
	CString		mstr_3Contents_2[99999];
	CString		mstr_3Contents_back_2[99999];
	CString		mstr_4Contents_2[99999];
	CString		mstr_2DContents_2[99999];
	CString		mstr_LotNoContents_2[99999];

///넓이저장
	CString		mstr_1Wid_1[99999];
	CString		mstr_2Wid_1[99999];
	CString		mstr_0Wid_1[99999];//2016.0616
	CString		mstr_3Wid_1[99999];
	CString		mstr_3Wid_back_1[99999];
	CString		mstr_4Wid_1[99999];
	CString		mstr_2DWid_1[99999];
	CString		mstr_LotNoWid_1[99999];
	CString		mstr_Unkown2Wid_1[99999];


	CString		mstr_1High_1[99999];
	CString		mstr_2High_1[99999];
	CString		mstr_0High_1[99999];//2016.0616
	CString		mstr_3High_1[99999];
	CString		mstr_3High_back_1[99999];
	CString		mstr_4High_1[99999];
	CString		mstr_2DHigh_1[99999];
	CString		mstr_LotNoHigh_1[99999];
	CString		mstr_Unkown2High_1[99999];

	CString		mstr_1Etc_1[99999];
	CString		mstr_2Etc_1[99999];
	CString		mstr_3Etc_1[99999];
	CString		mstr_3Etc_back_1[99999];
	CString		mstr_4Etc_1[99999];
	CString		mstr_2DEtc_1[99999];
	CString		mstr_2DEtc_2[99999];
	CString		mstr_LotNoEtc_1[99999];
	CString		mstr_Unkown2Etc_1[99999];
   
	CString     mstr_Label_Pasing[99999];//20140215

   	CString		mstr_Old_LotNo;//20120924

	CString     mstr_Test_LotNo;//2014,0218
	CString     mstr_Test_SerialNo;//2014,0218
	
/////////////////// 
};
extern st_nw_param st_NW;

// ******************************************************************************
// I/O 명칭 저장 구조체 선언                                                     
// ******************************************************************************
struct st_io_param 
{
	// module 0
	int o_m_stacker_guide_clamp_onoff;		//s0000
	int o_m_stacker_guide_unclamp_onoff;
	int o_m_stacker1_up_onoff;
	int o_m_stacker1_dn_onoff;
	int o_m_stacker2_up_onoff;
	int o_m_stacker2_dn_onoff;
	int o_m_stacker_tray_pusher_bwd_onoff;
	int o_m_stacker_tray_pusher_fwd_onoff;
	int o_m_stacker_tray_pusher_clamp_updn;
	int o_m_stacker1_rail_fwd_onoff;
	int o_m_stacker1_rail_bwd_onoff;
	int o_m_stacker2_rail_fwd_onoff;
	int o_m_stacker2_rail_bwd_onoff;
	int o_m_stacker2_tray_clamp_onoff;

	// module 1
	int i_m_stacker_guide_clamp_on_chk;		//s0100
	int i_m_stacker_guide_unclamp_off_chk;
	int i_m_stacker1_up_chk;
	int i_m_stacker1_dn_chk;
	int i_m_stacker1_tray_chk;
	int i_m_stacker_1_updn_pos_chk;
	int i_m_stacker2_up_chk;
	int i_m_stacker2_dn_chk;
	int i_m_stacker2_tray_chk;
	int i_m_stacker_2_updn_pos_chk;
	int i_m_stacker_tray_pusher_bwd_chk;
	int i_m_stacker_tray_pusher_fwd_chk;
	int i_m_stacker_tray_pusher_clamp_up_chk;
	int i_m_stacker_tray_pusher_clamp_dn_chk;
	int i_m_stacker_tray_guide_chk;

	// module 2
	int i_m_stacker1_rail_tray_chk;
	int i_m_stacker1_rail_fwd_chk;
	int i_m_stacker1_rail_bwd_chk;
	int i_m_stacker2_rail_tray_chk;
	int i_m_stacker2_rail_fwd_chk;
	int i_m_stacker2_rail_bwd_chk;
	int i_m_stacker2_tray_clamp_on_chk;
	int i_m_stacker2_tray_clamp_off_chk;

	int i_HS_Forward_Barcode_Up_chk;
	int i_HS_Forward_Barcode_Down_chk;
	int i_HS_Backward_Barcode_Up_chk;
	int i_HS_Backward_Barcode_Down_chk;
	int i_Left_Clip_Barcode_Up_chk;
	int i_Left_Clip_Barcode_Down_chk;
	int i_Right_Clip_Barcode_Up_chk;
	int i_Right_Clip_Barcode_Down_chk;


	// module 3
	int	o_hs_fwd_stacker_guide_clamp_onoff;
	int o_hs_fwd_stacker_guide_unclamp_onoff;
	int o_hs_fwd_stacker1_up_onoff;
	int o_hs_fwd_stacker1_dn_onoff;
	int o_clip_remove_blower_on;//20121020
	int o_clip_remove_blower_off;//20121020
	int o_hs_fwd_stacker_tray_pusher_bwd_onoff;
	int o_hs_fwd_stacker_tray_pusher_fwd_onoff;
	int o_hs_fwd_stacker_tray_pusher_clamp_updn;
	int o_hs_fwd_stacker1_rail_fwd_onoff;
	int o_hs_fwd_stacker1_rail_bwd_onoff;
	int o_hs_fwd_stacker2_rail_fwd_onoff;
	int o_hs_fwd_stacker2_rail_bwd_onoff;
	int o_hs_fwd_stacker2_tray_clamp_onoff;
	int o_hs_fwd_clamp_onoff;//2012,0720 추가
	int o_hs_bwd_clamp_onoff;

	// module 4
	int i_hs_fwd_stacker_guide_clamp_on_chk;
	int i_hs_fwd_stacker_guide_unclamp_off_chk;
	int i_hs_fwd_stacker1_up_chk;
	int i_hs_fwd_stacker1_dn_chk;
	int i_hs_fwd_stacker1_tray_chk;
	int i_hs_fwd_stacker1_updn_pos_chk;
	int i_hs_fwd_stacker2_up_chk;
	int i_hs_fwd_stacker2_dn_chk;
	int i_hs_fwd_stacker2_tray_chk;
	int i_hs_fwd_stacker2_updn_pos_chk;
	int i_hs_fwd_stacker_tray_pusher_bwd_chk;
	int i_hs_fwd_stacker_tray_pusher_fwd_chk;
	int i_hs_fwd_stacker_tray_pusher_clamp_up_chk;
	int i_hs_fwd_stacker_tray_pusher_clamp_dn_chk;
	int i_hs_fwd_stacker_tray_guide_chk;

	// module 5
	int i_hs_fwd_stacker1_rail_tray_chk;
	int i_hs_fwd_stacker1_rail_fwd_chk;
	int i_hs_fwd_stacker1_rail_bwd_chk;
	int i_hs_fwd_stacker2_rail_tray_chk;
	int i_hs_fwd_stacker2_rail_fwd_chk;
	int i_hs_fwd_stacker2_rail_bwd_chk;
	int i_hs_fwd_stacker2_tray_clamp_on_chk;
	int i_hs_fwd_stacker2_tray_clamp_off_chk;

	//2012,0720
	int i_hs_fwd_clamp_on_chk;
	int i_hs_fwd_clamp_off_chk;
	int i_hs_bwd_clamp_on_chk;
	int i_hs_bwd_clamp_off_chk;

	// module 6
	int	o_hs_bwd_stacker_guide_clamp_onoff;
	int o_hs_bwd_stacker_guide_unclamp_onoff;
	int o_hs_bwd_stacker1_up_onoff;
	int o_hs_bwd_stacker1_dn_onoff;
	int o_hs_bwd_stacker2_up_onoff;
	int o_hs_bwd_stacker2_dn_onoff;
	int o_hs_bwd_stacker_tray_pusher_bwd_onoff;
	int o_hs_bwd_stacker_tray_pusher_fwd_onoff;
	int o_hs_bwd_stacker_tray_pusher_clamp_updn;
	int o_hs_bwd_stacker1_rail_fwd_onoff;
	int o_hs_bwd_stacker1_rail_bwd_onoff;
	int o_hs_bwd_stacker2_rail_fwd_onoff;
	int o_hs_bwd_stacker2_rail_bwd_onoff;
	int o_hs_bwd_stacker2_tray_clamp_onoff;

	// module 7
	int i_hs_bwd_stacker_guide_clamp_on_chk;
	int i_hs_bwd_stacker_guide_unclamp_off_chk;
	int i_hs_bwd_stacker1_up_chk;
	int i_hs_bwd_stacker1_dn_chk;
	int i_hs_bwd_stacker1_tray_chk;
	int i_hs_bwd_stacker1_updn_pos_chk;
	int i_hs_bwd_stacker2_up_chk;
	int i_hs_bwd_stacker2_dn_chk;
	int i_hs_bwd_stacker2_tray_chk;
	int i_hs_bwd_stacker2_updn_pos_chk;
	int i_hs_bwd_stacker_tray_pusher_bwd_chk;
	int i_hs_bwd_stacker_tray_pusher_fwd_chk;
	int i_hs_bwd_stacker_tray_pusher_clamp_up_chk;
	int i_hs_bwd_stacker_tray_pusher_clamp_dn_chk;
	int i_hs_bwd_stacker_tray_guide_chk;

	// module 10
	int i_hs_bwd_stacker1_rail_tray_chk;
	int i_hs_bwd_stacker1_rail_fwd_chk;
	int i_hs_bwd_stacker1_rail_bwd_chk;
	int i_hs_bwd_stacker2_rail_tray_chk;
	int i_hs_bwd_stacker2_rail_fwd_chk;
	int i_hs_bwd_stacker2_rail_bwd_chk;
	int i_hs_bwd_stacker2_tray_clamp_on_chk;
	int i_hs_bwd_stacker2_tray_clamp_off_chk;

	// module 11
	int o_uld_stacker_guide_clamp_onoff;
	int o_uld_stacker_guide_unclamp_onoff;
	int o_uld_stacker2_up_onoff;
	int o_uld_stacker2_dn_onoff;
	int o_uld_stacker1_up_onoff;
	int o_uld_stacker1_dn_onoff;
	int o_uld_stacker_tray_pusher_bwd_onoff;
	int o_uld_stacker_tray_pusher_fwd_onoff;
	int o_uld_stacker_tray_pusher_clamp_updn;
	int o_uld_stacker1_rail_fwd_onoff;
	int o_uld_stacker1_rail_bwd_onoff;
	int o_uld_stacker2_rail_fwd_onoff;
	int o_uld_stacker2_rail_bwd_onoff;
	int o_uld_stacker2_tray_clamp_onoff;
	
	// module 12
	int i_uld_stacker_guide_clamp_on_chk;
	int i_uld_stacker_guide_unclamp_off_chk;
	int i_uld_stacker2_up_chk;
	int i_uld_stacker2_dn_chk;
	int i_uld_stacker1_tray_chk;
	int i_uld_stacker2_updn_pos_chk;
	int i_uld_stacker1_up_chk;
	int i_uld_stacker1_dn_chk;
	int i_uld_stacker2_tray_chk;
	int i_uld_stacker1_updn_pos_chk;
	int i_uld_stacker_tray_pusher_bwd_chk;
	int i_uld_stacker_tray_pusher_fwd_chk;	
	int i_uld_stacker_tray_pusher_clamp_up_chk;
	int i_uld_stacker_tray_pusher_clamp_dn_chk;
	int i_uld_stacker_tray_guide_chk;

	// module 13
	int i_uld_stacker1_rail_tray_chk;
	int i_uld_stacker1_rail_fwd_chk;
	int i_uld_stacker1_rail_bwd_chk;
	int i_uld_stacker2_rail_tray_chk;
	int i_uld_stacker2_rail_fwd_chk;
	int i_uld_stacker2_rail_bwd_chk;
	int i_uld_stacker2_tray_clamp_on_chk;	
	int i_uld_stacker2_tray_clamp_off_chk;
	int i_uld_stacker_direction_chk;

	// module 14
	int o_ld_module_rbt_picker1_updn;			// s1400
	int o_ld_module_rbt_glipper1_onoff;			// s1401
	int o_ld_module_rbt_picker2_updn;			// s1402
	int o_ld_module_rbt_glipper2_onoff;			// s1403
	int o_ld_module_picker_updn[2];
	int o_ld_module_glipper_onoff[2];
	int o_front_hs_bcr_onoff;					// s1404
	int o_back_hs_bcr_onoff;					// s1405
	int o_left_clip_bcr_onoff;					// s1406
	int o_right_clip_bcr_onoff;					// s1407

	

	int i_ld_module_rbt_picker1_up_chk;			// ps1400
	int i_ld_module_rbt_picker1_dn_chk;			// ps1401
	int i_ld_module_rbt_glipper1_dvc_chk;		// ps1402
	int i_ld_module_rbt_picker2_up_chk;			// ps1403
	int i_ld_module_rbt_picker2_dn_chk;			// ps1404
	int i_ld_module_rbt_glipper2_dvc_chk;		// ps1405
	int i_ld_module_rbt_dvc_direction_chk;		// ps1406

	int i_ld_module_picker_up_chk[2];
	int i_ld_module_picker_dn_chk[2];
	int i_ld_module_dvc_chk[2];

	// module 15
	int o_hs_fwd_rbt_picker1_updn;
	int o_hs_fwd_rbt_picker1_gripper_onoff;
	int o_hs_fwd_rbt_picker2_updn;
	int o_hs_fwd_rbt_picker2_gripper_onoff;
	int o_hs_fwd_rbt_picker_picker_updn[2];
	int o_hs_fwd_rbt_glipper_onoff[2];

	int i_hs_fwd_rbt_picker1_up_chk;
	int i_hs_fwd_rbt_picker1_dn_chk;
	int i_hs_fwd_rbt_picker1_gripper_dvc_chk;
	int i_hs_fwd_rbt_picker2_up_chk;
	int i_hs_fwd_rbt_picker2_dn_chk;
	int i_hs_fwd_rbt_picker2_gripper_dvc_chk;
	int i_hs_fwd_rbt_direction_1;
	int i_hs_fwd_rbt_direction_2;
	int i_hs_fwd_picker_up_chk[2];
	int i_hs_fwd_picker_dn_chk[2];
	int i_hs_fwd_dvc_chk[2];
				
	// module 16
	int o_hs_bwd_rbt_picker1_updn;
	int o_hs_bwd_rbt_picker1_gripper_onoff;
	int o_hs_bwd_rbt_picker2_updn;
	int o_hs_bwd_rbt_picker2_gripper_onoff;
	int o_hs_bwd_rbt_picker_picker_updn[2];
	int o_hs_bwd_rbt_glipper_onoff[2];

	int i_hs_bwd_rbt_picker1_up_chk;
	int i_hs_bwd_rbt_picker1_dn_chk;
	int i_hs_bwd_rbt_picker1_gripper_dvc_chk;
	int i_hs_bwd_rbt_picker2_up_chk;
	int i_hs_bwd_rbt_picker2_dn_chk;
	int i_hs_bwd_rbt_picker2_gripper_dvc_chk;
	int i_hs_bwd_rbt_direction_1;
	int i_hs_bwd_rbt_direction_2;
	int i_hs_bwd_picker_up_chk[2];
	int i_hs_bwd_picker_dn_chk[2];
	int i_hs_bwd_dvc_chk[2];

	// ************************************************************
	//  // module 17 Tube Clip
	// ************************************************************
	int o_ld_tube_s_latch_fwdbwd_onoff;			// s1700 초기전진
	int o_tube_s_m_plate_fwd_onoff;
	int o_tube_s_m_plate_bwd_onoff;
	int o_tube_s_m_plate_short_updn_onoff;
	int o_tube_s_m_plate_long_updn_onoff;
	int i_ld_tube_s_latch_fwd_chk;
	int i_ld_tube_s_latch_bwd_chk;
	int i_ld_tube_s_cover_on_chk;
	int i_ld_tube_s_empty_chk; //B접점
	int i_ld_tube_s_full_chk;
	int i_tube_s_m_plate_fwd_chk;
	int i_tube_s_m_plate_bwd_chk;
	int i_tube_s_m_plate_short_up_chk;
	int i_tube_s_m_plate_short_dn_chk;
	int i_tube_s_m_plate_long_up_chk;
	int i_tube_s_m_plate_long_dn_chk;

	int o_tube_rotator_0_onoff;							//s2000
	int o_tube_rotator_90_onoff;
	int o_tube_rotator_clamp_onoff;
	int o_tube_rotator_unclamp_onoff;
	int o_tube_rotator_cap_remove_onoff;
	int o_tube_rotator_latch_fwd_onoff;
	int o_tube_rotator_latch_bwd_onoff;

	int i_tube_rotator_0_chk;
	int i_tube_rotator_90_chk;
	int i_tube_rotator_clamp_chk;
	int i_tube_rotator_unclamp_chk;
	int i_tube_rotator_cap_remove_on_chk;
	int i_tube_rotator_cap_remove_off_chk;
	int i_tube_rotator_latch_fwd_onoff;              //초기전진
	int i_tube_rotator_latch_bwd_onoff;
	

	int o_uld_tube_pusher_fwdbwd_onoff;				// s2100
	int o_uld_tube_s_updn_onoff;
	int o_clip_buffer_updn_onoff;

	int i_ld_tube_chk;
	int i_rotate_tube_chk;
	int i_uld_tube_s_up_chk;
	int i_uld_tube_s_dn_chk;
	int i_uld_tube_s_cover_on_chk;
	int i_uld_tube_chk;
	int i_uld_tube_s_full_chk;
	int i_clip_buff_up_chk;
	int i_clip_buff_dn_chk;
	int i_clip_buff_clip1_chk;
	int i_clip_buff_clip2_chk;

	// ************************************************************
	//  Work Buffer1,2,3,4
	// ************************************************************
	int	o_buf1_f_vacuum_onoff;							// s2200
	int	o_buf1_f_eject_onoff;							// s2201
	int	o_buf1_f_hs_onoff;								// s2201
	int o_buf1_b_vacuum_onoff;							// s2202
	int o_buf1_b_eject_onoff;							// s2203
	int	o_buf1_m_clamp_onoff;							// s2204
	int o_buf1_hs_clamp_onoff;							// s2205

	int	i_buf1_f_vacuum_chk;							// ps2200 8
	int i_buf1_b_vacuum_chk;							// ps2201
	int i_buf1_m_clamp_on_chk;							// ps2202
	int i_buf1_m_clamp_off_chk;							// ps2203
	int i_buf1_hs_clamp_on_chk;							// ps2204
	int i_buf1_hs_clamp_off_chk;						// ps2205
	int i_tube_push_fwd_chk;							// ps2206
	int i_tube_push_bwd_chk;							// ps2207


	int	o_buf2_f_vacuum_onoff;							// s2300
	int	o_buf2_f_eject_onoff;							// s2301
	int	o_buf2_f_hs_onoff;								// s2301
	int o_buf2_b_vacuum_onoff;							// s2302
	int o_buf2_b_eject_onoff;							// s2303
	int	o_buf2_m_clamp_onoff;							// s2304
	int o_buf2_hs_clamp_onoff;							// s2305

	int	i_buf2_f_vacuum_chk;							// ps2300 8
	int i_buf2_b_vacuum_chk;							// ps2301
	int i_buf2_m_clamp_on_chk;							// ps2302
	int i_buf2_m_clamp_off_chk;							// ps2303
	int i_buf2_hs_clamp_on_chk;							// ps2304
	int i_buf2_hs_clamp_off_chk;						// ps2305

	int	o_buf3_f_vacuum_onoff;							// s2400
	int	o_buf3_f_eject_onoff;							// s2401
	int	o_buf3_f_hs_onoff;								// s2401
	int o_buf3_b_vacuum_onoff;							// s2402
	int o_buf3_b_eject_onoff;							// s2403
	int	o_buf3_m_clamp_onoff;							// s2404
	int o_buf3_hs_clamp_onoff;							// s2405
	int o_clip_buffer_align_onoff;						// s2406  //add 2012,0720

	int	i_buf3_f_vacuum_chk;							// ps2400 8
	int i_buf3_b_vacuum_chk;							// ps2401
	int i_buf3_m_clamp_on_chk;							// ps2402
	int i_buf3_m_clamp_off_chk;							// ps2403
	int i_buf3_hs_clamp_on_chk;							// ps2404
	int i_buf3_hs_clamp_off_chk;						// ps2405
	int i_clip_buffer_align_on_chk;						// ps2406 //add 2012,0720
	int i_clip_buffer_align_off_chk;					// ps2407

	int	o_buf4_f_vacuum_onoff;							// s2500
	int	o_buf4_f_eject_onoff;							// s2501
	int	o_buf4_f_hs_onoff;								// s2501
	int o_buf4_b_vacuum_onoff;							// s2502
	int o_buf4_b_eject_onoff;							// s2503
	int	o_buf4_m_clamp_onoff;							// s2504
	int o_buf4_hs_clamp_onoff;							// s2505
	int o_vlp_bcr_clamp_onoff;							// s2506 //add 2012,0720

	int	i_buf4_f_vacuum_chk;							// ps2500 8
	int i_buf4_b_vacuum_chk;							// ps2501
	int i_buf4_m_clamp_on_chk;							// ps2502
	int i_buf4_m_clamp_off_chk;							// ps2503
	int i_buf4_hs_clamp_on_chk;							// ps2504
	int i_buf4_hs_clamp_off_chk;						// ps2505
	int i_vlp_bcr_clamp_on_chk;							// ps2506 //add 2012,0720
	int i_vlp_bcr_clamp_off_chk;						// ps2507
	
	// ************************************************************
	//  Clip Robot
	// ************************************************************
	int o_clip_rbt_updn_onoff;							// s2600
	int o_clip_remove_up_onoff;							// s2601
	int o_clip_remove_down_onoff;						// s2602

	int o_ldclip_tube_updn_onoff;
	int o_uldclip_tube_updn_onoff;
	
	int i_clip_rbt_up_chk;								// ps2600
	int i_clip_rbt_dn_chk;
	int i_vision1_module_chk;
	int i_vision2_module_chk;
	int i_ldclip_tube_up_chk;
	int i_ldclip_tube_dn_chk;
	int i_uldclip_tube_up_chk;
	int i_uldclip_tube_dn_chk;

	// ************************************************************
	//  Picker Robot
	// ************************************************************
	int o_picker_rbt_updn_onoff;							// s2700
	int o_picker_rbt_gripper_onoff;							
	int o_sorter_rbt_updn_onoff;
	int o_sorter_rbt_gripper_onoff;
	int o_uld_picker_rbt_updn_onoff;							// s2700
	int o_uld_picker_rbt_gripper_onoff;							
	int o_sorter_picker_rbt_updn_onoff;
	int o_sorter_picker_rbt_gripper_onoff;

	
	int i_picker_rbt_up_chk;								// ps2700
	int i_picker_rbt_dn_chk;
	int i_picker_rbt_dvc_chk;
	int i_sorter_rbt_up_chk;
	int i_sorter_rbt_dn_chk;
	int i_sorter_rbt_dvc_chk;

	int i_uld_picker_rbt_up_chk;								// ps2700
	int i_uld_picker_rbt_dn_chk;
	int i_uld_picker_rbt_dvc_chk;
	int i_sorter_picker_rbt_up_chk;
	int i_sorter_picker_rbt_dn_chk;
	int i_sorter_picker_rbt_dvc_chk;
	int i_safety_door[3];

	// ************************************************************
	//  Lavel Attach
	// ************************************************************
	int o_label_attach_vacuum_onoff;					// s3000
	int o_label_attach_eject_onoff;
	int o_label_attach_unit_fwd_onoff;
	int o_label_attach_unit_bwd_onoff;

	int i_print_label_chk;								// ps3000
	int i_label_attach_vacuum_on_chk;
	int i_label_attach_unit_fwd_chk;
	int i_label_attach_unit_bwd_chk;
	int i_reject_tray_chk;

	int i_vision1_safety_up_chk;
	int i_vision1_safety_dn_chk;
	int i_vision2_safety_up_chk;
	int i_vision2_safety_dn_chk;

	int o_vis_buff1_rotator_onoff;						// s3100
	int o_vis_buff1_clamp_onoff;
	int o_vis_buff2_rotator_onoff;
	int o_vis_buff2_clamp_onoff;
	int o_ionizer_onoff; //add 2012,0720

	//2012.0629 james 
	int o_vis_buffer_clamp_onoff[2];
	int o_vis_buffer_rotator_onoff[2];

	int i_vis_buff1_rotator_on_chk;
	int i_vis_buff1_rotator_off_chk;
	int i_vis_buff1_clamp_on_chk;
	int i_vis_buff1_clamp_off_chk;
	int i_vis_buff2_rotator_on_chk;
	int i_vis_buff2_rotator_off_chk;
	int i_vis_buff2_clamp_on_chk;
	int i_vis_buff2_clamp_off_chk;
	int i_ionizer_err_chk; //add 2012,0720

	//2012.0629 james 
	int i_vis_buff_rotator_on_chk[2];
	int i_vis_buff_rotator_off_chk[2];
	int i_vis_buff_clamp_on_chk[2];
	int i_vis_buff_clamp_off_chk[2];

	int o_vis_reset1_onoff;							// s3200
	int o_vis_reset2_onoff;
	int o_vis_trigger1_onoff;
	int o_vis_trigger2_onoff;
	int o_bcr_print_feed;

	int i_vis_ready1_chk;
	int i_vis_ready2_chk;
	int i_vis_ready3_chk;
	int i_vis_ready4_chk;
	int i_bcr_print_ribbon_row;
	int i_bcr_print_servide_required;
	int i_bcr_print_end_print;
	int i_bcr_print_media_out;
	int i_bcr_print_ribbon_out;
	int i_bcr_print_data_ready;

	// ************************************************************
	//  Switch & Tower Lamp
	// ************************************************************
	int o_start_switch_lamp;							// s3300
	int o_stop_switch_lamp;								// s3301
	int o_alarm_reset_switch_lamp;						// s3302	
	int o_buzzer_off_switch_lamp;						// s3303
	
	int i_start_switch_chk;								// PS3300
	int i_stop_switch_chk;								// PS3301
	int i_alarm_reset_switch_chk;						// PS3302
	int i_buzzer_switch_chk;							// PS3303
	int i_auto_mode_switch_chk; 						// PS3304
	int i_manual_mode_switch_chk;						// PS3305
	int i_front_select_switch_chk;						// PS3306
	int i_rear_select_switch_chk;						// PS3307
	// ************************************************************
	int o_main_air;										// S3400
	int o_towerlamp_green;								// S3401
	int o_towerlamp_yellow;								// S3402
	int o_towerlamp_red;								// S3403
	int o_buzzer_1;										// s3404
	int o_buzzer_2;										// s3405
	int o_lock_door_onoff;								// s3406 All door
	int o_buzzer[2];

	int i_main_air_chk;									// PS3400
	int i_btm_door1_chk;								// PS3401
	int i_btm_door2_chk;								// PS3402
	int i_btm_door3_chk;								// PS3403
	int i_btm_door4_chk;								// PS3404
	int i_btm_door5_chk;								// PS3405
	int i_btm_door6_chk;								// PS3406
	int i_btm_door7_chk;								// PS3407

	int i_top_door1_chk;								// PS3500
	int i_top_door2_chk;
	int i_top_door3_chk;
	int i_top_door4_chk;
	int i_top_door5_chk;
	int i_top_door6_chk;
	int i_top_door7_chk;
	int i_top_door8_chk;
	int i_top_door9_chk;
	int i_top_door10_chk;
	int i_top_door11_chk;
	int i_top_door12_chk;
	int i_top_door13_chk;
	int i_top_door14_chk;
	int i_top_door[14];

	int i_mc1_chk; //MOTOR CHECK
	int i_mc2_chk;

	int i_emo1_chk;
	int i_emo2_chk;
	int i_emo3_chk;
	int i_emo4_chk;

	int i_stacker_light_curtain_chk;
	int i_vision_light_curtain_chk;

	int i_vision_ready1;								// PS3700
	int i_vision_ready2;
	int i_vision_complste1;
	int i_vision_passfail1; //0:PASS 1:FAIL
	int i_vision_complste2;
	int i_vision_passfail2; //0:PASS 1:FAIL
	int i_vision_yield1;
	int i_vision_yield12;
	int i_vision_error1;
	int i_vision_error2;

	//2012,0821 add
	int i_m_ld_slide_tray_chk;
	int i_m_ld_slide_onoff_chk;
	int i_m_uld_slide_tray_chk;
	int i_m_uld_slide_onoff_chk;

	int i_hs_fwd_ld_slide_tray_chk;
	int i_hs_fwd_ld_slide_onoff_chk;
	int i_hs_fwd_uld_slide_tray_chk;
	int i_hs_fwd_uld_slide_onoff_chk;
	
	int i_hs_bwd_ld_slide_tray_chk;
	int i_hs_bwd_ld_slide_onoff_chk;
	int i_hs_bwd_uld_slide_tray_chk;
	int i_hs_bwd_uld_slide_onoff_chk;

	int i_uld_ld_slide_tray_chk;
	int i_uld_ld_slide_onoff_chk;
	int i_uld_uld_slide_tray_chk;
	int i_uld_uld_slide_onoff_chk;


	int n_io_in[MAX_IN];	// 전체 인 포트 I/O 상태 저장 변수
	int n_io_out[MAX_IO];	// 전체 아웃 포트 I/O 상태 저장 변수
	int n_io_slave_in[15];	// IO SLAVE의 IN 갯수
	int n_io_slave_out[15];	// IO SLAVE의 OUT 갯수
} ;
extern  st_io_param  st_io;
// ******************************************************************************

// *************************************************************************
// I/O 동작 대기 시간 및 리미트 시간 저장 구조체 선언                            
// *************************************************************************
#define MAX_WAIT_TIME 95//40

struct st_io_wait_param 
{	
	// **********************************************************
	//  Robot Module Pick UpDn                                      
	// **********************************************************
	int i_picker_updn_w[2];//[0]up [1]dn
	int i_picker_l;
	int i_finger_onoff_w[2];//[0]on [1]off
	int i_finger_l;

	int i_module_clamp_w[2];
	int i_module_l;

	int i_stacker_guide_w[2];
	int i_stacker_guide_l;

	int i_stacker_f_b_w[2];
	int i_stacker_f_b_l;

	int i_stacker_rail_push_back_w[2];
	int i_stacker_rail_push_back_l;

	int i_stacker_updn_w[2];
	int i_stacker_updn_l;
	
	int i_hs_stacker_guide_w[2];
	int i_hs_stacker_guide_l;
	
	int i_stacker_clip_w[2];
	int i_stacker_clip_l;

	int i_label_onoff_w[2];
	int i_label_l;

	int i_label_vacuum;

	int i_label_eject;

	//2012.0629 james 추가 
	int i_vision_ratator_w[2];
	int i_vision_ratator_l;

	int	n_on_wait_time[MAX_WAIT_TIME];
	int n_off_wait_time[MAX_WAIT_TIME];
	int n_limit_wait_time[MAX_WAIT_TIME];


};
extern  st_io_wait_param  st_wait;

struct st_lot_database
{
	int				mndb;
	
	CString			mstrlot_no;
	COleDateTime	mstart_date;
	COleDateTime	mend_date;
	
	int				mnTotal_count;
	int				mnPass_count;
	int				mnFail_count;	
	float			mfYield;
};
extern struct st_lot_database st_lotdb;

struct st_time_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	
	CString			mstrlot_no;
	CString			mstrpart_no;
	
	int				mnTime_Section;
	int				mnTime_Time;
	int				mnMode;
	
	int				mnSectionBuffer;
};
extern struct st_time_database st_timedb;

struct st_alarm_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	
	CString			mstrlot_no;
	CString			mstrpart_no;
	
	CString			mstrAlarmCode;
	CString			mstrAlarmMsg;
};
extern struct st_alarm_database st_alarmdb;

struct st_ground_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	
	CString			mstrChannel[5];
};
extern struct st_ground_database st_grounddb;

struct st_buffer_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	CString			mstrlot_no;
	CString			mstrpart_no;
	int				mnbuffer_pos;
	int				mnlast_module;
	int				mnbypass;
	int				mnmodule_cnt;
	int				mnretest;
};
extern struct st_buffer_database		st_bufferdb;

struct st_buffer_tray_param
{
// 	char cModuleData[180][50];
// 
// 	int nDeviceExistence[180];
// 	int nTestResult[180];
// 	int nArrayInfo[180];
// 	char cSerialNumber[180][20];
// 	char cTestResult[180][20];
// 	char cArrayInfo[180][20];
// 	char cLotNumber[180][20];
// 	int nRetestCount[180];

	int nDeviceExistence[4][3][100];
	char cModuleData[3][90][50];
	int nArrayInfo[3][90];
	char cSerialNumber[3][90][20];
	char cLotNumber[3][90][20];
	char cTestResult[3][90][20];
	char cArrayInfo[3][90][20];

	char cTotalLotNumber[40];
	char cTotalPartNumber[40];
	char cModuleCnt[5];
	int nRetestMdlCnt[2];			// 0 : HTO, BTO, 1: NTO
	int nLastModule;
	int nByPass;

	int nTrayExistence;
};
extern st_buffer_tray_param	st_buffer_tray[MAX_SITE_INFO];

#define BT_REAR_BUFFER			0
#define BT_FRONT_BUFFER			1
#define BT_PICKER				2
#define BT_RETEST_BUFFER		3
#define BT_REJECT_BUFFER		4
#define BT_LOB_BUFFER			5
#define BT_TRAY_1				6
#define BT_TRAY_2				7
#define BT_CUSTOMER_BUFFER		8
#define BT_AIRBLOW_BOX			9
#define BT_LENGTH_VARIABLE		10
#define BT_SAFETY_ZONE			11

#define RACK_FSITE1_L		0
#define RACK_FSITE1_R		1
#define RACK_FSITE2_L		2
#define RACK_FSITE2_R		3
#define RACK_RSITE1_L		4
#define RACK_RSITE1_R		5
#define RACK_RSITE2_L		6
#define RACK_RSITE2_R		7

#define RACKSTATUS_READY			0		// 준비 상태, Power 들어오기 전
#define RACKSTATUS_POWER_CHECK		1		// Power를 넣었다. Power 켜지는지 체크
#define RACKSTATUS_BOOT_CHECK		2		// Power가 켜졌다. 부팅이 되는지 체크
#define RACKSTATUS_TESTEND_CHECK	3		// 부팅이 됐다. Test End 체크

#define BOARD_POWER_CHECK			1
#define BOARD_POWER_ON				2
#define BOARD_BOOT_CHECK			3
#define BOARD_TESTEND_CHECK			4

#define RACKWORK_READY			0		// 준비 상태
#define RACKWORK_POWER_ON		1		// Power를 켜라.
#define RACKWORK_POWER_OFF		2		// Power를 꺼라.

// 0: SITE1, 1: SITE2, 2: SITE3, 3: SITE4, 4: SITE5, 5: SITE6, 6: SITE7, 7: SITE8


/////////////////////////////////////// 20120502 유성준
#define MODULE_LOAD      			0
#define MODULE_LOAD_ROBOT  			1
#define WORK_BUFFER1				2
#define WORK_BUFFER2				3
#define WORK_BUFFER3				4
#define WORK_BUFFER4				5
#define WORK_BUFFER5				6
#define HEATSINK_LEFT               7
#define HEATSINK_LEFT_ROBOT         8
#define HEATSINK_RIGHT       		9
#define HEATSINK_RIGHT_ROBOT		10
#define SORTING_ROBOT		     	11
#define MODULE_CLAMP_ROBOT			12
#define CLIP_INSERT_ROBOT			13
#define BARCODE						14
#define PICKER_ROBOT		     	15
#define ANISTART					16
#define ANIINITIAL					17
#define M_TRAY_Z					18

#define READY_POS					0
#define MOVE_POS1					1
#define MOVE_POS2					2
#define MOVE_POS3					3
#define MOVE_POS4					4
#define MOVE_POS5					5
#define MOVE_POS6					6

#define READY_POS1					7
#define MOVE_POS11					8
#define MOVE_POS12					9
#define MOVE_POS13					10
#define MOVE_POS14					11
#define MOVE_POS15					12

#define READY_POS2					13
#define MOVE_POS21					14
#define MOVE_POS22					15
#define MOVE_POS23					16
#define MOVE_POS24					17
#define MOVE_POS25					18

#define READY_POS3					19
#define MOVE_POS31					20
#define MOVE_POS32					21
#define MOVE_POS33					22
#define MOVE_POS34					23
#define MOVE_POS35					24

#define READY_POS4					25
#define MOVE_POS41					26
#define MOVE_POS42					27
#define MOVE_POS43					28
#define MOVE_POS44					29
#define MOVE_POS45					30

///////////////////////////////////////

struct st_part_motor_info
{
	int				n_part_cnt;
	int				n_part_axis_cnt[20];
	int				n_part_axis[20][10];
	
	int				n_axis_pos_num[20][10];
	int				n_axis_minus_el[20][10];
	int				n_axis_plus_el[20][10];
	int				n_axis_home[20][10];
	int				n_axis_sd[20][10];
	int				n_axis_direction[20][10];
	int				n_axis_account[20][10];
	
	CString			str_part_info[20];
	CString			str_part_name[20];
	
	CString			str_part_axis_name[20][10];
	CString			str_axis_pos_name[20][10][25];
	CString			str_axis_pos_info[20][10][25];
};
extern struct st_part_motor_info st_motor_info;

struct st_part_info
{
	int			n_part_cnt;
	int			n_part_info;
	int			n_motor_axis_cnt[20];
	int			n_motor_axis[20][10];
	int			n_io_input_cnt;
	int			n_io_output_cnt;
	int			n_io_input[32];
	int			n_io_output[32];
	
	CString		str_part_name[20];
	CString		str_part_info[20];
	CString		str_motor_axis[20][10];
	CString		str_io_input[32];
	CString		str_io_output[32];
};
extern struct st_part_info st_part;

struct st_client_param
{
	CString		str_ip;			// ip address......
	
	int			n_port;			// port.....
	int			n_connect;		// client 연결상태......
	int			n_send_length;  // 송신 문자 길이......
	int			n_rev_length;	// 수신 문자 길이......
	int         n_client_send_flag;

	int			n_rev_info;		// client 종료 방법..... (0:바로 1: 수신후)

	char		ch_rev[8192];	// 수신내용....
	char		ch_send[8192];  // 송신내용....
	int         n_client_no;	// (0:라벨, 1:삼성)

	CString		str_rev;
	CString     str_send;
};
extern st_client_param st_client[10];

struct st_server_param
{
	CString		str_ip;			// ip address......
	
	int			n_port;			// port.....
	int			n_connect;		// client 연결상태......
	int			n_send_length;  // 송신 문자 길이......
	int			n_rev_length;	// 수신 문자 길이......
	
	int			n_rev_bcr;		//20120514 유성준

	char		ch_rev[8192];	// 수신내용....
	char		ch_send[8192];  // 송신내용....
};
extern st_server_param st_server[10];

struct st_barcode_param
{
	int		iScanningOnFlag;		//바코드 스케닝 중임을 알리기 위함
	int		iScanningMethod;		//스케닝 방식을 설정 (fly, step..)

	int		i_read_work_pos;

	CString strBarcode[4];
	BOOL b_barcode_first_load;	//20120604


	////////////////////////// 20120620
	CString str_barcode_print[100];	
	int n_barcode_hs_cnt;	
	int n_barcode_hs_paper_out_error;
	int n_barcode_hs_pause_error;
	int n_barcode_hs_head_up_error;
	int n_barcode_hs_ribbon_out_error;
	int n_barcode_hs_print_mode;	//0:rewind, 1:peel-off, 2:tear-off
	//////////////////////////

	int b_print_complete;	//20120630 라벨 프린터 출력 완료 상태 확인
	int n_Prt_Comp_Reply;
	int n_Prt_mode;

	BOOL b_Maunal_Input;//20120913
};
extern st_barcode_param st_barcode;

/////////////////////////////////////// 20120618
struct st_interface_param
{
	CString str_bcr_ml_name;
	int n_bcr_ml_mode;
	CString str_hs_ml_name;
	int n_hs_ml_mode;
	BOOL b_ml_select;

	BOOL b_vision_ml_select;	//20120625
	BOOL b_hs_ml_select;	//20120625

};
extern st_interface_param st_interface;
/////////////////////////////////////// 



struct st_data_param
{
	int		n_load_rev_cnt[1000];        // data input count.....
	int		n_dvc_no[1000];				// count 정보 변수....
	int		n_snd_dvc_no[1000];			// count 정보 변수....
	int		n_lotend[1000];
	int		n_lotdel[1000];
	int		n_lot_ready[1000];

	int		n_unload_count[2];             //0:Good 1: Error Lot당 생산되는 Lot의 개수
	int		n_sendlot_count;

	int		n_lotcount;					//앞장비에서 보내준 Lot을 Count한다.
	int     n_lot_total[1000];
	int		n_rcv_dvc_no[1000];			// count 정보 변수....
	int		n_load_count[1000];
	int		n_lotupdate;
	int		n_lotdelete;
	int		n_rfidlot_count;
	
	CString m_str_lot_id[1000][50];			// LOT_ID 입력 변수..... 
	CString m_str_maz_id[1000][50];			// maz id 입력 변수......
	CString m_str_part_id[1000][50];			// part id 입력 변수......
	CString m_str_rej_id[1000][50];			// part id 입력 변수......
	CString m_str_bcr_id[1000][50];			// part id 입력 변수......
	CString m_str_pkg_code[1000][50];			// count 정보 변수....
	CString	m_str_step[1000];

	int     n_backuplot_count;
	int     n_backupdvc_count;
	int		n_backup_dvc_no[1000];

	int		n_sgn_start;
	int		n_sgn_end;

	CString str_lot_id;
	CString str_part_no;
	CString str_ready_msg;

	float	f_pressure[16];					// 압력 데이터 저장 변수..... 
	float	f_sndpressure[16];				// 압력 데이터 저장 변수..... 
	float	f_oldWpressure[16];				// 압력 데이터 저장 변수..... 

	int	    m_Tray_pos;
	char    ch_lot_id[MOVE_TRAY_POS][100];			// lot id.....
	char    ch_part_no[MOVE_TRAY_POS][100];		// part no....
	char    ch_dev_no[MOVE_TRAY_POS][100][100];	// tray serial module.....
	int     n_module_no[MOVE_TRAY_POS];

	float	f_temp;

	CTime m_tbathcleanOldRef[CLEAN_PARA];
	CTime m_tbathcleantestRef[CLEAN_PARA];

	CTime m_tbathdryOldRef[CLEAN_PARA];
	CTime m_tbathdrytestRef[CLEAN_PARA];

	CTime m_tdrytestRef[DRY_PARA];
	CTime m_tolddryRef[DRY_PARA];

	int     n_cleanwork[CLEAN_PARA];
	int		n_clean_time[4];		// cleaning time......
	int		n_oldclean_time[4];		// cleaning time......
	int		n_dry_time[3];			// dry time.....
	int		n_olddry_time[3];			// dry time.....
	int		n_waterair_count[4];
	int		n_air_count[4];

	int		n_DryWork[DRY_PARA];


	int n_handler_recovery_flag;

	CTime m_tcleanwholeRef;
	CTime m_tcleanRef[CLEAN_PARA];
	CTime m_tcleantestRef[CLEAN_PARA];
	CTime m_toldcleanRef[CLEAN_PARA];

	int		n_CleanerWork;
	int		n_DryerWork;

	//RFID
	int n_rfid_manual_write;
	CString str_rfid_lotid;
	CString str_rfid_partid;
	CString str_rfid_pkg;
	CString str_rfid_step;
	int n_rfid_frame;

	//BARCODE
	CString str_sndBarcode;
	int		n_Countuph;
	float	mf_uphaverage;
	
};
extern struct st_data_param st_data;
////////////////////////////////////// 20120711
struct st_worker_db
{
	CTime		time;

	CString		str_worker_name;	// 작업자 번호.....
	CString		str_worker_crem;	// 작업자 조......
	CString		str_worker_no;		// 사원 번호......

};
//////////////////////////////////////

struct st_jamcode_info
{
	CString		str_descrip[1000000];
	CString 	str_jamcode[1000000];
	int			n_process_time[1000000];
	int			n_alart_cnt[1000000];
};

extern struct st_jamcode_info st_jamcode;

typedef struct st_MDRbt{               
	
	int n_site;

	struct st_MDRbt *next;

}st_MDRbt;

extern st_MDRbt *MDhead;
extern st_MDRbt *MDtail;

typedef struct st_LHSRbt{               
	
	int n_site;
	
	struct st_LHSRbt *next;
	
}st_LHSRbt;

extern st_LHSRbt *LHShead;
extern st_LHSRbt *LHStail;

typedef struct st_RHSRbt{

	int n_site;
	
	struct st_RHSRbt *next;
	
}st_RHSRbt;

extern st_RHSRbt *RHShead;
extern st_RHSRbt *RHStail;

typedef struct st_CLIPRbt{               
	
	int n_site;
	
	struct st_CLIPRbt *next;
	
}st_CLIPRbt;

extern st_CLIPRbt *CLIPhead;
extern st_CLIPRbt *CLIPtail;

typedef struct st_ULDRbt{     
	
	int n_site;
	
	struct st_ULDRbt *next;
	
}st_ULDRbt;

extern st_ULDRbt *ULDhead;
extern st_ULDRbt *ULDtail;

struct st_ani_param 
{
	double dAniEncoder[MAXMOTOR];
	int nSelectMotPos[MAXMOTOR];
	BOOL bSelectCylinder[99];
};
extern  st_ani_param  st_ani;

#endif
