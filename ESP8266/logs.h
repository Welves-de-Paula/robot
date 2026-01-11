#ifndef LOGS_H
#define LOGS_H

#include <Arduino.h>
#include <ESP8266WebServer.h>

/* ================= LOG SYSTEM ================= */
#define MAX_LOGS 20

struct LogEntry
{
    unsigned long timestamp;
    String method;
    String uri;
    String protocol;
    String clientIP;
    String headers;
    String body;
    int contentLength;
    bool active;
};

class LogSystem
{
private:
    LogEntry logs[MAX_LOGS];
    int currentIndex = 0;
    int totalLogs = 0;

public:
    void addLog(ESP8266WebServer &server)
    {
        LogEntry &log = logs[currentIndex];
        log.timestamp = millis();
        log.method = server.method() == HTTP_GET ? "GET" : server.method() == HTTP_POST  ? "POST"
                                                       : server.method() == HTTP_PUT     ? "PUT"
                                                       : server.method() == HTTP_DELETE  ? "DELETE"
                                                       : server.method() == HTTP_PATCH   ? "PATCH"
                                                       : server.method() == HTTP_OPTIONS ? "OPTIONS"
                                                                                         : "OTHER";
        log.uri = server.uri();
        log.protocol = "HTTP/1.1";
        log.clientIP = server.client().remoteIP().toString();
        log.contentLength = server.hasArg("plain") ? server.arg("plain").length() : 0;
        log.body = server.hasArg("plain") ? server.arg("plain") : "";

        // Capturar headers principais
        log.headers = "";
        if (server.hasHeader("User-Agent"))
        {
            log.headers += "User-Agent: " + server.header("User-Agent") + "\n";
        }
        if (server.hasHeader("Content-Type"))
        {
            log.headers += "Content-Type: " + server.header("Content-Type") + "\n";
        }
        if (server.hasHeader("Authorization"))
        {
            log.headers += "Authorization: [HIDDEN]\n";
        }
        if (server.hasHeader("Cookie"))
        {
            log.headers += "Cookie: [HIDDEN]\n";
        }
        if (server.hasHeader("Referer"))
        {
            log.headers += "Referer: " + server.header("Referer") + "\n";
        }

        log.active = true;
        currentIndex = (currentIndex + 1) % MAX_LOGS;
        if (totalLogs < MAX_LOGS)
            totalLogs++;
    }

    String getLogsJSON()
    {
        String json = "{\"logs\":[";
        bool first = true;

        // Percorrer do mais recente ao mais antigo
        int count = 0;
        for (int i = 0; i < totalLogs && count < MAX_LOGS; i++)
        {
            int idx = (currentIndex - 1 - i + MAX_LOGS) % MAX_LOGS;
            if (logs[idx].active)
            {
                if (!first)
                    json += ",";
                first = false;

                json += "{";
                json += "\"timestamp\":" + String(logs[idx].timestamp) + ",";
                json += "\"method\":\"" + logs[idx].method + "\",";
                json += "\"uri\":\"" + logs[idx].uri + "\",";
                json += "\"protocol\":\"" + logs[idx].protocol + "\",";
                json += "\"clientIP\":\"" + logs[idx].clientIP + "\",";
                json += "\"contentLength\":" + String(logs[idx].contentLength) + ",";
                json += "\"headers\":\"" + escapeJSON(logs[idx].headers) + "\",";
                json += "\"body\":\"" + escapeJSON(logs[idx].body.substring(0, 500)) + "\"";
                json += "}";
                count++;
            }
        }

        json += "]}";
        return json;
    }

    void clear()
    {
        for (int i = 0; i < MAX_LOGS; i++)
        {
            logs[i].active = false;
        }
        currentIndex = 0;
        totalLogs = 0;
    }

    int getCount()
    {
        return totalLogs;
    }

private:
    String escapeJSON(String str)
    {
        String result = "";
        for (unsigned int i = 0; i < str.length(); i++)
        {
            char c = str.charAt(i);
            if (c == '"')
                result += "\\\"";
            else if (c == '\\')
                result += "\\\\";
            else if (c == '\n')
                result += "\\n";
            else if (c == '\r')
                result += "\\r";
            else if (c == '\t')
                result += "\\t";
            else if (c < 32)
                result += "";
            else
                result += c;
        }
        return result;
    }
};

#endif