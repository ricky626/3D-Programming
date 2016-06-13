// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#pragma warning( disable: 4005)
// 아래 지정된 플랫폼에 우선하는 플랫폼을 대상으로 하는 경우 다음 정의를 수정하십시오.
// 다른 플랫폼에 사용되는 해당 값의 최신 정보는 MSDN을 참조하십시오.
#ifndef WINVER				// Windows XP 이상에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0501		// 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT		// Windows XP 이상에서만 기능을 사용할 수 있습니다.                   
#define _WIN32_WINNT 0x0501	// 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 이상에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINDOWS 0x0410 // Windows Me 이상에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_IE			// IE 6.0 이상에서만 기능을 사용할 수 있습니다.
#define _WIN32_IE 0x0600	// 다른 버전의 IE에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <Windows.h>
// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <Mmsystem.h>
#include <math.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <d3d9types.h>

#define RANDOM_COLOR	(0xFF000000 | ((rand() * 0xFFFFFF) / RAND_MAX))

#define VS_SLOT_VIEWPROJECTION_MATRIX	0x00
#define VS_SLOT_WORLD_MATRIX			0x01
#define PS_SLOT_COLOR	0x00

#define FIRST_PERSON_CAMERA	0x01
#define SPACESHIP_CAMERA	0x02
#define THIRD_PERSON_CAMERA	0x03

#define FRAME_BUFFER_WIDTH	640
#define FRAME_BUFFER_HEIGHT	480
#define ASPECT_RATIO	(float(FRAME_BUFFER_WIDTH)/float(FRAME_BUFFER_HEIGHT))

#define PS_SLOT_LIGHT		0x00
#define PS_SLOT_MATERIAL	0x01
// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
enum {
	TANK_OBJECT0,
	TANK_OBJECT1,
	TANK_OBJECT2,
	TANK_OBJECT3,
	TANK_OBJECT4,
	TANK_OBJECT5,
	TANK_OBJECT6,
	TANK_OBJECT7,
	TANK_OBJECT8,
	TANK_OBJECT9,
	MAP_OBJECT_X0,
	MAP_OBJECT_X1,
	MAP_OBJECT_Y0,
	MAP_OBJECT_Y1,
	MAP_OBJECT_Z0,
	MAP_OBJECT_Z1,
	WALL_OBJECT0,
	WALL_OBJECT1,
	WALL_OBJECT2,
	WALL_ROTATTE_OBJECT1,
	WALL_ROTATTE_OBJECT2,
	WALL_ROTATTE_OBJECT3,
	OBJECT_END,
};
