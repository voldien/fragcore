package org.fragengine.frag;

import android.app.Instrumentation;
import android.content.Intent;
import android.os.Bundle;

public class FragInstrumentation extends Instrumentation {
	private Intent intent;

	@Override
	public void onCreate(Bundle arguments) {
		intent = arguments.getParcelable("intent");
		start();
	}

	@Override
	public void onStart() {
		startActivitySync(intent);
	}
}
