package com.example.jnitest1;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Network {

    public static Response sendRequest(Request request) {
        HttpURLConnection connection = null;
        try {
            // 创建 URL 对象
            URL url = new URL(request.url);

            // 打开连接
            connection = (HttpURLConnection) url.openConnection();

            // 设置请求头
            if (request.headers != null) {
                for (Map.Entry<String, String> header : request.headers.entrySet()) {
                    connection.setRequestProperty(header.getKey(), header.getValue());
                }
            }

            // 获取响应码
            int responseCode = connection.getResponseCode();
            String statusCode = Integer.toString(responseCode);

            // 获取 MIME 类型
//            String mimeType = connection.getContentType();

            // 读取响应体
            InputStream inputStream = connection.getInputStream();
            ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
            byte[] buffer = new byte[2048];
            int bytesRead;
            while ((bytesRead = inputStream.read(buffer)) != -1) {
                outputStream.write(buffer, 0, bytesRead);
            }
            byte[] bytes = outputStream.toByteArray();

            // 获取所有响应头字段
            Map<String, List<String>> headers = connection.getHeaderFields();

            // 创建一个新的Map来存储结果
            Map<String, String> simplifiedHeaders = new HashMap<>();

            // 将每个响应头字段的第一个值添加到新的Map中
            for (Map.Entry<String, List<String>> entry : headers.entrySet()) {
                if (entry.getKey() != null && !entry.getValue().isEmpty()) {
                    simplifiedHeaders.put(entry.getKey(), entry.getValue().get(0));
                }
            }

            // 创建并返回 Response 对象
            return new Response(simplifiedHeaders, statusCode, bytes);
        } catch (Throwable throwable) {
            // 处理异常
            throwable.printStackTrace();
            return null;
        } finally {
            if (connection != null) {
                connection.disconnect();
            }
        }
    }


}
