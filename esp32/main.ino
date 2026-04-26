#include <WiFi.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";
const char* serverIP = "YOUR_LAPTOP_IP";
const int serverPort = 5000;

WiFiClient client;

QueueHandle_t xUploadQueue;
QueueHandle_t xCommandQueue;

#define RED_LED 2
#define BLUE_LED 4

void vSensorTask(void *pvParameters) {
    int temp = 25;
    char msg[32];

    while (1) {
        sprintf(msg, "REAL:%d", temp);
        xQueueSend(xUploadQueue, &msg, portMAX_DELAY);

        Serial.println(msg);
        temp++;

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vWiFiTask(void *pvParameters) {
    char msg[32];

    while (1) {
        if (!client.connected()) {
            if (!client.connect(serverIP, serverPort)) {
                vTaskDelay(pdMS_TO_TICKS(2000));
                continue;
            }
        }

        if (xQueueReceive(xUploadQueue, &msg, 0)) {
            client.println(msg);
        }

        if (client.available()) {
            String cmd = client.readStringUntil('\n');
            char buffer[32];
            cmd.toCharArray(buffer, 32);
            xQueueSend(xCommandQueue, &buffer, portMAX_DELAY);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vControlTask(void *pvParameters) {
    char cmd[32];

    while (1) {
        if (xQueueReceive(xCommandQueue, &cmd, portMAX_DELAY)) {
            if (strcmp(cmd, "CMD:COOL:ON") == 0) {
                digitalWrite(BLUE_LED, HIGH);
                digitalWrite(RED_LED, LOW);
            } else {
                digitalWrite(RED_LED, LOW);
                digitalWrite(BLUE_LED, LOW);
            }
        }
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);

    xUploadQueue = xQueueCreate(10, sizeof(char[32]));
    xCommandQueue = xQueueCreate(10, sizeof(char[32]));

    xTaskCreate(vControlTask, "Control", 4096, NULL, 3, NULL);
    xTaskCreate(vSensorTask, "Sensor", 4096, NULL, 2, NULL);
    xTaskCreate(vWiFiTask, "WiFi", 4096, NULL, 1, NULL);
}

void loop() {}
