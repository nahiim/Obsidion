package com.android.gles3jni;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;

public class JNIActivity extends Activity
{
    JNIView mView;

    AssetManager as;

    @Override protected void onCreate(Bundle icicle)
    {
        super.onCreate(icicle);
        as = getResources().getAssets();
        mView = new JNIView(this, as);
        setContentView(mView);
    }

    @Override protected void onPause()
    {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume()
    {
        super.onResume();
        mView.onResume();
    }
}
