/*-
 * Copyright 2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*
 * An example that uses EVP_PKEY_encrypt and EVP_PKEY_decrypt methods
 * to encrypt and decrypt data using an RSA keypair.
 * RSA encryption produces different encrypted output each time it is run,
 * hence this is not a known answer test.
 */

#include <iostream>

#include <openssl/evp.h>

/* 定义算法名称 */
#define ALGO_NAME "RSA"

/* 定义缓冲区长度 */
#define DATA_BUF_LEN 1024

/* 全局缓冲区 */
static unsigned char DATA_BUF[DATA_BUF_LEN] = { 0 };

/* 定义二进制数据块 */
struct BIN_DATA {
    unsigned char *data; /* 数据首地址 */
    size_t len;          /* 数据长度（字节） */
};

/* 非对称加密 */
int asym_encrypt(EVP_PKEY_CTX *ctx, struct BIN_DATA *in, struct BIN_DATA *enc)
{
    size_t blen = 0;

    if (!ctx || !in || !enc) {
        return 0;
    }

    /* 先获取加密缓冲区大小 */
    if (EVP_PKEY_encrypt(ctx, NULL, &blen, in->data, in->len) != 1) {
        return 0;
    }

    if (blen > enc->len) {
        std::cout << "缓冲区不足！";
        return 0;
    }

    /* 非对称加密 */
    if (EVP_PKEY_encrypt(ctx, enc->data, &enc->len, in->data, in->len) != 1) {
        return 0;
    }
    return 1;
}

/* 非对称解密 */
int asym_decrypt(EVP_PKEY_CTX *ctx, struct BIN_DATA *enc, struct BIN_DATA *dec)
{
    size_t blen = 0;

    if (!ctx || !enc || !dec) {
        return 0;
    }

    /* 先获取加密缓冲区大小 */
    if (EVP_PKEY_decrypt(ctx, NULL, &blen, enc->data, enc->len) != 1) {
        return 0;
    }

    if (blen > dec->len) {
         std::cout <<"缓冲区不足！";
        return 0;
    }

    /* 非对称解密 */
    if (EVP_PKEY_decrypt(ctx, dec->data, &dec->len, enc->data, enc->len) != 1) {
        return 0;
    }
    return 1;
}

/* Openssl 这个版本暂时没有实现高层次 api 对 SM2_3 的支持，所以这里用 RSA 算法做演示 */
int main(int argc, char *argv[]) 
{
    EVP_PKEY_CTX *ectx = NULL;
    EVP_PKEY_CTX *dctx = NULL;

    /* 定义原文 */
    char data[] = "12345678901234567890abcdefgABCDEFGIOPBNM12356789";
    int len = sizeof(data);

    /* 原文数据 */
    struct BIN_DATA in = {
        (unsigned char *)data,
        (size_t)len
    };

    /* 加密缓冲区 */
    struct BIN_DATA enc = {
        DATA_BUF,
        DATA_BUF_LEN / 2
    };

    /* 解密缓冲区 */
    struct BIN_DATA dec = {
        DATA_BUF + DATA_BUF_LEN / 2,
        DATA_BUF_LEN / 2
    };

    /* 首先生成非对称**对 */
    EVP_PKEY *pair = EVP_PKEY_Q_keygen(NULL, NULL, ALGO_NAME, 2048);

    if (!pair) {
         std::cout <<"生成**失败";
        return 0;
    }

    /* 获取非对称算法上下文 */
    ectx = EVP_PKEY_CTX_new_from_pkey(NULL, pair, NULL);
    if (!ectx) {
         std::cout << "生成非对称算法上下文失败";
        goto end;
    }

    /* 加密前初始化上下文 */
    if (EVP_PKEY_encrypt_init_ex(ectx, NULL) != 1) {
         std::cout <<"初始化非对称算法上下文失败";
        goto end;
    }

    /* 加密 */
    if (asym_encrypt(ectx, &in, &enc) != 1) {
         std::cout <<"加密失败";
        goto end;
    }

    dctx = EVP_PKEY_CTX_new_from_pkey(NULL, pair, NULL);
    if (!dctx) {
         std::cout <<"生成非对称算法上下文失败";
        goto end;
    }

    /* 解密前初始化上下文 */
    if (EVP_PKEY_decrypt_init_ex(dctx, NULL) != 1) {
         std::cout <<"初始化非对称算法上下文失败";
        goto end;
    }

    /* 解密 */
    if (asym_decrypt(dctx, &enc, &dec) != 1) {
         std::cout <<"解密失败";
        goto end;
    }

    /* 打印信息 */

    /* 打印原文数据 */
     std::cout <<"原文数据" << in.data << in.len << std::endl;

    /* 打印加密数据 */
     std::cout <<"加密数据" << enc.data << enc.len<< std::endl;

    /* 打印解密数据 */
     std::cout <<"解密数据" << dec.data << dec.len<< std::endl;

end:
    if (dctx) {
        EVP_PKEY_CTX_free(dctx);
    }

    if (ectx) {
        EVP_PKEY_CTX_free(ectx);
    }

    EVP_PKEY_free(pair);
    return 0;
}