
#include "test2.h"
#include <iostream>
#include <memory>
using namespace std;

APIdll::APIdll(const Napi::CallbackInfo& info) : ObjectWrap(info) {
    cout << "test 1" << endl;
    Napi::Env env = info.Env();
    string normal = info[0].As<Napi::String>().ToString();
    string anomaly = info[1].As<Napi::String>().ToString();
    this->isSimple = info[2].As<Napi::Boolean>();
    cout << "test 2" << endl;
    this->fileNameNormal = new char[strlen(normal.c_str()) + 1];
    this->fileNameAnomaly = new char[strlen(anomaly.c_str()) + 1];
    cout << "test 3" << endl;
    strcpy(this->fileNameNormal, normal.c_str());
    strcpy(this->fileNameAnomaly, anomaly.c_str());
    cout << "test 4" << endl;
    this->tsTrain = new TimeSeries(this->fileNameNormal);
    cout << "test 5" << endl;
    this->tsAnomaly = new TimeSeries(this->fileNameAnomaly);
    cout << "test 6" << endl;
    if (this->isSimple) {
        cout << "test 7" << endl;
        this->simpleAD.learnNormal(*tsTrain);
        cout << "test 8" << endl;
        this->anomalyReport = simpleAD.detect(*tsAnomaly);
        cout << "test 9" << endl;
    }
    else {
        cout << "test 7" << endl;
        this->hybridAD.learnNormal(*tsTrain);
        cout << "test 8" << endl;
        this->anomalyReport = simpleAD.detect(*tsAnomaly);
        cout << "test 9" << endl;
    }
}

Napi::Value APIdll::getAllAnomalyReportCpp(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    string returnStr;
    cout << "get 1" << endl;
    for (auto i = anomalyReport.begin(); i != anomalyReport.end(); ++i) {
        cout << i->timeStep << endl;
        cout << i->description << endl;
        returnStr += to_string(i->timeStep) + " " + i->description + "\n";

    }
    cout << "get 2" << endl;
    cout << returnStr << endl;
    return Napi::String::New(env, returnStr);
}

Napi::Function APIdll::GetClass(Napi::Env env) {
    return DefineClass(env, "APIdll", {
        APIdll::InstanceMethod("GetAllAnomalyReportCpp", &APIdll::getAllAnomalyReportCpp),
        });
}

//init func
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::String nameAPIdll = Napi::String::New(env, "APIdll");
    exports.Set(nameAPIdll, APIdll::GetClass(env));
    return exports;
}

NODE_API_MODULE(addon, Init)



