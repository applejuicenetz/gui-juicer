# Changelog

### Version 1.3.1

* Neuer Start-Tab mit den appleJuice-News passend zur Core-Version, wie in den anderen GUIs; Links öffnen im Browser
* HTTPS funktioniert jetzt auch unter Linux: Pakete bringen OpenSSL 1.0 mit; News und Update-Prüfung sind damit erreichbar
* Server-Quelle verwendet die aktuelle URL `http://www.applejuicenet.cc/serverlist/xmllist.php` wie JavaGUI und phpGUI; die alte Adresse wird automatisch ersetzt
* Schreibweise „appleJuice“ im Menü
* Über-Dialog verweist auf das GitHub-Repository statt auf `applejuicer.net`

### Version 1.3

* Unterstützung für Dateien über 2 GiB (64-Bit-Dateigrößen und -Positionen) passend zum neuen Core; mit großen Dateien gegen einen echten Core getestet
* Größen in ajfsp-Links, Suchergebnissen, Freigaben und Listenexport werden nicht mehr abgeschnitten
* Fortschrittsanzeigen für Down- und Uploads korrekt bei Positionen über 4 GiB
* Neue Pakete: Windows-Setup (x64, läuft auch auf Windows ARM64), Linux-Archive für amd64 und arm64, Flatpak für arm64
* Update-Prüfung über die GitHub-Release-API mit Link auf die Release-Seite; das alte Self-Update (HTTP-Download und Überschreiben der Programmdateien) ist entfernt

### Version 1.2

* Dock-Widgets für Teileliste und Server-Willkommensnachricht
* Auto-Update-Funktionalität für vorkompilierte Windows Version
* ajfsp-Handler unter Windows
* Anzeige des Online-Benutzerhandbuchs hinzugefügt
* Filter für Suchergebnisse
* Button um die gesamte Konfiguration von Juicer auf Standardwerte zurückzusetzen
* Tippfehler korrigiert
* Übersetzungen hinzugefügt und korrigiert (deutsch)
* Ändern des Status von Start/Pause-Buttons nach Statuswechsel von Downloads
* laden der Freigaben nach dem Start
* engeres Desgin (weniger Rand, Bescheibungen in der Statuszeile entfernt)
* Sortierung von Elementen mit Fortschirttsanzeige im Upload-Fenster korrigiert
* Version im "Über-Dialog"
* Schmaler Zeilen im Down- und Upload-Fenster
* Zwang für Passwort beim verbinden zu einem Core auf localhost korrigiert


### Version 1.1

* Größenverhalten beim zurückkehren vom Tray korrigiert
* Networkinfo-Dialog verbessert
* mehr informationen im Upload-Fenster
* neue Geschwindigkeitsberechnung für Downloads
* Überprüfe ob Core nicht auf localhost läuft und warne ggf. an den entspr. Stellen
* zusätzliche Informationen über Benutzer (nickname, downloadblock)
* Bug beim Schließen der GUI mit Tray-Icon korrigiert
* Speicherzugriffsfehler durch Tray-Icon beim Beenden entfernt
* globales Setzen des Powerdownloads geändert
* speichern und wiederherstellen der Hauptfensterstati
* Dateinamen im Upload-Fenster korrigiert
* einige Speicherzugriffsfehler bei Core nicht auf localhost entfernt
* Speicherzugriffsfehler beim Verbindungsverlust entfernt
* Speicherzugriffsfehler beim laden der Freigaben entfernt
* ...
