#include <eccrypto.h>
#include <ecp.h>
#include <magnus.hpp>
#include <osrng.h>
#include <xed25519.h>

int main()
{
    using namespace CryptoPP;
    OID CURVE = ASN1::brainpoolP512r1();
    AutoSeededRandomPool rng;
    ECDH<ECP>::Domain dhA(CURVE), dhB(CURVE);

    SecByteBlock privA(dhA.PrivateKeyLength()), pubA(dhA.PublicKeyLength());
    SecByteBlock privB(dhB.PrivateKeyLength()), pubB(dhB.PublicKeyLength());

    dhA.GenerateKeyPair(rng, privA, pubA);
    dhB.GenerateKeyPair(rng, privB, pubB);

    if (dhA.AgreedValueLength() != dhB.AgreedValueLength())
        throw std::runtime_error("Shared shared size mismatch");

    SecByteBlock sharedA(dhA.AgreedValueLength()), sharedB(dhB.AgreedValueLength());

    if (!dhA.Agree(sharedA, privA, pubB))
        throw std::runtime_error("Failed to reach shared secret (A)");

    if (!dhB.Agree(sharedB, privB, pubA))
        throw std::runtime_error("Failed to reach shared secret (B)");

    Integer ssa, ssb;

    ssa.Decode(sharedA.BytePtr(), sharedA.SizeInBytes());
    std::cout << "(A): " << std::hex << ssa << std::endl;

    ssb.Decode(sharedB.BytePtr(), sharedB.SizeInBytes());
    std::cout << "(B): " << std::hex << ssb << std::endl;

    if (ssa != ssb)
        throw std::runtime_error("Failed to reach shared secret (C)");

    std::cout << "Agreed to shared secret" << std::endl;
}
