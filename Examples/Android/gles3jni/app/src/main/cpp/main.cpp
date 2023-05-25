

#include <jni.h>
#include <android/log.h>
#include <EGL/egl.h>

#include <chrono>

#include "include/obs/asset_loader.h"

#include "src/Application.h"
// Include the latest possible header file( GL version header )
#if __ANDROID_API__ >= 24
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>


#endif

//#define DEBUG 1
#define LOG_TAG "ANDROID LOG"
#define LOG_INFO(...)    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOG_ERROR(...)   __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_DEBUG(...)   __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#define LOG_VERBOSE(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

using namespace std::chrono;

Application app;

AAssetManager* m_asset_manager;
AssetLoader asset_loader;

double dt, start_time, time_passed, curr_frame_time, prev_frame_time = 0;



extern "C"
{
    JNIEXPORT void JNICALL Java_com_android_gles3jni_JNILib_init(JNIEnv* env, jobject obj, jobject asset_manager)
    {

        m_asset_manager = AAssetManager_fromJava(env, asset_manager);

        asset_loader = AssetLoader(m_asset_manager);

        // Get the current time point
        auto now = high_resolution_clock::now();

        // Cast the duration since the epoch to a double
        start_time = duration_cast<duration<double>>(now.time_since_epoch()).count();
        app.Init(asset_loader);
    }


    JNIEXPORT void JNICALL Java_com_android_gles3jni_JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height)
    {}


    JNIEXPORT void JNICALL Java_com_android_gles3jni_JNILib_step(JNIEnv* env, jobject obj)
    {
        auto now = std::chrono::high_resolution_clock::now();
        curr_frame_time = duration_cast<duration<double>>(now.time_since_epoch()).count();

        dt = curr_frame_time - prev_frame_time;
        prev_frame_time = curr_frame_time;

        auto tp = std::chrono::high_resolution_clock::now();
        double time_passed_since_epoch = duration_cast<duration<double>>(tp.time_since_epoch()).count();

        time_passed = time_passed_since_epoch - start_time;

        app.Update(dt, time_passed);
        app.Render(dt, time_passed);
    }
};




