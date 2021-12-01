#!/bin/sh
wget -O ignore.tar.gz http://bobbycar.commanderred.xyz/ignore.tar.gz.gpg
gpg --quiet --batch --yes --decrypt --passphrase="$IGNORE_KEY" --output ./ignore.tar.gz ignore.tar.gz.gpg
tar xzf ignore.tar.gz
