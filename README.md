# Juicer

[![Release](https://img.shields.io/github/v/release/applejuicenetz/gui-juicer.svg)](https://github.com/applejuicenetz/gui-juicer/releases/latest)
[![Downloads](https://img.shields.io/github/downloads/applejuicenetz/gui-juicer/total)](https://github.com/applejuicenetz/gui-juicer/releases)
[![Lizenz](https://img.shields.io/github/license/applejuicenetz/gui-juicer.svg)](LICENSE)
[![CI](https://github.com/applejuicenetz/gui-juicer/actions/workflows/ci.yml/badge.svg)](https://github.com/applejuicenetz/gui-juicer/actions/workflows/ci.yml)
[![Release-Build](https://github.com/applejuicenetz/gui-juicer/actions/workflows/build.yml/badge.svg)](https://github.com/applejuicenetz/gui-juicer/actions/workflows/build.yml)

Juicer ist eine Qt4-Oberfläche für den appleJuice Core. Die Verbindung erfolgt über dessen HTTP/XML-Schnittstelle.

## Linux

Build-Ziele sind ausschließlich **64 Bit: amd64 und arm64**. Das Archiv für die eigene Architektur entpacken und `./juicer` starten. Qt4-Bibliotheken liegen im Archiv; Systembibliotheken für X11, Schriftarten, Bilder und OpenSSL müssen auf dem Zielsystem vorhanden sein. Die Pakete sind keine vollständig statischen Anwendungen.

Im Anmeldedialog Host, XML-Port (standardmäßig `9851`) und Core-Passwort eintragen. Für lokale Verbindungen `127.0.0.1` verwenden. Der Core muss separat installiert und gestartet sein.

Dateigrößen und Chunkpositionen werden als 64-Bit-Werte verarbeitet. Fortschrittsbalken verwenden einen skalierten Wertebereich.

## Einschränkungen

Docker wird nur zum Bauen verwendet und ist zum Starten der Pakete nicht nötig.

Lizenz: GNU GPL v3 oder neuer, siehe [LICENSE](LICENSE).
