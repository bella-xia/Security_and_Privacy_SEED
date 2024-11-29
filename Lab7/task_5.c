#include <stdio.h>
#include <openssl/bn.h>

#define NBITS 256

void printBN(char *msg, BIGNUM * a)
{
   /* Use BN_bn2hex(a) for hex string
    * Use BN_bn2dec(a) for decimal string */
   char * number_str = BN_bn2hex(a);
   printf("%s %s\n", msg, number_str);
   OPENSSL_free(number_str);
}

int main ()
{

  BN_CTX *ctx = BN_CTX_new();

  BIGNUM *e = BN_new();
  BIGNUM *n = BN_new();
  BIGNUM *signed_m = BN_new();
  BIGNUM *signed_m_w_corrupted_s = BN_new();
  BIGNUM *expected_m = BN_new();
  BIGNUM *s = BN_new();
  BIGNUM *corrupted_s = BN_new();

  BN_hex2bn(&n, "AE1CD4DC432798D933779FBD46C6E1247F0CF1233595113AA51B450F18116115");
  BN_hex2bn(&s, "643D6F34902D9C7EC90CB0B2BCA36C47FA37165C0005CAB026C0542CBDB6802F");
  BN_hex2bn(&corrupted_s, "643D6F34902D9C7EC90CB0B2BCA36C47FA37165C0005CAB026C0542CBDB6803F");
  BN_hex2bn(&expected_m, "4c61756e63682061206d697373696c652e");
  BN_hex2bn(&e, "010001");

  BN_mod_exp(signed_m, s, e, n, ctx);
  BN_mod_exp(signed_m_w_corrupted_s, corrupted_s, e, n, ctx);
  printBN("Decrypted Signed message: m = s^e mod n = ", signed_m);
  printBN("Decrypted Signed message with corrupted s: m = s'^e mod n = ", signed_m_w_corrupted_s);
  printBN("Excepted signed message 'Launch a missile.': ", expected_m);

  BN_clear_free(e);
  BN_clear_free(n);
  BN_clear_free(signed_m);
  BN_clear_free(corrupted_s);
  BN_clear_free(signed_m_w_corrupted_s);
  BN_clear_free(s);
  BN_clear_free(expected_m);

  return 0;
}






