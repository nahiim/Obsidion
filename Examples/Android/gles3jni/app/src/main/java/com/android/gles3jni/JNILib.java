package com.android.gles3jni;

// Wrapper for native library

import android.content.res.AssetManager;

public class JNILib
{

    static {
        System.loadLibrary("main");
    }

    public static native void init(AssetManager asset_manager);
    public static native void resize(int width, int height);
    public static native void step();
}
