# Juicer

## Architektur

Qt4-Anwendung mit QMake. `juicer.pro` baut `src/src.pro`; `XMLModule` verarbeitet die HTTP/XML-Schnittstelle des Core. `Item` speichert Dateigrößen als `qulonglong`. Bytepositionen dürfen nicht über `int`, `long` oder `double` in Links, Dateilisten oder Fortschrittsbalken gelangen. Prozentanzeigen dürfen Gleitkommaarithmetik verwenden; Originalwerte und Größen für Links und Sortierung bleiben Ganzzahlen. Fortschrittsbalken verwenden den festen Bereich 0 bis 10000.

Ziele sind ausschließlich 64 Bit: Linux amd64 und arm64, Windows x86_64 (läuft auf Windows ARM64 über Emulation). Keine 32-Bit-Builds, kein macOS.

## Build und Tests

Alle Builds laufen in Docker-Containern. Docker muss die gewählte Architektur ausführen können; auf ARM benötigt amd64 eine vorhandene binfmt/QEMU-Einrichtung. Ergebnisse landen in `dist/` (nicht versioniert).

```sh
sh packaging/build-linux.sh arm64
sh packaging/build-linux.sh amd64
sh packaging/build-windows.sh
sh packaging/build-flatpak.sh
```

- **Linux:** `Dockerfile.qt4` verwendet Ubuntu 18.04 mit Qt 4.8.7. `build-linux.sh` baut in einer temporären Container-Kopie, erzeugt die UI-Header neu, führt `tests/progress_test.cpp` unter Xvfb aus, bündelt die Qt4-Bibliotheken und `libaudio` und prüft den Start mit einem Timeout. Exit 124 ist beim Starttest erwartet, weil die GUI bis zum Timeout laufen soll. Ergebnis: `dist/juicer-1.3.1-linux-<arch>.tar.gz`. Systembibliotheken für X11, Schriftarten und Bilder bleiben externe Voraussetzungen.
- **Windows:** `Dockerfile.windows` (Ubuntu 18.04, statische MXE-Qt4-Toolchain aus dem signierten MXE-Paketarchiv, Schlüssel per GPG entpackt) baut `dist/juicer-1.3.1-windows-x86_64.exe`. Das Setup `packaging/juicer.nsi` bettet diese EXE direkt ein (kein Download, keine NSIS-Plugins) und folgt dem Aufbau der Vorlage im `setup`-Repository: Installation nach `Program Files`, Verknüpfungen, Deinstallationseintrag, `ajfsp://`-Registrierung. Es läuft nur auf 64-Bit-Windows. NSIS wird in einem getrennten Container ausgeführt. Ergebnis: `dist/juicer-1.3.1-windows-x86_64-setup.exe`. Das Setup ist auf Windows x64 und Windows ARM64 manuell geprüft, einschließlich `ajfsp://`.
- **Flatpak:** `build-flatpak.sh` baut aus dem Linux-Archiv der Host-Architektur ein Bundle `dist/juicer-1.3.1-linux-<arch>.flatpak`. Voraussetzung sind `flatpak-builder` sowie `org.freedesktop.Platform` und `Sdk` in Version 25.08. Die Dateien liegen unter `packaging/flatpak/`; App-ID `io.github.applejuicenetz.juicer`. Gebaut wird nur für die Host-Architektur; ein amd64-Bundle braucht einen amd64-Host. Das Icon `juicer-128.png` ist aus `src/resources/juicer-big.png` skaliert, weil AppStream eine Standardgröße verlangt.
- **Tests:** `tests/progress_test.cpp` prüft Positionen über 4 GiB, Grenzfälle bis `2^63 - 1` und exakte Speicherung oberhalb von 2^53. Bei Änderungen weitere Grenzfälle und echte Verbraucher testen. Ein GUI-Start belegt keine Core-Verbindung. Downloads und Uploads großer Dateien (mehrere GiB) sind manuell gegen einen echten Core geprüft; automatisiert abgedeckt sind nur die Rechenwege.

Qt4 und Ubuntu 18.04 sind veraltet; Container sind isolierte Build-Umgebungen, keine öffentlich erreichbaren Dienste. Nach Testläufen Container und nicht mehr benötigte Testimages entfernen.

## GitHub Actions

`.github/workflows/ci.yml` läuft bei Push auf `main` und Pull Requests ausschließlich mit Shell-Syntaxprüfung und Qt4-Regressionstests. Dabei werden nur Testprogramme kompiliert; keine Anwendungspakete oder Release-Artefakte erzeugt.

`.github/workflows/build.yml` läuft ausschließlich bei gepushten Versionstags wie `1.3.1` oder `v1.3.1`. Der Tag muss zur Version in `src/src.pro` und den Paketpfaden passen. Linux-Jobs bauen auf nativen Ubuntu-24.04-Runnern für amd64 und arm64 jeweils Archiv und Flatpak, führen die Pakettests aus und prüfen den Flatpak-Start unter Xvfb. Der Windows-Job baut auf amd64 die EXE und das eingebettete NSIS-Setup. Nach Erfolg aller Paket-Jobs erstellt ein separater Job den GitHub-Release mit vier Paketen (zwei Flatpaks, Windows-EXE und Setup) ohne separate Prüfsummendateien. Die Linux-Archive dienen nur als lokale Zwischenprodukte für den Flatpak-Build und werden nicht hochgeladen; zudem heißt das Setup-Asset für das Multi-Setup verbindlich `Juicer.setup.exe`; nur dieser Job erhält Schreibrechte. CI-Artefakte bleiben einen Tag erhalten. Versionsabhängige Artefaktpfade, Metadaten und Tag-Prüfung bei Versionswechseln gemeinsam anpassen.

## Wartung

- `README.md` enthält deutschsprachige Nutzerinformationen, diese Datei interne Hinweise.
- Quellen, UI-Dateien und Ressourcen gemeinsam pflegen. Generierte `ui_*.h`, `moc_*.cpp`, `qrc_*.cpp`, Objektdateien und Binaries nicht committen.
- Version in `src/src.pro`, `src/application.cpp`, `packaging/juicer.nsi`, Metainfo und Paketnamen gemeinsam ändern.
- `UpdateCheck` fragt per HTTPS die GitHub-Release-API (`/repos/applejuicenetz/gui-juicer/releases/latest`) ab und vergleicht `tag_name` mit der Anwendungsversion. Bei einer neueren Version kann die Release-Seite im Browser geöffnet werden. Keine Downloads, Installation oder Überschreibung von Programmdateien vorsehen. Start-Prüfungen sind still; der Menüpunkt zeigt auch Fehler oder den aktuellen Stand. `tests/updatecheck_test.cpp` prüft Tag-Auswertung und Versionsvergleich und läuft beim Linux-Paketbuild mit.
- Die statische Windows-Qt4-Toolchain enthält gelinktes OpenSSL; HTTPS und Auswertung der echten GitHub-Antwort sind auf der Windows-VM geprüft. Qt 4.8 lädt zur Laufzeit nur `libssl.so.1.0.0`/`libcrypto.so.1.0.0` (OpenSSL 1.0.x); Ubuntu 18.04 und die Freedesktop-Runtime haben nur 1.1 beziehungsweise 3. `Dockerfile.qt4` baut deshalb OpenSSL 1.0.2u mit geprüfter SHA256 als Shared Library (ohne `no-comp`/`no-ssl3`, sonst scheitert der Build oder Qt findet Symbole nicht), `build-linux.sh` legt beide Bibliotheken nach `lib/` im Archiv und Flatpak, und `newsfeed_test` prüft `QSslSocket::supportsSsl()` mit `LD_LIBRARY_PATH` auf das Paket. OpenSSL 1.0.2u ist End-of-Life; es dient nur den GET-Abrufen von News, Serverliste und Update-Prüfung. Bei Updates der Basis-Distribution `QSslSocket::supportsSsl()` erneut prüfen. Flatpak-Updates bleiben Aufgabe von Flatpak. TLS-Prüfung niemals deaktivieren.
- Im Windows-Setup unter Windows ARM64 und x64 bleibt `ajfsp://` auf `Juicer.exe` registriert; Registrierung nur gemeinsam mit der Vorlage im `setup`-Repository ändern.
- Vor Commit `git diff --check`, die Paketbuilds und Tests ausführen. Keine Veröffentlichung oder Push ohne Auftrag.
- `NewsFeed` lädt auf dem Start-Tab (`newsBrowser`, `QTextBrowser`, kein WebKit) `https://applejuicenetz.github.io/news/<Core-Version>.html`; ungültige oder leere Version ergibt `404.html`. Das Laden startet in `Juicer::setCoreVersion`. Charset kommt aus dem Meta-Tag (die Seiten sind ISO-8859-15), nur der Body wird angezeigt, `script`/`style`/`iframe`/`object`/`embed` werden entfernt. TLS-Prüfung bleibt aktiv; schlägt der Abruf fehl (zum Beispiel Linux-Qt4 ohne OpenSSL), zeigt der Tab einen Hinweis mit FAQ-Link. `tests/newsfeed_test.cpp` prüft URL und HTML-Aufbereitung und läuft in CI und Linux-Paketbuild. Neue Texte in `juicer_de.ts` und `juicer_de.qm` ergänzen.
- `ServerList` lädt die Server-Quelle (Standard `http://www.applejuicenet.cc/serverlist/xmllist.php`, wie JavaGUI und phpGUI), folgt bis zu drei Weiterleitungen und liefert die eindeutigen `ajfsp://server|…/`-Links an `Juicer::processLinks`. Leere oder die alte Einstellung `applejuicenet.de/18.0.html` werden auf den Standard abgebildet. `tests/serverlist_test.cpp` prüft Parser und URL-Migration; `--live URL` ruft die echte Quelle ab. Schreibweise im Programm und in Texten: „appleJuice“.
