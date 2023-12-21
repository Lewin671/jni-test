package com.example.jnitest1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.jnitest1.databinding.ActivityMainBinding;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'jnitest1' library on application startup.
    static {
        System.loadLibrary("jnitest1");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.e("myTag", stringFromJNI());
            }
        });

        File externalFilesDir = this.getExternalFilesDir(null);
        Log.e("myTag", "externalFilesDir: " + externalFilesDir.getAbsolutePath());
        new Thread("test") {
            @Override
            public void run() {
                setUpStorage(externalFilesDir.getAbsolutePath() + "/");
            }
        }.start();

//        new Thread() {
//            @Override
//            public void run() {
//                Request request = new Request("https://www.baidu.com/", null);
//                Response response = Network.sendRequest(request);
//                if (response != null) {
//                    Log.e("myTag", response.statusCode);
//                    Log.e("myTag", response.headers.toString());
//                    Log.e("myTag", new String(response.bytes));
//                }
//            }
//        }.start();
    }

    /**
     * A native method that is implemented by the 'jnitest1' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void setUpStorage(String rootDir);

}