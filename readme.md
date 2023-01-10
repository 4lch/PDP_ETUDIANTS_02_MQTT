# INTERNNEEEEET

## <ins>Prérequis</ins>
- Avoir un groupe
- Avoir un compte [Github](https://github.com)
- [Git / Git Bash](https://gitforwindows.org/)
- [VS Code](https://code.visualstudio.com/)
- [Platformio](https://platformio.org/platformio-ide)

---

## <ins>Objectif</ins>
Vous avez déjà eu l'opportunité d'utiliser MQTT lors du cours d'IOT. L'objectif de ce cours est de vous donner une nouvelle opportunité de bien comprendre le fonctionnement du prototole et son utilisation pour la distribution de messages légers en IOT. Le but de ce TP est de faire remonter les données de température et humidité lues lors du TP précédent avec MQTT.

## <ins>Préparation</ins>
1. `Forkez` le projet du TP sur votre compte github avec le nom **"PDP_ETUDIANT_02_TD<`NUMERO_DE_TD`>\_GP<`NUMERO_DE_GROUPE`>"**. Par exemple, le groupe 7 du TD 4 devra créer le repo **"PDP_ETUDIANT_02_TD04_GP07"**
2. <ins>`ATTENTION : un projet mal nommé sera sanctionné de -5pts sur le suivi du TP. C'est littéralement la partie la plus facile pour vous et ça facilite énormément la correction.`</ins>
3. Clonez **votre fork** sur votre machine avec `git clone <url>`
4. Ouvrir le projet avec PlatformIO (sinon les options de compilation et téléversement peuvent ne pas apparaître)
5. Compléter le code pour compléter chaque étape et `commit` **REGULIEREMENT**

Afin de vérifier votre compréhension du fonctionnement de l'environnement de développement, vous allez travailler avec Git, vous devrez donc `commit` vos modifications régulièrement. Chaque étape devra faire l'objet d'une `branch` distincte sur votre `repository`. Pour indiquer qu'un exercice est terminé, vous devrez tagguer le commit corrrespondant avec un formalisme indiqué dans les instructions. Comme vous devez travailler sur des branches par étape, le `master` devrait être inchangé à la fin.

## <ins>Evaluation</ins>
L'évaluation de votre travail sera réalisée sur les critères suivants :
- Etapes terminées
- Respect des consignes de nommage des `branch` et `tag` git
- Contenu et description des `commit`
- Qualité du code (structure, lisibilité, commentaires utiles si et seulement si nécessaire...)
- Temps nécessaire pour réaliser les étapes

## <ins>Quelques ressources</ins>
- [Documentation AZ-Delivery](https://cdn.shopify.com/s/files/1/1509/1638/files/ESP_-_32_NodeMCU_Developmentboard_Datenblatt_AZ-Delivery_Vertriebs_GmbH_10f68f6c-a9bb-49c6-a825-07979441739f.pdf?v=1598356497)
- [Reference framework Arduino](https://www.arduino.cc/reference/en/)
- [Datasheet DHT11](https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf)
- [Le site web de MQTT](https://mqtt.org/)
---

## <ins>Etape 1 - INTERNNEEEEET</ins>
Une seule étape dans ce TP :)

## Réalisation
1. Créer une `branch` etape_1 à partir de master avec `git checkout -b etape_1 master`. Cette commande crée la nouvelle branche et vous met dessus directement. Si vous la créez avec `git branch`, pensez à switcher dessus avec `git checkout etape_1`. La récupération des repos et des branches est automatisée. **Si la branche n'existe pas sur votre repo github ou qu'elle est mal nommée, elle ne sera pas corrigée.**
2. Vérifier que le code fourni compile et fonctionne. Il reprend là où vous avez laissé la fin du TP1.
3. Dans `main.cpp` dans le dossier `src`, remplacer le bloc
```C
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
```
par
```C
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "WiFiClientSecure.h"
#include <PubSubClient.h>

const char* wifi_ssid = <votre_ssid_wifi>;
const char* wifi_password = <votre_mdp_wifi>;
const char* mqtt_server = "27cc61dbaffc4da08cd0081cabd8cf01.s2.eu.hivemq.cloud";
int mqtt_port = 8883;
const char* mqtt_user = "ocres4ever";
const char* mqtt_pass = "ocresse123";
const char* client_id = <votre_groupe>;
```
4. Remplacer les bouts manquants dans le code:
   1. `<votre_ssid_wifi>` : Le nom du réseau WiFi que vous voulez utiliser. Si le WiFi de l'école ne marche pas, essayez avec du partage de connexion (bonne chance à E3 😁).
   2. `<votre_mdp_wifi>` : Le mot de passe du réseau WiFi.
   3. `<votre_groupe>` : Votre nom de groupe avec le formalisme "TDXX_GPXX", eg "TD02_GP01" pour le groupe 1 du TD 2. **IMPORTANT: Respectez exactement ce format pour votre groupe sinon les messages ne seront pas reçus.**
5. Petite explication :
   - Un vrai produit industriel utilisera sûrement un réseau LPWAN style LoRa plutôt que du WiFi pour limiter sa conso et augmenter la portée, mais ici on fait un proto, alors on va au plus simple :)
   - On utilise un broker cloud offert par HiveMQ (gratuit jusqu'à 100 connexions simultanées et 10GB de stockage) dont on configure les accès ici. Notez qu'on utilise la bibliothèque `"WiFiClientSecure.h"` au lieu de `"WiFi.h"` car HiveMQ force l'utilisation de connexions sécurisées par TLS.
   - On utilise la bibliothèque `"PubSubClient.h"` pour établir la connexion au broker avec le protocole MQTT.
   - la variable `client_id` est un identifiant qui permet au broker de connaître l'origine d'un message.
6. Au-dessus du `setup`, créer et instancier le client WiFi et le client MQTT.
```C
WiFiClientSecure client;
PubSubClient mqtt_client(client); 
```
7. Toujours au-dessus du setup, déclarer la fonction qui servira à connecter la carte au réseau WiFi. L'appeler à l'endroit indiqué dans le `setup`.
```C
void connect_wifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(wifi_ssid, wifi_password);
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 2OOms for re-trying
    delay(200);
  }
  Serial.println("\nConnected.");
}
```
8. Configurer le serveur MQTT à l'endroit indiqué avec
```C
mqtt_client.setServer(mqtt_server, mqtt_port);
```
9. A ce moment, le code est prêt à envoyer les valeurs lues au broker avec MQTT.
10. Au bon endroit dans le code, connectez le client au broker avec la fonction `mqtt_client.connect()`, connez-lui les bons arguments.
11. <ins>`Si et seulement si`</ins> la fonction `connect` a bien fonctionné (elle retourne un booléen...), publiez la température et l'humidité relative avec MQTT.
    - Le topic pour la température est `"TDXX_GPXX/temp"`, même formalisme que pour `client_id`.
    - Le topic pour l'humidité est `"TDXX_GPXX/relhum"`, même formalisme que pour `client_id`.
12. Un programme affiche les messages reçus sur le broker en direct. Vérifiez que votre montage envoie bien les mesures lues.
13. Appelez-moi pour vérifier que tout fonctionne bien.
14. `commit` le code si ce n'est pas déjà fait.
15. Tagguer le dernier commit à corriger avec "e1" avec la commande `git tag e1 HEAD`. Cette commande utilise "HEAD" comme référence au commit le plus récent.
17. Publier vos `commit` avec vos tags avec `git push origin --tags` (ou `git push --set_upstream origin etape_1 --tags` pour associer la branche sur le repo distant si c'est votre premier push sur cette branche)
---