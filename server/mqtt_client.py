import paho.mqtt.client as mqtt

BROKER = "mqtt-dashboard.com"
PORT = 1883
TOPIC = "esteira/produtos/contagem"


def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print("Conectado ao broker MQTT!")
        print(f"Inscrito no tópico: {TOPIC}")

        client.subscribe(TOPIC)
    else:
        print(f"Falha na conexão. Código: {reason_code}")


def on_message(client, userdata, msg):
    mensagem = msg.payload.decode()

    print("\n[MQTT] Mensagem recebida")
    print(f"Tópico: {msg.topic}")
    print(f"Mensagem: {mensagem}")


client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

client.on_connect = on_connect
client.on_message = on_message

print("Iniciando servidor MQTT...")

client.connect(BROKER, PORT, 60)

client.loop_forever()