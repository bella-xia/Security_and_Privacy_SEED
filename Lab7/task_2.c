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
  BIGNUM *d = BN_new();
  BIGNUM *m = BN_new();
  BIGNUM *m_verify = BN_new();
  BIGNUM *c = BN_new();

  BN_hex2bn(&n, "DCBFFE3E51F62E09CE7032E2677A78946A849DC4CDDE3A4D0CB81629242FB1A5");
  BN_hex2bn(&m, "4120746f702073656372657421");
  BN_hex2bn(&d, "74D806F9F3A62BAE331FFE3F0A68AFE35B3D2E4794148AACBC26AA381CD7D30D");
  BN_hex2bn(&e, "010001");


  BN_mod_exp(c, m, e, n, ctx);
  printBN("Encryption: c = m^e mod n = ", c);

  BN_mod_exp(m_verify, c, d, n, ctx);
  printBN("Encryption Verification: ", m_verify);
  printBN("Expected Value: ", m);

  BN_clear_free(e);
  BN_clear_free(n);
  BN_clear_free(d);
  BN_clear_free(m);
  BN_clear_free(m_verify);
  BN_clear_free(c);

  return 0;
}






