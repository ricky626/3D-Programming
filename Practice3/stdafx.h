// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once
#pragma warning( disable: 4005)
// �Ʒ� ������ �÷����� �켱�ϴ� �÷����� ������� �ϴ� ��� ���� ���Ǹ� �����Ͻʽÿ�.
// �ٸ� �÷����� ���Ǵ� �ش� ���� �ֽ� ������ MSDN�� �����Ͻʽÿ�.
#ifndef WINVER				// Windows XP �̻󿡼��� ����� ����� �� �ֽ��ϴ�.
#define WINVER 0x0501		// �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#ifndef _WIN32_WINNT		// Windows XP �̻󿡼��� ����� ����� �� �ֽ��ϴ�.                   
#define _WIN32_WINNT 0x0501	// �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 �̻󿡼��� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_WINDOWS 0x0410 // Windows Me �̻� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#ifndef _WIN32_IE			// IE 6.0 �̻󿡼��� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_IE 0x0600	// �ٸ� ������ IE�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <Windows.h>
// C�� ��Ÿ�� ��� �����Դϴ�.
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
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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
