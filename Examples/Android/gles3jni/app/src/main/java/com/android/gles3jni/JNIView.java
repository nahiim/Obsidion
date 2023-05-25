package com.android.gles3jni;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class JNIView extends GLSurfaceView
{
     private static final String TAG = "GLES3JNI";
     private static final boolean DEBUG = true;

     public JNIView(Context context)
     {
          super(context);
     }
     public JNIView(Context context, AssetManager asset_manager)
     {
          super(context);

          // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
          // supporting OpenGL ES 2.0 or later backwards-compatible versions.
          setEGLConfigChooser(8, 8, 8, 0, 16, 0);
          setEGLContextClientVersion(3);
          setRenderer(new Renderer(asset_manager));
     }

     private static class Renderer implements GLSurfaceView.Renderer
     {
          AssetManager m_assetManager;

          public Renderer(AssetManager asset_manager)
          {
               m_assetManager = asset_manager;
          }
          public void onDrawFrame(GL10 gl)
          {
               JNILib.step();
          }

          public void onSurfaceChanged(GL10 gl, int width, int height)
          {
               JNILib.resize(width, height);
          }

          public void onSurfaceCreated(GL10 gl, EGLConfig config)
          {
               JNILib.init(m_assetManager);
          }
     }
}