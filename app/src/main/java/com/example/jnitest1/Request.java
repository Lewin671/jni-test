package com.example.jnitest1;

import java.util.Map;

public class Request {
    public String getUrl() {
        return url;
    }
    public Map<String, String> getHeaders() {
        return headers;
    }

    final public String url;
    final public Map<String, String> headers;

    public Request(String url, Map<String, String> headers) {
        this.url = url;
        this.headers = headers;
    }
}

