# Projet Métallophone 1A

Ce projet implémente un métallophone automatisé contrôlé par un microcontrôleur **STM32** et un ordinateur. Il se compose de deux parties distinctes :

- 🎛️ **Partie microcontrôleur (STM32)** : interface utilisateur locale et contrôle des mailloches
- 💻 **Partie PC** : lecture de fichiers MIDI et transmission des commandes via liaison série virtuelle (Dossier "Transmitter")

---

## Architecture du projet

### 1. Partie STM32 (firmware)

Le firmware du STM32 gère :

- L'**interface utilisateur locale** :
  - Codeur rotatif pour la navigation
  - Afficheur LCD
  - Bouton poussoir pour la validation
- La **réception des commandes série** envoyées par l'ordinateur via la liaison UART virtuelle (ST-LINK)
- L'**actionnement des mailloches** via des sorties GPIO

#### Plateforme utilisée
- Microcontrôleur : **STM32F031K6 Nucleo**
- IDE recommandé : **STM32CubeIDE**
- Bibliothèques : **HAL, afficheur LCD**

---

### 2. Partie PC (logiciel hôte)

Le programme sur l’ordinateur effectue les tâches suivantes :

- Chargement et lecture d’un **fichier MIDI**
- Traduction des notes MIDI en **commandes compréhensibles par le STM32**
- Envoi des commandes via **liaison série virtuelle UART (port COM)** fournie par le ST-LINK

---

## ▶️ Exécution rapide

### STM32
1. Ouvrir le projet dans **STM32CubeIDE**
2. Connecter via USB (ST-LINK) (vérifier que le ST-Link est bien configuré dans le projet)
3. Compiler et flasher le microcontrôleur

### PC
1. Compiler la partie "Transmitter" (make fourni)
2. Lancer le programme (ouvrir un terminal, puis cd bin, puis transmitter.exe)
3. Entrez le chemin du fichier
