# Offene Aufgaben und Ideen

## Funktionale Ergänzungen

- [ ] Optionales automatisches Anpassen der Spaltenbreiten nach Aktualisierungen anbieten; manuelles Anpassen ist bereits vorhanden.
- [ ] Verbindungsdauer statt Verbindungsdatum anzeigen.
- [ ] Core optional aus Juicer starten.
- [ ] Incoming-Ansicht um Navigation in Unterverzeichnissen und weitere Dateioperationen wie Verschieben erweitern; Auflisten, Kopieren und Löschen sind bereits vorhanden.
- [ ] In `IncomingModule::storeFtp()` bei bereits vorhandener Zieldatei einen anderen Dateinamen abfragen; bisher wird nur eine Fehlermeldung angezeigt.
- [ ] Fortschrittsanzeige für lokale Kopiervorgänge ergänzen; FTP-Übertragungen besitzen bereits eine Anzeige.
- [ ] Übersetzungen vervollständigen, insbesondere die Texte der GitHub-Updateprüfung.

## Prüfaufgaben

Die folgenden Punkte stammen aus ungeklärten Fehlermeldungen beziehungsweise Testwünschen. Vor einer Fehlerkorrektur Verhalten reproduzieren und einen Regressionstest ergänzen.

- [ ] Sortierung aller Upload- und Serverspalten prüfen; Vergleichsoperatoren sind vorhanden, vollständige Korrektheit ist nicht belegt.
- [ ] Login und Wiederverbindung prüfen, insbesondere Verbindungsabbruch unter Windows und nachträglicher Start des Core bei bereits laufender GUI.
- [ ] Öffnen von Downloads über FTP vollständig prüfen; der zugehörige Code ist vorhanden.
- [ ] Fehlerbehandlung der Incoming-FTP-Funktionen prüfen, insbesondere Verbindungsfehler beim Auflisten und Löschen.
- [ ] Auswahl von Freigabeverzeichnissen unter Windows prüfen.
- [ ] Schutz vor versehentlichem Löschen neuer Dateien bewerten; allgemeine Löschbestätigung ist vorhanden.

## Gestaltungsideen

- [ ] Eigenständiges, konsistentes Icon-Set gestalten; dabei ein passendes Symbol zum Kopieren von `ajfsp://`-Links vorsehen.
- [ ] Weitere anpassbare Ansichten prüfen; Dock-Widgets für Teileliste und Server-Willkommensnachricht sind bereits vorhanden.
