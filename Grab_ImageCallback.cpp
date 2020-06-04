#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <atomic>
#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>
#include "HighResClock.hpp"
#include "MvCameraControl.h"

bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo) {
    if (NULL == pstMVDevInfo) {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
        return false;
    }
    if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE) {
        printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chModelName);
    } else {
        printf("Not support.\n");
    }

    return true;
}

int main() {
    int nRet = MV_OK;

    void* handle = NULL;

    MV_CC_DEVICE_INFO_LIST stDeviceList;
    memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // 枚举设备
    // enum device
    nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
    if (MV_OK != nRet) {
        printf("MV_CC_EnumDevices fail! nRet [%x]\n", nRet);
        return 1;
    }
    if (stDeviceList.nDeviceNum > 0) {
        for (int i = 0; i < stDeviceList.nDeviceNum; i++) {
            printf("[device %d]:\n", i);
            MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
            if (NULL == pDeviceInfo) {
                return 1;
            }
            PrintDeviceInfo(pDeviceInfo);
        }
    } else {
        printf("Find No Devices!\n");
        return 1;
    }

    printf("Please Intput camera index: ");
    unsigned int nIndex = 0;
    scanf("%d", &nIndex);

    if (nIndex >= stDeviceList.nDeviceNum) {
        printf("Intput error!\n");
        return 1;
    }

    while (true) {
        // 选择设备并创建句柄
        // select device and create handle
        std::cout << "________________________________________" << std::endl;
        std::cout << "MV_CC_CreateHandle" << std::endl;
        nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
        if (MV_OK != nRet) {
            std::cerr << "MV_CC_CreateHandle fail! nRet" << nRet << std::endl;
        }

        // 打开设备
        // open device
        std::cout << "MV_CC_OpenDevice" << std::endl;
        nRet = MV_CC_OpenDevice(handle);
        if (MV_OK != nRet) {
            std::cerr << "MV_CC_OpenDevice fail! nRet" << nRet << std::endl;
        }

        std::cout << "UserSetSelector" << std::endl;
        nRet = MV_CC_SetEnumValue(handle, "UserSetSelector", 0 /*default*/);
        if (MV_OK != nRet) {
            std::cerr << "UserSetSelector fail! nRet" << nRet << std::endl;
        }

        std::cout << "UserSetLoad" << std::endl;
        nRet = MV_CC_SetCommandValue(handle, "UserSetLoad");
        if (MV_OK != nRet) {
            std::cerr << "UserSetLoad fail! nRet" << nRet << std::endl;
        }

        std::cout << "UserSetDefault" << std::endl;
        nRet = MV_CC_SetEnumValue(handle, "UserSetDefault", 0 /*default*/);
        if (MV_OK != nRet) {
            std::cerr << "UserSetDefault fail! nRet" << nRet << std::endl;
        }

        std::cout << "ExposureAuto" << std::endl;
        nRet = MV_CC_SetEnumValue(handle, "ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
        if (MV_OK != nRet) {
            std::cerr << "ExposureAuto fail! nRet" << nRet << std::endl;
        }

        std::cout << "ExposureTime" << std::endl;
        nRet = MV_CC_SetFloatValue(handle, "ExposureTime", 200);
        if (MV_OK != nRet) {
            std::cerr << "ExposureTime fail! nRet" << nRet << std::endl;
        }

        std::cout << "TriggerMode" << std::endl;
        nRet = MV_CC_SetEnumValue(handle, "TriggerMode", false);
        if (MV_OK != nRet) {
            std::cerr << "TriggerMode fail! nRet" << nRet << std::endl;
        }

        std::cout << "TriggerSource" << std::endl;
        nRet = MV_CC_SetEnumValue(handle, "TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);
        if (MV_OK != nRet) {
            std::cerr << "TriggerSource fail! nRet" << nRet << std::endl;
        }

        std::cout << "AcquisitionMode" << std::endl;
        nRet = MV_CC_SetEnumValue(handle, "AcquisitionMode", MV_ACQ_MODE_CONTINUOUS);
        if (MV_OK != nRet) {
            std::cerr << "AcquisitionMode fail! nRet" << nRet << std::endl;
        }

        std::cout << "AcquisitionFrameRateEnable" << std::endl;
        nRet = MV_CC_SetBoolValue(handle, "AcquisitionFrameRateEnable", true);
        if (MV_OK != nRet) {
            std::cerr << "AcquisitionFrameRateEnable fail! nRet" << nRet << std::endl;
        }

        std::cout << "AcquisitionFrameRate" << std::endl;
        nRet = MV_CC_SetFloatValue(handle, "AcquisitionFrameRate", 1000);
        if (MV_OK != nRet) {
            std::cerr << "AcquisitionFrameRate fail! nRet" << nRet << std::endl;
        }

        std::cout << "Height" << std::endl;
        nRet = MV_CC_SetIntValue(handle, "Height", 104);
        if (MV_OK != nRet) {
            std::cerr << "Height fail! nRet" << nRet << std::endl;
        }

        std::cout << "Width" << std::endl;
        nRet = MV_CC_SetIntValue(handle, "Width", 2592);
        if (MV_OK != nRet) {
            std::cerr << "Width fail! nRet" << nRet << std::endl;
        }

        std::cout << "PixelFormat" << std::endl;
        nRet = MV_CC_SetEnumValue(handle, "PixelFormat", PixelType_Gvsp_Mono8);
        if (MV_OK != nRet) {
            std::cerr << "PixelFormat fail! nRet" << nRet << std::endl;
        }

        struct ClockAndMem {
            ClockAndMem() : clock("clock", 1000) {}
            unsigned char buffer[104 * 2592];
            HighResClock clock;
            uint64_t counter = 0;
        } clockAndMem;

        std::cout << "MV_CC_RegisterImageCallBackEx" << std::endl;
        nRet = MV_CC_RegisterImageCallBackEx(
            handle,
            [](unsigned char* buffer, MV_FRAME_OUT_INFO_EX* frameInfo, void* clockAndMem) {
                std::memcpy(buffer, ((ClockAndMem*)clockAndMem)->buffer, 104 * 2592);
                std::cout << "frameInfo size:" << sizeof(MV_FRAME_OUT_INFO_EX) << std::endl;
            },
            &clockAndMem);
        if (MV_OK != nRet) {
            std::cerr << "MV_CC_RegisterImageCallBackEx fail! nRet" << nRet << std::endl;
        }

        std::cout << "MV_CC_RegisterExceptionCallBack" << std::endl;
        nRet = MV_CC_RegisterExceptionCallBack(
            handle, [](unsigned int msgType, void* handle) {

            // as an exception occured, we stop the camera and restart it

            std::cerr << "exception " << msgType << std::endl;

            std::cout << "Ex-callback: MV_CC_StopGrabbing" << std::endl;
            int nRet = MV_CC_StopGrabbing(handle);
            if (MV_OK != nRet) {
                std::cerr << "Ex-callback: MV_CC_StopGrabbing fail! nRet" << nRet << std::endl;
            }

            std::cout << "Ex-callback: MV_CC_CloseDevice" << std::endl;
            nRet = MV_CC_CloseDevice(handle);
            if (MV_OK != nRet) {
                std::cerr << "Ex-callback: MV_CC_CloseDevice fail! nRet" << nRet << std::endl;
            }

            std::cout << "Ex-callback: MV_CC_DestroyHandle" << std::endl;
            nRet = MV_CC_DestroyHandle(handle);
            if (MV_OK != nRet) {
                std::cerr << "Ex-callback: MV_CC_DestroyHandle fail! nRet" << nRet << std::endl;
            }


            // here I would start the cameras again
            // ...

        }, handle);
        if (MV_OK != nRet) {
            std::cerr << "MV_CC_RegisterExceptionCallBack fail! nRet" << nRet << std::endl;
        }

        std::cout << "MV_CC_StartGrabbing" << std::endl;
        nRet = MV_CC_StartGrabbing(handle);
        if (MV_OK != nRet) {
            std::cerr << "MV_CC_StartGrabbing fail! nRet" << nRet << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // 停止取流
        // end grab image
        std::cout << "MV_CC_StopGrabbing" << std::endl;
        nRet = MV_CC_StopGrabbing(handle);
        if (MV_OK != nRet) {
            std::cerr << "MV_CC_StopGrabbing fail! nRet" << nRet << std::endl;
        }

        // 关闭设备
        // close device
        std::cout << "MV_CC_CloseDevice" << std::endl;
        nRet = MV_CC_CloseDevice(handle);
        if (MV_OK != nRet) {
            std::cerr << "MV_CC_CloseDevice fail! nRet" << nRet << std::endl;
        }

        // 销毁句柄
        // destroy handle
        std::cout << "MV_CC_DestroyHandle" << std::endl;
        nRet = MV_CC_DestroyHandle(handle);
        if (MV_OK != nRet) {
            std::cerr << "MV_CC_DestroyHandle fail! nRet" << nRet << std::endl;
        }
    }

    if (nRet != MV_OK) {
        if (handle != NULL) {
            MV_CC_DestroyHandle(handle);
            handle = NULL;
        }
    }


    return 0;
}
