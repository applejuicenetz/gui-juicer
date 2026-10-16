#!/bin/sh
# Baut das Flatpak-Bundle für die Architektur des Hosts aus dem Linux-Archiv.
# Voraussetzung: dist/juicer-1.3.1-linux-<arch>.tar.gz (packaging/build-linux.sh)
# und flatpak-builder mit org.freedesktop.Platform//25.08 und Sdk.
set -eu
root=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
case "$(uname -m)" in
  x86_64) arch=amd64 ;;
  aarch64) arch=arm64 ;;
  *) printf 'Nicht unterstützte Architektur: %s\n' "$(uname -m)" >&2; exit 1 ;;
esac
archive="$root/dist/juicer-1.3.1-linux-$arch.tar.gz"
[ -f "$archive" ] || { printf 'Fehlt: %s\n' "$archive" >&2; exit 1; }
stage="$root/dist/flatpak-stage"
rm -rf "$stage"
mkdir -p "$stage"
tar -xzf "$archive" -C "$stage"
cp "$root/packaging/flatpak/juicer.sh" "$stage/juicer.sh"
cp "$root/packaging/flatpak/io.github.applejuicenetz.juicer.desktop" "$stage/"
cp "$root/packaging/flatpak/io.github.applejuicenetz.juicer.metainfo.xml" "$stage/"
cp "$root/packaging/flatpak/juicer-128.png" "$stage/juicer.png"
build="$root/dist/flatpak-build"
repo="$root/dist/flatpak-repo"
rm -rf "$build"
flatpak-builder --user --force-clean --disable-rofiles-fuse --repo="$repo" "$build" \
  "$root/packaging/flatpak/io.github.applejuicenetz.juicer.yaml"
flatpak build-bundle "$repo" "$root/dist/juicer-1.3.1-linux-$arch.flatpak" io.github.applejuicenetz.juicer
rm -rf "$stage" "$build" "$repo"
sha256sum "$root/dist/juicer-1.3.1-linux-$arch.flatpak"
