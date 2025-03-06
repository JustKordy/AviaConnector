#include <Windows.h>
#include <iostream>
#include "Windows.h"
#include <SimConnect/SimConnect.h>

HANDLE hSimConnect = nullptr;

enum DATA_REQUESTS {
    REQUEST_1,
};

struct AircraftData {
    double latitude;
    double longitude;
    double altitude;
    double pitch;
    double yaw;
};

void CALLBACK MyDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
    if (pData->dwID == SIMCONNECT_RECV_ID_SIMOBJECT_DATA)
    {
        SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
        if (pObjData->dwRequestID == REQUEST_1)
        {
            AircraftData* pS = (AircraftData*)&pObjData->dwData;
            std::cout << "Lat: " << pS->latitude << " Lon: " << pS->longitude << " Alt: " << pS->altitude << " PITCH: " << pS->pitch << std::endl;
        }
    }
}

int main() {
    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Aircraft Position Example", nullptr, 0, 0, 0)))
    {
        std::cout << "Connected to Microsoft Flight Simulator!" << std::endl;

        SimConnect_AddToDataDefinition(hSimConnect, REQUEST_1, "PLANE LATITUDE", "degrees");
        SimConnect_AddToDataDefinition(hSimConnect, REQUEST_1, "PLANE LONGITUDE", "degrees");
        SimConnect_AddToDataDefinition(hSimConnect, REQUEST_1, "PLANE ALTITUDE", "feet");

        SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_1, REQUEST_1, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);

        while (true)
        {
            SimConnect_CallDispatch(hSimConnect, MyDispatchProc, nullptr);
            Sleep(1000);
        }

        SimConnect_Close(hSimConnect);
    }
    else
    {
        std::cout << "Failed to connect to SimConnect!" << std::endl;
    }

    return 0;
}