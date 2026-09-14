import json
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
    try:
        mensagem = msg.payload.decode()

        dados = json.loads(mensagem)

        esteira_id = dados["esteira_id"]
        quantidade = dados["quantidade"]

        print("\n[MQTT] Mensagem recebida")
        print(f"Tópico: {msg.topic}")
        print(f"Esteira: {esteira_id}")
        print(f"Quantidade: {quantidade}")

    except json.JSONDecodeError:
        print("\n[MQTT] Erro: mensagem não é um JSON válido")
        print(f"Mensagem recebida: {mensagem}")

    except KeyError as e:
        print(f"\n[MQTT] Erro: campo ausente no JSON: {e}")


client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

client.on_connect = on_connect
client.on_message = on_message

print("Iniciando servidor MQTT...")

client.connect(BROKER, PORT, 60)

client.loop_forever()