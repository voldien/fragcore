package org.fragengine.frag;

import android.app.Activity;

public class FragLib {

	static {
		System.loadLibrary("fragengine_android");
	}

	public static native void initialize(Activity activity);

	public static native void ondestroy();
}
