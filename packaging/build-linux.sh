#!/bin/sh
set -eu
arch=${1:-arm64}
case "$arch" in arm64|amd64) ;; *) printf 'Unsupported architecture: %s\n' "$arch" >&2; exit 1;; esac
root=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
image=juicer-build:qt4-$arch
docker build --platform "linux/$arch" -f "$root/Dockerfile.qt4" -t "$image" "$root"
mkdir -p "$root/dist"
docker run --rm --platform "linux/$arch" -v "$root:/source:ro" -v "$root/dist:/output" "$image" sh -ec '
mkdir -p /build
cp -a /source/src /build/src
cp /source/juicer.pro /build/
cp -a /source/tests /build/tests
cd /build
find . -name Makefile -delete
find . -name "*.o" -delete
find . -name "moc_*.cpp" -delete
find . -name "qrc_*.cpp" -delete
find src -name "ui_*.h" -delete
qmake juicer.pro
make -j4
cd tests
qmake tests.pro
make -j2
Xvfb :88 -screen 0 1024x768x24 >/build/xvfb.log 2>&1 &
export DISPLAY=:88
sleep 1
./progress_test
qmake updatecheck_test.pro
make -j2
./updatecheck_test
qmake newsfeed_test.pro
make -j2
./newsfeed_test
qmake serverlist_test.pro
make -j2
./serverlist_test
cd /build
mkdir -p /output/stage/bin /output/stage/lib
cp src/juicer /output/stage/bin/
strip /output/stage/bin/juicer
ldd src/juicer | while read name arrow path rest; do
 case "$name" in libQt*|libaudio*) cp -L "$path" /output/stage/lib/;; esac
done
cp -L /opt/openssl-1.0.2/lib/libssl.so.1.0.0 /opt/openssl-1.0.2/lib/libcrypto.so.1.0.0 /output/stage/lib/
# Qt4 muss OpenSSL aus dem Paket finden, nicht aus der Build-Umgebung.
cd /build/tests
env LD_LIBRARY_PATH=/output/stage/lib ./newsfeed_test
cd /build
printf "#!/bin/sh\nHERE=\$(CDPATH= cd -- \"\$(dirname \"\$0\")\" && pwd)\nexport LD_LIBRARY_PATH=\"\$HERE/lib\${LD_LIBRARY_PATH:+:\$LD_LIBRARY_PATH}\"\nexec \"\$HERE/bin/juicer\" \"\$@\"\n" > /output/stage/juicer
chmod +x /output/stage/juicer
cp /source/LICENSE /output/stage/
cp /source/README.md /output/stage/
file /output/stage/bin/juicer
set +e
timeout 5 /output/stage/juicer >/output/smoke.log 2>&1
result=$?
set -e
[ "$result" = 124 ] || { cat /output/smoke.log; exit 1; }
arch=$(dpkg --print-architecture)
tar -czf /output/juicer-1.3.1-linux-$arch.tar.gz -C /output/stage .
rm -rf /output/stage
'
sha256sum "$root/dist/juicer-1.3.1-linux-$arch.tar.gz"
