FROM debian@sha256:3783cc01769c7b2b1b83a5c5ad96c815348e28ed7da68e2e3687004faa906251
RUN apt-get update && apt-get install -y --no-install-recommends wine=8.0~repack-4 wine32=8.0~repack-4 && rm -rf /var/lib/apt/lists/*
ENV WINEDEBUG=-all WINEARCH=win32 WINEPREFIX=/tmp/rbr-wine
WORKDIR /work
ENTRYPOINT ["wine"]
