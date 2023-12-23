package com.example.jnitest1;

import android.util.Log;

import androidx.annotation.NonNull;

import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

public class Logger {
    public static class Builder {
        String event;
        Map<String, Object> data = new HashMap<>();

        Builder event(String event) {
            this.event = event;
            return this;
        }

        Builder append(@NonNull String key, Object value) {
            this.data.put(key, value);
            return this;
        }

        void done() {
            Log.e("myTag", "event=" + event + ", " + "data=" + data);
        }
    }

    public static Builder build() {
        return new Builder();
    }


}
