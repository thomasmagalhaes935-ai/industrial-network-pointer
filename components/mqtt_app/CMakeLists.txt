idf_component_register(
    SRCS 
        "mqtt.c"
    INCLUDE_DIRS 
        "include"
    REQUIRES 
        mqtt         # Chama o componente nativo MQTT do ESP-IDF
        esp_event
        log
)