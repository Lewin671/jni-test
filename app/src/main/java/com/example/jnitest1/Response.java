package com.example.jnitest1;

import java.util.Map;

public class Response {
    final public Map<String, String> headers;

    public Map<String, String> getHeaders() {
        return headers;
    }

    public String getStatusCode() {
        return statusCode;
    }

    public byte[] getBytes() {
        return bytes;
    }

    final public String statusCode;
    final public byte[] bytes;


    public Response(Map<String, String> headers, String statusCode, byte[] bytes) {
        this.headers = headers;
        this.statusCode = statusCode;
        this.bytes = bytes;
    }
}
