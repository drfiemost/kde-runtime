/* This file is part of the KDE project
   Copyright (C) 2001 George Staikos <staikos@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "cbc.h"
#include <string.h>
#include <kdebug.h>

CipherBlockChain::CipherBlockChain(BlockCipher *cipher, bool useECBforReading) :
    _cipher(cipher)
    , _useECBforReading(useECBforReading)
{
    _next = 0L;
    _register = 0L;
    _len = -1;
    _reader = _writer = 0L;
    if (cipher) {
        _blksz = cipher->blockSize();
    }
}

CipherBlockChain::~CipherBlockChain()
{
    delete[](char *)_register;
    _register = 0L;
    delete[](char *)_next;
    _next = 0L;
}

bool CipherBlockChain::setKey(void *key, int bitlength)
{
    if (_cipher) {
        return _cipher->setKey(key, bitlength);
    }
    return false;
}

int CipherBlockChain::keyLen() const
{
    if (_cipher) {
        return _cipher->keyLen();
    }
    return -1;
}

bool CipherBlockChain::variableKeyLen() const
{
    if (_cipher) {
        return _cipher->variableKeyLen();
    }
    return false;
}

bool CipherBlockChain::readyToGo() const
{
    if (_cipher) {
        return _cipher->readyToGo();
    }
    return false;
}

void CipherBlockChain::initRegister() {
    if (_register == 0L) {
        size_t registerLen = _cipher->blockSize();
        _register = new unsigned char[registerLen];
        _len = registerLen;
    }
    memset(_register, 0, _len);
}

int CipherBlockChain::encrypt(void *block, int len)
{
    if (_cipher && !_reader) {
        int rc = -1;

        _writer |= 1;

        initRegister();

        if ((len % _len) >0) {
            kDebug() << "Block length given encrypt (" << len << ") is not a multiple of " << _len;
            return -1;
        }

        char *elemBlock = static_cast<char*>(block);
        for (int b = 0; b < len/_len; b++) {

            // This might be optimizable
            char *tb = static_cast<char*>(elemBlock);
            for (int i = 0; i < _len; i++) {
                *tb++ ^= ((char *)_register)[i];
            }

            rc = _cipher->encrypt(elemBlock, _len);

            if (rc != -1) {
                memcpy(_register, elemBlock, _len);
            }
            elemBlock += _len;
        }

        return rc;
    }
    return -1;
}

// This is the old decrypt method, that was decrypting using ECB
// instead of CBC
int CipherBlockChain::decryptECB(void *block, int len) {
    if (_cipher && !_writer) {
        int rc;

        _reader |= 1;

        if (!_register) {
            _register = new unsigned char[len];
            _len = len;
            memset(_register, 0, len);
        } else if (len > _len) {
            return -1;
        }

        if (!_next) {
            _next = new unsigned char[_len];
        }
        memcpy(_next, block, _len);

        rc = _cipher->decrypt(block, len);

        if (rc != -1) {
            // This might be optimizable
            char *tb = (char *)block;
            for (int i = 0; i < len; i++) {
                tb[i] ^= ((char *)_register)[i];
            }
        }

        void *temp;
        temp = _next;
        _next = _register;
        _register = temp;

        return rc;
    }
    return -1;
}

int CipherBlockChain::decrypt(void *block, int len)
{
    if (_useECBforReading) {
        kDebug() << "decrypting using ECB!";
        return decryptECB(block, len);
    }

    if (_cipher && !_writer) {
        int rc = 0;

        _reader |= 1;

        initRegister();

        if ((len % _len) >0) {
            kDebug() << "Block length given for decrypt (" << len << ") is not a multiple of " << _len;
            return -1;
        }

        char *elemBlock = static_cast<char*>(block);
        for (int b = 0; b < len/_len; b++) {
            if (_next == 0L) {
                _next = new unsigned char[_len];
            }
            memcpy(_next, elemBlock, _len);

            int bytesDecrypted = _cipher->decrypt(elemBlock, _len);

            if (bytesDecrypted != -1) {
                rc += bytesDecrypted;
                // This might be optimizable
                char *tb = (char *)elemBlock;
                for (int i = 0; i < _len; i++) {
                    *tb++ ^= ((char *)_register)[i];
                }
            }

            void *temp;
            temp = _next;
            _next = _register;
            _register = temp;

            elemBlock += _len;
        }

        return rc;
    }
    return -1;
}

