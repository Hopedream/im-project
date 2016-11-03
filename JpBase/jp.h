#ifndef __JP_H__
#define __JP_H__
#pragma once


#ifdef DLL_EXPORT
#define PUBLIC_API __declspec(dllexport)
#else
#define PUBLIC_API __declspec(dllimport)
#endif

#define JP_MAX_BUFF		9999999

// Windows 头文件: 
#include <iostream>
#include <map>
#include <list>
#include <string>

#include <windows.h>
#include <objbase.h>
#include <zmouse.h>
#include <exdisp.h>
#include <comdef.h>

// 包含DUILib头文件
#include "UIlib.h"
using namespace DuiLib;
#include "JPUtils.h"
#include "JpStrHelper.h"
#include "JPNotifyHandle.h"

#endif 