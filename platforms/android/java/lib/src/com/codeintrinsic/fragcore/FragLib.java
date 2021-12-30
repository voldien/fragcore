package com.codeintrinsic.fragcore;

import android.app.Activity;

public class FragCoreLib {

	static {
		System.loadLibrary("fragcore-android-native-lib");
	}

	public static native void initialize(Activity activity);

	public static native void ondestroy();
}
