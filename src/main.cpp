/**
 * Get your first picture with ESP32
 *
 * Open the Serial Monitor and enter 'capture' (without quotes)
 * to capture a new image
 *
 * BE SURE TO SET "TOOLS > CORE DEBUG LEVEL = INFO"
 * to turn on debug messages
 */
#include <Arduino.h>
#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/extra/esp32/fs/sdmmc.h>

using namespace eloq;
String label;

/**
 *
 */
void setup() {
    delay(3000);
    Serial.begin(115200);
    Serial.println("___GET YOUR FIRST PICTURE___");

    // camera settings
    // replace with your own model!
    // supported models:
    // - aithinker
    // - m5
    // - m5_wide
    // - m5_timer
    // - eye
    // - wrover
    // - wroom_s3
    // - freenove_s3
    // - xiao
    // - ttgo_lcd
    // - simcam
    camera.pinout.aithinker();
    camera.brownout.disable();
    // supported resolutions
    // - yolo (96x96)
    // - qqvga
    // - qcif
    // - face (240x240)
    // - qvga
    // - cif
    // - hvga
    // - vga
    // - svga
    // - xga
    // - hd
    // - sxga
    // - uxga
    // - fhd
    // - qxga
    // ...
    camera.resolution.svga();
    // supported qualities:
    // - low
    // - high
    // - best
    camera.quality.high();

    // init camera
    while (!camera.begin().isOk())
        Serial.println(camera.exception.toString());
    
    while (!sdmmc.begin().isOk())
        Serial.println(sdmmc.exception.toString());


    Serial.println("SD card OK");
    Serial.println("Camera OK");
    Serial.println("Informe o objeto a ser capturado: ");
}

/**
 *
 */
void loop() {
    // await for Serial command
    if (!Serial.available())
        return;

    label = Serial.readStringUntil('\n');
    if ( label != "celular" and label != "controle" and label != "pendrive" ) {
        Serial.println("objeto inválido");
        return;
    }

    // capture picture
    if (!camera.capture().isOk()) {
        Serial.println(camera.exception.toString());
        return;
    }

    if (sdmmc.save(camera.frame).inside(label).to("", "jpg").isOk()){
        Serial.print("Imagem salva em ");
        Serial.println(sdmmc.session.lastFilename);
    } else Serial.println(sdmmc.session.exception.toString());

    Serial.println("Informe o objeto a ser capturado: ");
}