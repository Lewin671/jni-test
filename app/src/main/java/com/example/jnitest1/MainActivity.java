package com.example.jnitest1;

import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.webkit.WebResourceRequest;
import android.webkit.WebResourceResponse;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.TextView;

import com.example.jnitest1.databinding.ActivityMainBinding;

import java.io.ByteArrayInputStream;
import java.io.File;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'jnitest1' library on application startup.
    static {
        System.loadLibrary("jnitest1");
    }

    private ActivityMainBinding binding;
    private WebView mWebView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.hide();
        }

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method

        File externalFilesDir = getExternalFilesDir(null);
        assert externalFilesDir != null;
        Log.e("myTag", "externalFilesDir: " + externalFilesDir.getAbsolutePath());
        final WebView webView = new WebView(MainActivity.this);
        mWebView = webView;
        webView.getSettings().setJavaScriptEnabled(true);
        webView.setWebViewClient(new WebViewClient() {
            @Nullable
            @Override
            public WebResourceResponse shouldInterceptRequest(WebView view, WebResourceRequest request) {
                String url = null;
                Uri uri = request.getUrl();
                if (uri != null) {
                    url = uri.toString();
                }

                if (url != null && (url.endsWith(".js") || url.endsWith(".css"))) {
                    long start = SystemClock.uptimeMillis();
                    Response response = LocalCDNService.sendRequest(new Request(url, request.getRequestHeaders()));
                    long cost = SystemClock.uptimeMillis() - start;
                    if (response != null) {
                        Log.e("myTag", "hit localCDN : " + cost + " fetchType=" + response.headers.get("fetchType") + " byteLen=" + response.bytes.length + " " + url);
                        boolean isCss = url.endsWith(".css");
                        String mimeType = isCss ? "text/css" : "text/javascript";
                        WebResourceResponse webResourceResponse = new WebResourceResponse(mimeType, "UTF-8", new ByteArrayInputStream(response.bytes));
                        webResourceResponse.setResponseHeaders(response.headers);
                    }
                }
                return super.shouldInterceptRequest(view, request);
            }
        });

        binding.button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread("test") {
                    @Override
                    public void run() {
                        setUpStorage(externalFilesDir.getAbsolutePath() + "/cache/");
                        new Handler(Looper.getMainLooper()).post(new Runnable() {
                            @Override
                            public void run() {
                                webView.loadUrl("https://main.m.taobao.com/");
                                setContentView(webView);
                            }
                        });
                    }
                }.start();
            }
        });
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            WebView webView = mWebView;
            if (webView != null && webView.canGoBack()) {
                webView.goBack();
                return true;
            }
        }
        return super.onKeyDown(keyCode, event);
    }

    /**
     * A native method that is implemented by the 'jnitest1' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void setUpStorage(String rootDir);

}